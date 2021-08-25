#!/bin/bash

THIRD_PARTY_SOURCES_DIR=$PWD"/src/3rdparty"

# update path to bind generated executables in 3rd party bin
PATH=$PATH:$PWD"/bin"
 
# The third parties
YASM_VERSION="yasm-1.3.0"
NASM_VERSION="nasm-2.14.03rc2"
ZLIB_VERSION="zlib-1.2.8"
SDL2_VERSION="SDL2-2.0.14"
LIBPNG_VERSION="libpng-1.6.37"
LIBJPEG_VERSION="jpeg-9b"
SDL2_IMAGE_VERSION="SDL2_image-2.0.0"
SDL2_NET_VERSION="SDL2_net-2.0.0"
FREETYPE2_VERSION="freetype-2.5.2"
LIBXMP_VERSION="libxmp-lite-4.5.0"
X264_VERSION="x264-stable"
LIBOGG_VERSION="libogg-1.3.4"
LIBVORBIS_VERSION="libvorbis-1.3.6"
FFMPEG_VERSION="FFmpeg-n2.8.15"

YASM_FILE=$YASM_VERSION".tar.gz"
NASM_FILE=$NASM_VERSION".tar.gz"
ZLIB_FILE=$ZLIB_VERSION".tar.gz"
SDL2_FILE=$SDL2_VERSION".tar.gz"
LIBPNG_FILE=$LIBPNG_VERSION".tar.gz"
LIBJPEG_FILE=$LIBJPEG_VERSION".tar.gz"
SDL2_IMAGE_FILE=$SDL2_IMAGE_VERSION".tar.gz"
SDL2_NET_FILE=$SDL2_NET_VERSION".tar.gz"
FREETYPE2_FILE=$FREETYPE2_VERSION".tar.gz"
LIBXMP_FILE=$LIBXMP_VERSION".tar.gz"
X264_FILE=$X264_VERSION".tar.gz"
LIBOGG_FILE=$LIBOGG_VERSION".tar.gz"
LIBVORBIS_FILE=$LIBVORBIS_VERSION".tar.gz"
FFMPEG_FILE=$FFMPEG_VERSION".tar.gz"
SQLITE3_FILE=$SQLITE3_VERSION".tar.gz"

EXTENSION_STATIC_LIB=".a"
EXTENSION_DYNAMIC_LIB=".so"
EXTENSION_DYNAMIC_DLL=".so"
EXTENSION_X264=$EXTENSION_DYNAMIC_DLL
INSTALLED_DIR_LIBRARY_FILE="lib"
PRE_EXTENSION_OGG_LIB="lib"
RESULT_FILE_SWSCALE="libswscale"$EXTENSION_DYNAMIC_DLL
RESULT_FILE_Z="libz.a"

LIBVORBIS_EXTENSION=$EXTENSION_DYNAMIC_DLL
BUILD_DIR="build/gcc"
CMAKE_CONFIG=($COMMON_CMAKE_OPTIONS)

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



if [ $machine = "mingw" ]; then
	
	EXTENSION_EXE=".exe"
	EXTENSION_DYNAMIC_DLL=".dll"
	EXTENSION_DYNAMIC_LIB=".dll.a"
	EXTENSION_X264="-157"$EXTENSION_DYNAMIC_DLL
	PRE_EXTENSION_OGG_LIB=""
	LIBVORBIS_EXTENSION="file-3"$EXTENSION_DYNAMIC_DLL
	RESULT_FILE_SWSCALE="swscale-3"$EXTENSION_DYNAMIC_DLL
	RESULT_FILE_Z="libzlibstatic.a"
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
	mkdir $THIRD_PARTY_BUILD_DIR --parents
fi

cd $THIRD_PARTY_BUILD_DIR

#------------------------------------------------------------------------------------------------------------------------
#
# NASM
#

echo building nasm...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$NASM_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$NASM_FILE
fi

cd $NASM_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/bin/nasm"$EXTENSION_EXE ] 
then

	./configure --prefix=$THIRD_PARTY_INSTALL_DIR --host=$TARGET_OS

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..

#------------------------------------------------------------------------------------------------------------------------
#
# YASM
#

echo building yasm...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$YASM_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$YASM_FILE
fi

cd $YASM_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/bin/yasm"$EXTENSION_EXE ] 
then

	./configure --prefix=$THIRD_PARTY_INSTALL_DIR --build=$BUILD

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..




#------------------------------------------------------------------------------------------------------------------------
#
# ZLIB
#

echo building zlib...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$ZLIB_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$ZLIB_FILE
fi

cd $ZLIB_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/"$RESULT_FILE_Z ] 
then
	rm -rf build/gcc
	cmake "${CMAKE_CONFIG[@]}" -DBUILD_SHARED_LIBS:BOOL=OFF

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean -C $BUILD_DIR
	make -j2  install -C $BUILD_DIR

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..


#------------------------------------------------------------------------------------------------------------------------
#
# SDL2
#

echo building SDL2...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$SDL2_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$SDL2_FILE
fi

cd $SDL2_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libSDL2"$EXTENSION_STATIC_LIB ] 
then

	#rm -rf build/gcc #CMakeCache.txt CMakeFiles
	#cmake "${CMAKE_CONFIG[@]}" -DBUILD_SHARED_LIBS:BOOL=OFF
	./configure $COMMON_CONFIGURE_OPTIONS --host=$HOST

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean #-C $BUILD_DIR
	make -j2  install #-C $BUILD_DIR

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..

