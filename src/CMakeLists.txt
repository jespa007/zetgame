cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)	# To enable MSVC_RUNTIME_LIBRARY. It has effect only when policy CMP0091 is set to NEW prior to the first project() 
project(zetgame)

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# COMMON MACROS
#

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${srcTemplatePath} to ${destDir}/${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# INITIALIZE VARIABLES
#


IF ( "${TARGET}" STREQUAL "")
   SET ( TARGET "release" )
ENDIF ()


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# COMPILE PARAMS
#


message("-- Install drectory: " ${CMAKE_INSTALL_PREFIX})


set( POST_NAME "")
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

elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")  # g++ / gcc

	set( POST_NAME "")
	SET(COMPILER "gcc")

	SET( COMMON_C_PARAMS " ${COMMON_C_PARAMS} -D__STDC_CONSTANT_MACROS  -pthread -fomit-frame-pointer -std=c99 -pipe -Wall -Wextra -pedantic -Wstrict-prototypes -Wsign-compare -Wtype-limits ")
	  
	if (MINGW)
		MESSAGE("-- Plataform: MINGW" )
		message("-- Mingw path: $ENV{MINGWDIR} ")
		SET( COMMON_C_PARAMS " ${COMMON_C_PARAMS} -DEZXML_NOMMAP " )
		set(CMAKE_LEGACY_MINGW_WIN32 0)
	elseif(UNIX)
    	MESSAGE ("-- Plataform: UNIX" )
    else()
    	MESSAGE ("-- Plataform: Unknown" )
	endif()
	

	IF ( "${TARGET}" STREQUAL "debug")
		
		set( POST_NAME "_d")	
		set(COMMON_C_PARAMS " ${COMMON_C_PARAMS} -g -O0 -D__DEBUG__ ")
	
		IF( EXISTS ${PROJECT_SOURCE_DIR}/../../memmgr )
			message("-- Memmanager: ON")
			SET( COMMON_C_PARAMS " ${COMMON_C_PARAMS} -D__MEMMANAGER__")
			include_directories (
				${PROJECT_SOURCE_DIR}/../../memmgr
			)
		
	   		set(ZETGAME_LIB_SRCS ${ZETGAME_LIB_SRCS} ${PROJECT_SOURCE_DIR}/../../memmgr/memmgr.c)
	   	else()
	   		message("-- Memmanager: OFF (../../memmgr not exist)")
	   	endif()
	   		
	else()
		set(TARGET "release")
		set(COMMON_C_PARAMS " ${COMMON_C_PARAMS} -O2 ")
	endif()	
	
	#Redefine output dir ...
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${TARGET})
	LINK_DIRECTORIES(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})	
	
	SET(CMAKE_C_FLAGS   	" ${COMMON_C_PARAMS}  " )
	
endif()


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

MESSAGE ("-- Target : " ${TARGET})
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


IF ( "${TARGET}" STREQUAL "debug" AND EXISTS ${PROJECT_SOURCE_DIR}/../../zetpacker )
	MESSAGE ("-- zetpacker_dev: ON" )
	include_directories (
		${PROJECT_SOURCE_DIR}/../../zetpacker/src
	) 	
	set( LIB_DEPS ${LIB_DEPS} 
		${PROJECT_SOURCE_DIR}/../../zetpacker/build/${COMPILER}/${TARGET}/libzetpacker${POST_NAME}.a
	) 

