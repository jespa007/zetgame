cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)	# To enable MSVC_RUNTIME_LIBRARY. It has effect only when policy CMP0091 is set to NEW prior to the first project() 
project(zetgame)


IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release )
ENDIF()

set(POST_NAME "")
set(IS_DEBUG FALSE)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(IS_DEBUG TRUE)
	SET( CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -D__DEBUG__ ")
	set( POST_NAME "_d")
endif()

MESSAGE ("-- Target: " ${CMAKE_BUILD_TYPE})


file(READ "zetgame.h" zetgame_h)

string(REGEX MATCH "ZETGAME_VERSION_MAJOR([\t-\ ][0-9])*" _ ${zetgame_h})
string(STRIP "${CMAKE_MATCH_1}" ZETGAME_VERSION_MAJOR)

string(REGEX MATCH "ZETGAME_VERSION_MINOR([\t-\ ][0-9])*" _ ${zetgame_h})
string(STRIP "${CMAKE_MATCH_1}" ZETGAME_VERSION_MINOR)

string(REGEX MATCH "ZETGAME_VERSION_PATCH([\t-\ ][0-9])*" _ ${zetgame_h})
string(STRIP "${CMAKE_MATCH_1}" ZETGAME_VERSION_PATCH)



MESSAGE("-- zetgame ${ZETGAME_VERSION_MAJOR}.${ZETGAME_VERSION_MINOR}.${ZETGAME_VERSION_PATCH}" )

set(ZETGAME_LIB_NAME "zetgame-${ZETGAME_VERSION_MAJOR}-${ZETGAME_VERSION_MINOR}-${ZETGAME_VERSION_PATCH}")

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# COMPILE PARAMS
#

message("-- Install drectory: " ${CMAKE_INSTALL_PREFIX})

set(ZETGAME_LIB_SRCS "")


if(MSVC)
	SET(COMPILER "msvc")
	MESSAGE ("-- Plataform: MSVC" )
	
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	#disable C4103 warning because is perfectly valid. Only MSVC complains about it
	add_definitions(/wd4103)
	add_definitions(/wd4800)
	add_definitions(/wd4244)
	
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	
	
	if(BUILD_SHARED_LIBS)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	else()
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()	

else()

	SET(COMPILER "gcc")

	SET( CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -D__STDC_CONSTANT_MACROS  -pthread -fomit-frame-pointer -std=c99 -pipe -static-libgcc -Wall -Wextra -pedantic -Wstrict-prototypes -Wsign-compare -Wtype-limits ")
	  
	if (MINGW)
		MESSAGE("-- Plataform: MINGW" )
		message("-- Mingw path: $ENV{MINGWDIR} ")
		SET( CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -DEZXML_NOMMAP " )
		set(CMAKE_LEGACY_MINGW_WIN32 0)
	elseif(UNIX)
    	MESSAGE ("-- Plataform: UNIX" )
    else()
    	MESSAGE ("-- Plataform: Unknown" )
	endif()
	

	IF ( IS_DEBUG)
		set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O0 -g ")
		
		IF (EXISTS ${PROJECT_SOURCE_DIR}/../../memmgr)
		
			message("-- Memmanager: Yes")
			SET( CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -D__MEMMANAGER__")
			include_directories (
				${PROJECT_SOURCE_DIR}/../../memmgr
			)
			
	   		set(ZETGAME_LIB_SRCS ${ZETGAME_LIB_SRCS} ${PROJECT_SOURCE_DIR}/../../memmgr/memmgr.c)
		
		ELSE()
	
			if(UNIX)
				if( NOT "${DIST}" STREQUAL "Alpine")
					set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -fsanitize=address ")
					message("-- Memmanager: -fsanitize=address")
				else()
					message("-- Memmanager: -fsanitize NOT supported")
				endif()
			 else()
			 	message("-- Memmanager: OFF (../../memmgr not exist)")
			 endif()
		ENDIF()
	ELSE()
		set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O2 ")
	ENDIF()
	
	#Redefine output dir ...
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/gcc/${CMAKE_BUILD_TYPE})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/gcc/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/gcc/${CMAKE_BUILD_TYPE})
	LINK_DIRECTORIES(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})	
	

endif()

MESSAGE ("-- Compiler: " ${COMPILER} )

include_directories (
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include
	${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/freetype2
	${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/libxmp-lite
	${OPEN_GL_INCLUDE_DIRS}  
	${GLU_INCLUDE_DIRS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	MESSAGE ("-- Architecture: 64bit")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    MESSAGE ("-- Architecture: 32bit")
endif()


if ( "${FFMPEG}" STREQUAL "ON" )
	MESSAGE ("-- FFMPEG: ON" )
	set(TARGET_FLAGS " ${TARGET_FLAGS} -D__WITH_FFMPEG__ ")
else()
	MESSAGE ("-- FFMPEG: OFF" )
endif()

MESSAGE ("-- Target : " ${CMAKE_BUILD_TYPE})
MESSAGE ("-- Compiler: " ${COMPILER})


#----------------------------------------------------------------------------------------------------------------------------------------------------
#
# LINK PARAMS
#

LINK_DIRECTORIES (
	${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/lib
)


# 3rd party libraries
set( LIB_DEPS
		jpeg 
		freetype
		xmp-lite 
		SDL2
) 


if ( "${FFMPEG}" STREQUAL "true" )

	set( LIB_DEPS ${LIB_DEPS} avformat avcodec avutil swscale swresample )

endif()


IF ( IS_DEBUG AND EXISTS ${PROJECT_SOURCE_DIR}/../../zetpacker )
	MESSAGE ("-- zetpacker_dev: ON" )
	include_directories (
		${PROJECT_SOURCE_DIR}/../../zetpacker/src
	) 	
	
	file(READ "${PROJECT_SOURCE_DIR}/../../zetpacker/src/zetpacker.h" zetpacker_h)
	
	string(REGEX MATCH "ZETPACKER_VERSION_MAJOR([\t-\ ][0-9])*" _ ${zetpacker_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETPACKER_VERSION_MAJOR)
	
	string(REGEX MATCH "ZETPACKER_VERSION_MINOR([\t-\ ][0-9])*" _ ${zetpacker_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETPACKER_VERSION_MINOR)
	
	string(REGEX MATCH "ZETPACKER_VERSION_PATCH([\t-\ ][0-9])*" _ ${zetpacker_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETPACKER_VERSION_PATCH)
	
	set(ZETPACKER_LIB_NAME "zetpacker-${ZETPACKER_VERSION_MAJOR}-${ZETPACKER_VERSION_MINOR}-${ZETPACKER_VERSION_PATCH}")
	
	set( LIB_DEPS ${LIB_DEPS}  ${PROJECT_SOURCE_DIR}/../../zetpacker/bin/${COMPILER}/${CMAKE_BUILD_TYPE}/lib${ZETPACKER_LIB_NAME}_d.a)
else()
	IF (IS_DEBUG )
		MESSAGE ("-- zetpacker_dev: OFF" )
	ENDIF()
	include_directories (
		${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/zetpacker
	)
	
	set( LIB_DEPS ${LIB_DEPS}   zetpacker )
endif()

IF ( IS_DEBUG AND EXISTS ${PROJECT_SOURCE_DIR}/../../zetnet )
	MESSAGE ("-- zetnet_dev: ON" )
	include_directories (
		${PROJECT_SOURCE_DIR}/../../zetnet/src
	) 	
	
	file(READ "${PROJECT_SOURCE_DIR}/../../zetnet/src/zetnet.h" zetnet_h)
	
	string(REGEX MATCH "ZETNET_VERSION_MAJOR([\t-\ ][0-9])*" _ ${zetnet_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETNET_VERSION_MAJOR)
	
	string(REGEX MATCH "ZETNET_VERSION_MINOR([\t-\ ][0-9])*" _ ${zetnet_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETNET_VERSION_MINOR)
	
	string(REGEX MATCH "ZETNET_VERSION_PATCH([\t-\ ][0-9])*" _ ${zetnet_h})
	string(STRIP "${CMAKE_MATCH_1}" ZETNET_VERSION_PATCH)
	
	set(ZETNET_LIB_NAME "zetnet-${ZETNET_VERSION_MAJOR}-${ZETNET_VERSION_MINOR}-${ZETNET_VERSION_PATCH}")
	
	set( LIB_DEPS ${LIB_DEPS}  ${PROJECT_SOURCE_DIR}/../../zetnet/bin/${COMPILER}/${CMAKE_BUILD_TYPE}/lib${ZETNET_LIB_NAME}_d.a)
else()
	IF (IS_DEBUG )
		MESSAGE ("-- zetnet_dev: OFF" )
	ENDIF()
	include_directories (
		${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/zetnet
	)
	
	set( LIB_DEPS ${LIB_DEPS}   zetnet )
endif()

set(
 LIB_DEPS ${LIB_DEPS} 
 jpeg
 z
 freetype
 xmp-lite
 
)


# 3rd party libraries
if(MSVC)
	  # Add dependency platform libraries
  	set( LIB_DEPS  ${LIB_DEPS} 
		  	opengl32 # OpenGL for windows
		  	Rpcrt4   # Win32 uuid create 
		  	ws2_32	 # Win32 winsock2
			imm32    # Win32 Keyboard
			winmm    # Win32 Audio
			version  # Win32 GetVersion Info
			setupapi # Win32 DiGet class
			SDL2	 # SDL2
			SDL2main # SDL2_main
			Iphlpapi # Win32 network functions
	)
else()
	list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/lib/cmake/SDL2/)
	find_package(PkgConfig REQUIRED)
	find_package(SDL2 REQUIRED)
	

	if(MINGW)
		set(LIB_DEPS ${LIB_DEPS} 
			mingw32
			Rpcrt4	 # Win32 uuid create 
			Opengl32 # OpenGL for windows
			ws2_32	 # Win32 winsock2
			Iphlpapi # Win32 network functions
			
		)
		
		if(NOT IS_DEBUG)
			set(LIB_DEPS ${LIB_DEPS} 
				 -mwindows
			)
		endif()
		
	else()
		set(LIB_DEPS ${LIB_DEPS} 
			GL		# OpenGL
			uuid
		)
	endif()
	
	set(LIB_DEPS ${LIB_DEPS} 
		
	   SDL2main 
	   SDL2
	   ${SDL2_EXTRA_LINK_FLAGS_STATIC}
	)		
endif()

set( LINK_PARAMS "-Wl,--no-as-needed" )
IF(MINGW)
	set( LINK_PARAMS "${LINK_PARAMS},--export-all-symbols" )
ENDIF()



#----------------------------------------------------------------------------------------------------------------------------------------------------
#
# LIBRARY
#


set( ZETGAME_LIB_SRCS ${ZETGAME_LIB_SRCS}

	#-- Base --
	base/@zg_base.c
	
	#-- Graphics -- 
	graphics/@zg_graphics.c
	
	
	#-- Animation --
	animation/@zg_animation.c
	
	#-- Multimedia -- 
	mmedia/@zg_mmedia.c
	
	#-- Math --
	math/@zg_math.c
	
	#-- Input --
	input/@zg_input.c
	
	#-- Scene --
	##scene/@zg_scene.c
	
	#-- Physics --
	physics/@zg_physics.c
	
	#-- Gui --
	gui/@zg_gui.c
	
	#-- Sprite --
	sprite/@zg_sprite.c

	#-- ZG_Tilemap --
	tilemap/@zg_tilemap.c
	
	#-- ZG_Entity Component System --
	#ecs/@zg_ecs.c
	
	#-- Zetgame --
	zetgame.c		
)

add_library( ${ZETGAME_LIB_NAME}${POST_NAME} STATIC ${ZETGAME_LIB_SRCS} )

#-----------------------------------------
#
# EXECUTABLES
#

#Graphics
add_executable(test_primitives2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_primitives2d.c
)

target_link_libraries(test_primitives2d${POST_NAME}     ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

#--- COLLIDER2D
add_executable(test_collider2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_collider2d.c
)

target_link_libraries(test_collider2d${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

add_executable(test_animation${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_animation.c
)

target_link_libraries(test_animation${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

add_executable(test_viewport${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_viewport.c
)

target_link_libraries(test_viewport${POST_NAME}    ${ZETGAME_LIB_NAME}${POST_NAME}  	${LIB_DEPS})

if(false)

add_executable(test_mixer${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_mixer.c
)

target_link_libraries(test_mixer${POST_NAME}   		 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})
	
add_executable(test_2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_2d.c
)

target_link_libraries(test_2d${POST_NAME}    			 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS} )


add_executable(test_viewport${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_viewport.c
)

target_link_libraries(test_viewport${POST_NAME}    ${ZETGAME_LIB_NAME}${POST_NAME}  	${LIB_DEPS})

add_executable(test_animation${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_animation.c
)

target_link_libraries(test_animation${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

#--- SCENEGRAPH
#add_executable(test_scenegraph${POST_NAME} 
#	${PROJECT_SOURCE_DIR}/../test/test_scenegraph.c
#)

#target_link_libraries(test_scenegraph${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

#--- SPRITE
add_executable(test_sprite${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_sprite.c
)

target_link_libraries(test_sprite${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

#--- TILEMAP
add_executable(test_tilemap${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_tilemap.c
)

target_link_libraries(test_tilemap${POST_NAME}     ${ZETGAME_LIB_NAME}${POST_NAME} 	${LIB_DEPS} )

#--- SHADER
add_executable(test_shader${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_shader.c
)

target_link_libraries(test_shader${POST_NAME}     ${ZETGAME_LIB_NAME}${POST_NAME} 	${LIB_DEPS} )

endif()

#--- PARTICLE SYSTEM
#add_executable(test_particle${POST_NAME} 
#	${PROJECT_SOURCE_DIR}/../test/test_particle_system.c
#)


#--- TEXTBOX
add_executable(test_textbox${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_textbox.c
)

target_link_libraries(test_textbox${POST_NAME}     ${ZETGAME_LIB_NAME}${POST_NAME} 	${LIB_DEPS} )


#--- GUI
add_executable(test_gui${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_gui.c
)

target_link_libraries(test_gui${POST_NAME}    	 ${ZETGAME_LIB_NAME}${POST_NAME} ${LIB_DEPS})

