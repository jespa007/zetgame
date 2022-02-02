#!/bin/bash

source build_3rdparty_common.sh

ZLIB_VERSION="zlib-1.2.8"
SDL2_VERSION="SDL2-2.0.14"
LIBPNG_VERSION="libpng-1.6.37"
LIBJPEG_VERSION="jpeg-9b"
SDL2_IMAGE_VERSION="SDL2_image-2.0.0"
SDL2_NET_VERSION="SDL2_net-2.0.0"
FREETYPE2_VERSION="freetype-2.5.2"
LIBXMP_VERSION="libxmp-lite-4.5.0"

ZLIB_FILE=$ZLIB_VERSION".tar.gz"
SDL2_FILE=$SDL2_VERSION".tar.gz"
LIBPNG_FILE=$LIBPNG_VERSION".tar.gz"
LIBJPEG_FILE=$LIBJPEG_VERSION".tar.gz"
SDL2_IMAGE_FILE=$SDL2_IMAGE_VERSION".tar.gz"
SDL2_NET_FILE=$SDL2_NET_VERSION".tar.gz"
FREETYPE2_FILE=$FREETYPE2_VERSION".tar.gz"
LIBXMP_FILE=$LIBXMP_VERSION".tar.gz"

RESULT_FILE_Z="libz.a"

if [ $machine = "mingw" ]; then
	
	RESULT_FILE_Z="libzlibstatic.a"

fi

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
	make -j2  install -C $BUILD_DIR

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

if ! [ -f $THIRD_PARTY_INSTALL_DIR"/lib/libxmp-lite"$EXTENSION_STATIC_LIB ] 
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

if ! [ "$TARGET_OS" = "mingw32" ]
then

	if [ -d $THIRD_PARTY_INSTALL_DIR"/lib/cmake" ] 
	then
		echo "clean up"
		rm -r $THIRD_PARTY_INSTALL_DIR"/lib/cmake"
	fi
fi