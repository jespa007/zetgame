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


#include "3rdparty/gifdec/gifdec.h"
#include "3rdparty/cJSON/cJSON.h"
#include "3rdparty/cXML/cXML.h"
#include "3rdparty/lodepng/lodepng.h"

#include	"3rdparty/util-fmemopen/util-fmemopen.h"
#include	"gl/glextra.h"

#include "jpeg/jpeg_mem.h"
#include "sdl/_zg_base_sdl_.h"

//----------------------------------------
// system

#ifdef __MEMMANAGER__
#include "memmgr.h"
#endif


#include 	"zetpacker.h"
#include 	"zetnet.h"

#include	"common.h"
#include	"ZG_Memory.h"


#include	"ZG_Term.h"
#include	"ZG_Path.h"
#include	"ZG_Log.h"
#include	"ZG_List.h"
#include	"ZG_MapString.h"
#include	"ZG_MapStringIterator.h"
#include	"ZG_MapInt.h"
#include	"ZG_String.h"
#include	"ZG_BufferByte.h"
#include	"ZG_BufferShort.h"
#include	"ZG_BufferFloat.h"
#include	"ZG_Directory.h"
#include	"ZG_File.h"

#include	"ZG_FileSystem.h"
#include	"ZG_Bit.h"
#include	"ZG_Guid.h"


#if  !defined(MAX)
#define  MAX(a,  b)              ((a)  >  (b)  ?  (a)  :  (b))
#define  MIN(a,  b)              ((a)  <  (b)  ?  (a)  :  (b))
#endif


typedef  struct{
	unsigned        id;
	const char  *	str;
}EnumStr;


#endif
