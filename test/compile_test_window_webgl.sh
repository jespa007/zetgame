#!/bin/bash
echo EMSCRIPTEN_TOOLCHAIN_PATH=$EMSCRIPTEN_TOOLCHAIN_PATH
if [ "$EMSCRIPTEN_TOOLCHAIN_PATH" = "" ]
then
	echo "EMSCRIPTEN_TOOLCHAIN_PATH environment variable not set"
	exit
fi

$EMSCRIPTEN_TOOLCHAIN_PATH/emcc test_window_webgl.c -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -o test_window_webgl.html