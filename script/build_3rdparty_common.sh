#!/bin/bash

cd ..

THIRD_PARTY_SOURCES_DIR=$PWD"/src/3rdparty"

# update path to bind generated executables in 3rd party bin
PATH=$PATH:$PWD"/bin"

EXTENSION_STATIC_LIB=".a"
EXTENSION_DYNAMIC_LIB=".so"
EXTENSION_DYNAMIC_DLL=".so"

INSTALLED_DIR_LIBRARY_FILE="lib"
BUILD_DIR="build/gcc"

echo PKG_CONFIG_PATH=$PKG_CONFIG_PATH
echo LDFLAGS=$LDFLAGS

BUILD=""
TARGET_OS=""
HOST=""
ARCH=""
CROSS_PREFIX=""
EXTENSION_EXE=""

uname_out="$(uname -s)"
case "${uname_out}" in
    Linux*)     machine=unix;toolchain=gcc;;
    Darwin*)    machine=mac;toolchain=gcc;;
    CYGWIN*)    machine=cygwin;toolchain=gcc;;
    MINGW*)     machine=mingw;toolchain=gcc;;
	MSYS*)     machine=mingw;toolchain=gcc;;
    *)          echo  "unknow platform=":${uname_out};exit -1;
esac
echo "MACHINE:"${machine}
echo "TOOLCHAIN:"${toolchain}
THIRD_PARTY_BUILD_DIR=$PWD"/build/"${toolchain}
THIRD_PARTY_INSTALL_DIR=$PWD"/3rdparty/"${toolchain}
echo "BUILD DIR:"${THIRD_PARTY_BUILD_DIR}
echo "SOURCES DIR:"$THIRD_PARTY_SOURCES_DIR
echo "INSTALL DIR:"$THIRD_PARTY_INSTALL_DIR

COMMON_CMAKE_OPTIONS=" -H. -B"$BUILD_DIR" -DCMAKE_INSTALL_PREFIX:PATH="$THIRD_PARTY_INSTALL_DIR
COMMON_CONFIGURE_OPTIONS=" --disable-shared --enable-static --prefix="$THIRD_PARTY_INSTALL_DIR
export CFLAGS="-O2 -I"$THIRD_PARTY_INSTALL_DIR"/include"
export LDFLAGS="-L"$THIRD_PARTY_INSTALL_DIR"/lib"
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$THIRD_PARTY_INSTALL_DIR"/lib/pkgconfig"
CMAKE_CONFIG=($COMMON_CMAKE_OPTIONS)


if [ $machine = "mingw" ]; then
	
	EXTENSION_EXE=".exe"
	EXTENSION_DYNAMIC_DLL=".dll"
	EXTENSION_DYNAMIC_LIB=".dll.a"
	INSTALLED_DIR_LIBRARY_FILE="bin"

	HOST=x86_64-w64-mingw32
	CROSS_PREFIX=x86_64-w64-mingw32-
	ARCH=x86_64
	BUILD=$HOST
	TARGET_OS=mingw32
	COMMON_CONFIGURE_OPTIONS=$COMMON_CONFIGURE_OPTIONS" --build=$BUILD"
	CMAKE_CONFIG=($COMMON_CMAKE_OPTIONS -DCMAKE_COLOR_MAKEFILE=0xFF -G 'MinGW Makefiles' -DCMAKE_SH='CMAKE_SH-NOTFOUND')
	export BINARY_PATH=$THIRD_PARTY_INSTALL_DIR"/bin"
	export INCLUDE_PATH=$THIRD_PARTY_INSTALL_DIR"/include"
	export LIBRARY_PATH=$THIRD_PARTY_INSTALL_DIR"/lib"
else
	export CFLAGS=$CFLAGS" -fPIC"
fi


if ! [ -d "$THIRD_PARTY_BUILD_DIR" ] 
then
	mkdir -p $THIRD_PARTY_BUILD_DIR
fi

cd $THIRD_PARTY_BUILD_DIR