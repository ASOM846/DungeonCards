cmake_minimum_required(VERSION 3.16)

project(mineswepper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Opcje ---------------------------------------------------------------
# Włącz FAST_BUILD domyślnie — wyłącz ręcznie z -DFAST_BUILD=OFF jeśli chcesz
option(FAST_BUILD "Enable fast build helpers (ccache/sccache, PCH, UNITY_BUILD,
faster compile flags)" ON)
option(RAYLIB_USE_SUBMODULE "Use external/raylib subdirectory if present" OFF)

# Domyślny typ buildu - Debug (szybsza kompilacja i sensowne dla devel)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# --- FAST_BUILD: compiler launcher (sccache/ccache) i szybkie flagi ----------
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
    # Debug, szybsza kompilacja
    add_compile_options(/Od /Zi)
    add_compile_options(/W3)
  else()
    # szybkie flagi devel: brak optymalizacji, debug symbols, mniej I/O overhead
    add_compile_options(-O0 -g -pipe)
    # jeśli chcesz widzieć warningi, usuń -w
    # add_compile_options(-w)
  endif()
endif()

# --- Zbierz źródła (GLOB_RECURSE dla wygody) ------------------------------
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(mineswepper ${SRC_FILES})

# --- UNITY_BUILD (redukuje spawn overhead dla wielu plików) ---------------
# właściwość UNITY_BUILD dostępna w CMake >= 3.24
if(FAST_BUILD AND (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24"))
  message(STATUS "FAST_BUILD: enabling UNITY_BUILD (merge many translation units)")
  set_target_properties(mineswepper PROPERTIES UNITY_BUILD ON)
  set_target_properties(mineswepper PROPERTIES UNITY_BUILD_BATCH_SIZE 16)
endif()

# --- Znajdowanie raylib -----------------------------------------------
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

target_link_libraries(mineswepper PRIVATE ${RAYLIB_TARGET})

# --- Drobne ustawienia kompilatora (opcjonalne) ----------------------------
if(MSVC)
  target_compile_options(mineswepper PRIVATE /W4)
else()
  target_compile_options(mineswepper PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Informacje na konfiguracji ------------------------------------------
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "FAST_BUILD = ${FAST_BUILD}")
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMake version: ${CMAKE_VERSION}")

# Rekomendacja:
#  - zainstaluj: sudo apt install ninja-build ccache    (albo sccache)
#  - konfiguracja: cmake -S . -B build -G Ninja
#  - build: cmake --build build --parallel $(nproc)
#  - aby wymusić wyłączenie FAST_BUILD: cmake -S . -B build -G Ninja -DFAST_BUILD=OFF
