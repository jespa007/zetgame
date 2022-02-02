#!/bin/bash

source build_3rdparty_common.sh

# The third parties
YASM_VERSION="yasm-1.3.0"
NASM_VERSION="nasm-2.14.03rc2"
X264_VERSION="x264-stable"
LIBOGG_VERSION="libogg-1.3.4"
LIBVORBIS_VERSION="libvorbis-1.3.6"
FFMPEG_VERSION="FFmpeg-n2.8.15"

YASM_FILE=$YASM_VERSION".tar.gz"
NASM_FILE=$NASM_VERSION".tar.gz"
X264_FILE=$X264_VERSION".tar.gz"
LIBOGG_FILE=$LIBOGG_VERSION".tar.gz"
LIBVORBIS_FILE=$LIBVORBIS_VERSION".tar.gz"
FFMPEG_FILE=$FFMPEG_VERSION".tar.gz"


EXTENSION_X264=$EXTENSION_DYNAMIC_DLL
RESULT_FILE_SWSCALE="libswscale"$EXTENSION_DYNAMIC_DLL
PRE_EXTENSION_OGG_LIB="lib"
LIBVORBIS_EXTENSION=$EXTENSION_DYNAMIC_DLL

if [ $machine = "mingw" ]; then
	
	EXTENSION_X264="-157"$EXTENSION_DYNAMIC_DLL
	PRE_EXTENSION_OGG_LIB=""
	LIBVORBIS_EXTENSION="file-3"$EXTENSION_DYNAMIC_DLL
	RESULT_FILE_SWSCALE="swscale-3"$EXTENSION_DYNAMIC_DLL
fi


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
		echo "clean up"
		rm -r $THIRD_PARTY_INSTALL_DIR"/lib/cmake"
	fi
fi