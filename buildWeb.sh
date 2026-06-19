#!/usr/bin/env bash
set -e

source "$HOME/emsdk/emsdk_env.sh"

emcc -o index.html \
  $(find src -type f -name "*.cpp") \
  -Isrc \
  -I"$HOME/libs/raylib/src" \
  "$HOME/libs/raylib/src/libraylib.web.a" \
  -Os -Wall \
  -DPLATFORM_WEB \
  -sUSE_GLFW=3 \
  -sASYNCIFY \
  -sALLOW_MEMORY_GROWTH=1 \
  --shell-file minshell.html \
  --preload-file assets
