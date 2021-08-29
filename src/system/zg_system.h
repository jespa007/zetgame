#ifndef __ZG_SYSTEM_H__
#define __ZG_SYSTEM_H__



#include 	<stdio.h>
#include 	<stdint.h>
#include 	<stdbool.h>
#include	<time.h>
#include	<memory.h>
#include	<math.h>
#include	<float.h>
#include	<assert.h>
#include 	<locale.h>


#if defined(_WIN32 ) // win32
	#include	<winsock2.h>
	#include	<windows.h>
	#include	<conio.h>
	#include	<tchar.h>
	#include 	<Rpc.h>
#endif

#if defined(__GNUC__) // linux or mingw

	#include	<sys/stat.h>
	#include	<sys/types.h>
	#include	<unistd.h>
	#include	<memory.h>
	#include	<fcntl.h>
	#include	<dirent.h>

	#if defined(__linux__)

		#include	<dlfcn.h>
		#include	<sys/ioctl.h>
		#include	<termios.h>
		#include 	<uuid/uuid.h>
	#endif
#endif

//----------------------------------------
// system

/* Libraries completely written in C, so we need to tell the C++ compiler that so it links correctly. */
#include <jpeglib.h>

#include <freetype2/ft2build.h>
#include <freetype2/freetype.h>
#include <freetype2/ftglyph.h>

#ifdef __WITH_FFMPEG__
	#include 	<libavcodec/avcodec.h>
	#include 	<libavformat/avformat.h>
	#include 	<libswscale/swscale.h>
	#include 	<libavutil/opt.h>
	#include 	<libavutil/imgutils.h>
	#include 	<libavcodec/avcodec.h>
	#include 	<libavformat/avformat.h>
	#include 	<libswscale/swscale.h>
	#include 	<libavutil/samplefmt.h>
	#include 	<libavutil/opt.h>
	#include 	<libavutil/channel_layout.h>
	#include 	<libavutil/samplefmt.h>
	#include 	<libswresample/swresample.h>
#endif


// xmp is build as static so we have to define BUILDING_STATIC
#include 	<xmp.h>

//----------------------------------------
// system

#ifdef __MEMMANAGER__
#include "memmgr.h"
#endif

#include "3rdparty/gifdec/gifdec.h"
#include "3rdparty/cJSON/cJSON.h"
#include "3rdparty/xml.c/src/xml.h"
#include "3rdparty/lodepng/lodepng.h"
#include "jpeg/jpeg.h"
#include "sdl/zg_sdl.h"



#include 	"zetpacker.h"
#include 	"zetnet.h"


#include	"common.h"
#include	"zg_memory.h"
#include	"util-fmemopen.h"
#include	"gl/glextra.h"


#include	"Term.h"
#include	"Path.h"
#include	"Log.h"
#include	"List.h"
#include	"MapString.h"
#include	"MapStringIterator.h"
#include	"MapInt.h"
#include	"StrUtils.h"
#include	"BufferByte.h"
#include	"BufferShort.h"
#include	"BufferFloat.h"
#include	"Directory.h"
#include	"File.h"

#include	"FileSystem.h"
#include	"Bit.h"
#include	"Guid.h"


#if  !defined(MAX)
#define  MAX(a,  b)              ((a)  >  (b)  ?  (a)  :  (b))
#define  MIN(a,  b)              ((a)  <  (b)  ?  (a)  :  (b))
#endif


typedef  struct{
	unsigned        id;
	const char  *	str;
}EnumStr;


#endif
