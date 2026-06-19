cmake_minimum_required(VERSION 3.16)

project(dungeoncards VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FAST_BUILD "Enable fast build helpers (ccache/sccache, PCH, UNITY_BUILD,
faster compile flags)" ON)
option(RAYLIB_USE_SUBMODULE "Use external/raylib subdirectory if present" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

if(FAST_BUILD)
  find_program(SCCACHE_EXE sccache)
  find_program(CCACHE_EXE ccache)

  if(SCCACHE_EXE)
    message(STATUS "FAST_BUILD: using sccache: ${SCCACHE_EXE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE_EXE} CACHE STRING "" FORCE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_EXE} CACHE STRING "" FORCE)
  elseif(CCACHE_EXE)
    message(STATUS "FAST_BUILD: using ccache: ${CCACHE_EXE}")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_EXE} CACHE STRING "" FORCE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXE} CACHE STRING "" FORCE)
  else()
    message(STATUS "FAST_BUILD: no sccache/ccache found in PATH")
  endif()

  if(MSVC)
    add_compile_options(/Od /Zi)
    add_compile_options(/W3)
  else()
    add_compile_options(-O0 -g -pipe)
  endif()
endif()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(dungeoncards ${SRC_FILES})

if(FAST_BUILD AND (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24"))
  message(STATUS "FAST_BUILD: enabling UNITY_BUILD (merge many translation units)")
  set_target_properties(dungeoncards PROPERTIES UNITY_BUILD ON)
  set_target_properties(dungeoncards PROPERTIES UNITY_BUILD_BATCH_SIZE 16)
endif()

set(RAYLIB_TARGET "")

find_package(raylib CONFIG QUIET)
if(raylib_FOUND)
  if(TARGET raylib)
    set(RAYLIB_TARGET raylib)
  elseif(TARGET raylib::raylib)
    set(RAYLIB_TARGET raylib::raylib)
  endif()
endif()

if(NOT RAYLIB_TARGET)
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(RAYLIB_PKG QUIET raylib)
    if(RAYLIB_PKG_FOUND)
      add_library(raylib::pkg INTERFACE IMPORTED)
      target_include_directories(raylib::pkg INTERFACE ${RAYLIB_PKG_INCLUDE_DIRS})
      target_link_libraries(raylib::pkg INTERFACE ${RAYLIB_PKG_LIBRARIES})
      target_compile_options(raylib::pkg INTERFACE ${RAYLIB_PKG_CFLAGS_OTHER})
      set(RAYLIB_TARGET raylib::pkg)
    endif()
  endif()
endif()

if(NOT RAYLIB_TARGET AND RAYLIB_USE_SUBMODULE)
  if(EXISTS "${CMAKE_SOURCE_DIR}/external/raylib/CMakeLists.txt")
    add_subdirectory(external/raylib)
    if(TARGET raylib)
      set(RAYLIB_TARGET raylib)
    elseif(TARGET raylib::raylib)
      set(RAYLIB_TARGET raylib::raylib)
    endif()
  endif()
endif()

if(NOT RAYLIB_TARGET)
  message(FATAL_ERROR
      "raylib not found.\n"
      "Na Linux: zainstaluj pakiet dev i pkg-config (np. 'sudo apt install libraylib-dev pkg-config').\n"
      "Lub dodaj submodule external/raylib i ustaw RAYLIB_USE_SUBMODULE=ON."
    )
endif()

target_link_libraries(dungeoncards PRIVATE ${RAYLIB_TARGET})
add_custom_command(
    TARGET dungeoncards POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:dungeoncards>/assets"
    COMMENT "Kopiowanie folderu assets do katalogu wyjściowego..."
)

if(MSVC)
  target_compile_options(dungeoncards PRIVATE /W4)
else()
  target_compile_options(dungeoncards PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "FAST_BUILD = ${FAST_BUILD}")
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMake version: ${CMAKE_VERSION}")