#------------------------------------------------------------------------------------------------------------------------
#
# LIBJPEG
#

echo building libjpeg...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$LIBJPEG_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$LIBJPEG_VERSION".tar.gz"
fi

cd $LIBJPEG_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libjpeg"$EXTENSION_STATIC_LIB ] 
then

	./configure $COMMON_CONFIGURE_OPTIONS

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..



#------------------------------------------------------------------------------------------------------------------------
#
# FREETYPE2
#

echo building freetype2...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$FREETYPE2_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$FREETYPE2_FILE
fi

cd $FREETYPE2_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libfreetype"$EXTENSION_STATIC_LIB ] 
then

	rm -rf build/gcc

	#./configure --enable-static --disable-shared  --with-harfbuzz=no --prefix=$THIRD_PARTY_INSTALL_DIR
	cmake "${CMAKE_CONFIG[@]}" -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true


	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean -C $BUILD_DIR
	make -j2  install --trace -C $BUILD_DIR

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..

#------------------------------------------------------------------------------------------------------------------------
#
# XMP
#

echo building xmp...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$LIBXMP_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$LIBXMP_FILE
fi

cd $LIBXMP_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libxmp"$EXTENSION_STATIC_LIB ] 
then

	./configure --disable-shared --enable-static --build=$BUILD --prefix=$THIRD_PARTY_INSTALL_DIR


	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2 install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..


#------------------------------------------------------------------------------------------------------------------------
#
# X264
#

echo building x264...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$X264_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$X264_FILE
fi

cd $X264_VERSION

#echo $THIRD_PARTY_INSTALL_DIR"/lib/libx264"$EXTENSION_STATIC_LIB
if ! [ -f  $THIRD_PARTY_INSTALL_DIR"/lib/libx264"$EXTENSION_STATIC_LIB ] 
then

	./configure  --host=$HOST --prefix=$THIRD_PARTY_INSTALL_DIR  --enable-static --disable-cli --disable-gpl --disable-opencl --disable-avs --disable-swscale --disable-lavf --disable-ffms --disable-gpac --disable-lsmash --disable-thread

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..

#------------------------------------------------------------------------------------------------------------------------
#
# LIBOGG
#

echo building libogg ...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$LIBOGG_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$LIBOGG_FILE
fi

cd $LIBOGG_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libogg"$EXTENSION_STATIC_LIB ] 
then

	#rm -rf CMakeCache.txt CMakeFiles

	#cmake CMakeLists.txt "${CMAKE_CONFIG[@]}" -DBUILD_SHARED_LIBS=0
	./configure  --build=$BUILD  --disable-shared --enable-static --prefix=$THIRD_PARTY_INSTALL_DIR

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi
	
	#if [ "$TARGET_OS" = "mingw32" ]
	#then
	#	mv $THIRD_PARTY_INSTALL_DIR"/bin/libogg.dll" $THIRD_PARTY_INSTALL_DIR"/bin/ogg.dll" 
	#fi

fi

cd ..

#------------------------------------------------------------------------------------------------------------------------
#
# LIBVORBIS
#

echo building libvorbis ...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$LIBVORBIS_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$LIBVORBIS_FILE
fi

cd $LIBVORBIS_VERSION

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libvorbis"$EXTENSION_STATIC_LIB ] 
then
	# libvorbis must be compiled as shared to be detected by ffmpeg
	./configure  --build=$BUILD  --disable-shared --enable-static --prefix=$THIRD_PARTY_INSTALL_DIR

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make -j2  install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..



#------------------------------------------------------------------------------------------------------------------------
#
# FFMPEG
#

echo building ffmpeg...

if ! [ -d $THIRD_PARTY_BUILD_DIR"/$FFMPEG_VERSION" ] 
then
	tar xf $THIRD_PARTY_SOURCES_DIR"/"$FFMPEG_FILE
fi

cd $FFMPEG_VERSION


if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libswscale"$EXTENSION_STATIC_LIB ] 
then

	./configure  --arch=$ARCH --target-os=$TARGET_OS --prefix=$THIRD_PARTY_INSTALL_DIR --disable-everything --disable-doc --enable-yasm --enable-static --disable-shared --enable-small --enable-avcodec --enable-avformat --enable-swresample --enable-swscale --enable-decoder=h264,libvorbis,libogg --enable-parser=h264 --enable-protocol=file --enable-demuxer=mov --enable-muxer=rawvideo,mp4 --enable-gpl --enable-libx264 --enable-libvorbis  --extra-cflags="-I$THIRD_PARTY_INSTALL_DIR/include" --extra-cxxflags="-I$THIRD_PARTY_INSTALL_DIR/include" --extra-ldflags="-L$THIRD_PARTY_INSTALL_DIR/lib" 
	

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

	make clean
	make install

	if ! [ $? -eq 0 ] 
	then
		exit 1
	fi

fi

cd ..


#------------------------------------------------------------------------------------------------------------------------

if ! [ "$TARGET_OS" = "mingw32" ]
then

	if [ -d $THIRD_PARTY_INSTALL_DIR"/lib/cmake" ] 
	then
		echo "removing cmake dir due conflicts with the path"
		rm $THIRD_PARTY_INSTALL_DIR"/lib/cmake" -R
	fi
fi