else()
	IF ( "${TARGET}" STREQUAL "debug" )
		MESSAGE ("-- zetpacker_dev: OFF" )
	ENDIF()
	include_directories (
		${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/zetpacker
	)
	set( LIB_DEPS ${LIB_DEPS} 
 		zetpacker
 	)
endif()

IF ( "${TARGET}" STREQUAL "debug" AND EXISTS ${PROJECT_SOURCE_DIR}/../../zetnet )
	MESSAGE ("-- zetnet_dev: ON" )
	include_directories (
		${PROJECT_SOURCE_DIR}/../../zetnet/src
	) 	
	set( LIB_DEPS ${LIB_DEPS} 
		${PROJECT_SOURCE_DIR}/../../zetnet/build/${COMPILER}/${TARGET}/libzetnet${POST_NAME}.a
	)
else()
	IF ( "${TARGET}" STREQUAL "debug" )
		MESSAGE ("-- zetnet_dev: OFF" )
	ENDIF()
	include_directories (
		${PROJECT_SOURCE_DIR}/../3rdparty/${COMPILER}/include/zetnet
	)
 	set( LIB_DEPS ${LIB_DEPS} 
 		zetnet 
 	)
endif()

set(EXE_DEPS "")

IF (MINGW OR MSVC)
	
	IF(MINGW)
		# Don't change link order to avoid link fails 
		set(EXE_DEPS
			mingw32 
			SDL2main  
			SDL2
		)
		
		
	endif()

  	# Add dependency platform libraries
  	set( LIB_DEPS  ${LIB_DEPS} 
  			zlibstatic
		  	opengl32 
		  	Rpcrt4   # Win32 uuid create 
		  	ws2_32	 # Win32 winsock2
			imm32    # Win32 Keyboard
			winmm    # Win32 Audio
			version  # Win32 GetVersion Info
			setupapi # Win32 DiGet class
			
	)
	
ELSE()

	# Additional libraries for linux? 
	set( LIB_DEPS ${LIB_DEPS} 
		z
		m 
		GL 
		dl
		uuid
	)


ENDIF()

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
	base/zg_base.c
	
	#-- Animation --
	animation/zg_animation.c
	
	#-- Multimedia -- 
	mmedia/zg_mmedia.c
	
	#-- Math --
	math/zg_math.c
	
	#-- Input --
	input/zg_input.c
	
	#-- Graphics -- 
	graphics/zg_graphics.c
	
	#-- Scene --
	scene/zg_scene.c
	
	#-- Physics --
	physics/zg_physics.c
	
	#-- Gui --
	gui/zg_gui.c
	
	#-- Sprite --
	sprite/zg_sprite.c

	#-- Tilemap --
	tilemap/zg_tilemap.c
	
	#-- ECS --
	ecs/zg_ecs.c
	
	#-- Core --
	ZetGame.c		
)

add_library( zetgame${POST_NAME} STATIC ${ZETGAME_LIB_SRCS} )


target_link_libraries(zetgame${POST_NAME}
	${LIB_DEPS}
	${LINK_PARAMS}
)	


#-----------------------------------------
#
# EXECUTABLES
#

IF ( FALSE )

add_executable(test_mixer${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_mixer.c
)

target_link_libraries(test_mixer${POST_NAME}   			${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )
	
add_executable(test_2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_2d.c
)

target_link_libraries(test_2d${POST_NAME}    			${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#Graphics
add_executable(test_primitives${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_primitives.c
)

target_link_libraries(test_primitives${POST_NAME}    ${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )


add_executable(test_viewport${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_viewport.c
)

target_link_libraries(test_viewport${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

add_executable(test_animation${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_animation.c
)

target_link_libraries(test_animation${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

endif()


#--- SCENEGRAPH
add_executable(test_scenegraph${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_scenegraph.c
)

target_link_libraries(test_scenegraph${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- SPRITE
add_executable(test_sprite${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_sprite.c
)

target_link_libraries(test_sprite${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- TILEMAP
add_executable(test_tilemap${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_tilemap.c
)

target_link_libraries(test_tilemap${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- COLLIDER2D
add_executable(test_collider2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_collider2d.c
)

target_link_libraries(test_collider2d${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )


#--- TEXTBOX
add_executable(test_textbox${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_textbox.c
)

target_link_libraries(test_textbox${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- GUI
add_executable(test_gui${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_gui.c
)

target_link_libraries(test_gui${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- SHADER
add_executable(test_shader${POST_NAME} 
	${PROJECT_SOURCE_DIR}/../test/test_shader.c
)

target_link_libraries(test_shader${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#--- PARTICLE SYSTEM
#add_executable(test_particle${POST_NAME} 
#	${PROJECT_SOURCE_DIR}/../test/test_particle_system.c
#)

target_link_libraries(test_gui${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )
