cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW) #remove link dir directories
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


message("dest dir:" ${CMAKE_INSTALL_PREFIX})


if(MSVC)
	MESSAGE ("-- Compiler: MSVC" )
	MESSAGE ("-- Platform: Windows" )
	
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	#disable C4103 warning because is perfectly valid. Only MSVC complains about it
	add_definitions(/wd4103)
	add_definitions(/wd4800)
	add_definitions(/wd4244)

elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")  # g++ / gcc
	SET( COMMON_PARAMS " ${COMMON_PARAMS} -D__STDC_CONSTANT_MACROS  -pthread -fomit-frame-pointer -std=c99 -pipe -Wall -Wstrict-prototypes -Wsign-compare -Wtype-limits ")
	  
	MESSAGE ("-- Compiler: gcc" )
	if (MINGW)
		MESSAGE ("-- Platform: Windows (MinGW)" )
		set(CMAKE_LEGACY_MINGW_WIN32 0)
	else()
		MESSAGE ("-- Platform: Unix" )
		SET( COMMON_PARAMS " ${COMMON_PARAMS} -fPIC " )
	endif() 
	
	set(TARGET_FLAGS " -O2 ")
	set( POST_NAME "")


	IF ( "${TARGET}" STREQUAL "debug")
		
		set( POST_NAME "_d")	
		
		set(TARGET_FLAGS " -O0 ")
		set(TARGET_FLAGS " ${TARGET_FLAGS} -g -D__DEBUG__ ")
	ENDIF ()

	if ( "${MEMMANAGER}" STREQUAL "ON")
		message("-- Memmanager: ON (Warning execution time could be slow)")
		SET( COMMON_PARAMS " ${COMMON_PARAMS} -D__MEMMANAGER__")
		include_directories (
			../memmgr
		)
		
	   	set(ZETGAME_LIB_SRCS ${ZETGAME_LIB_SRCS} ${PROJECT_SOURCE_DIR}/../memmgr/memmgr.c)
	   		
	else()
		MESSAGE ("-- Memmanager: OFF " )
	endif()	
	
endif()


  
include_directories (
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/3rdparty/include
	${PROJECT_SOURCE_DIR}/3rdparty/include/freetype2
	${OPEN_GL_INCLUDE_DIRS}  
	${GLU_INCLUDE_DIRS}
)




if ( "${FFMPEG}" STREQUAL "ON" )
	MESSAGE ("-- FFMPEG: ON" )
	set(TARGET_FLAGS " ${TARGET_FLAGS} -D__WITH_FFMPEG__ ")
else()
	MESSAGE ("-- FFMPEG: OFF" )
endif()

if( "${ZETDEV}" STREQUAL "ON" )
 	MESSAGE ("-- ZetDev: ON" )
 	set(TARGET_FLAGS " ${TARGET_FLAGS} -D__ZETDEV__ ")
	include_directories (
		${PROJECT_SOURCE_DIR}/../zetpacker
		${PROJECT_SOURCE_DIR}/../zetnet
	) 	
else()
	MESSAGE ("-- ZetDev: OFF" )
	include_directories (
		${PROJECT_SOURCE_DIR}/3rdparty/include/zetpacker
		${PROJECT_SOURCE_DIR}/3rdparty/include/zetnet
	)
endif()


# Set C flags.
SET(CMAKE_C_FLAGS   " ${TARGET_FLAGS} ${COMMON_PARAMS}  " )


MESSAGE ("Target : " ${TARGET})

#Redefine output dir ...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET})
LINK_DIRECTORIES(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#----------------------------------------------------------------------------------------------------------------------------------------------------
#
# RESOURCES
#
set(
	embeddFiles 
)

# Find all the input files
FILE(GLOB embeddFiles RELATIVE "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/assets/image/*.gif" 
    "${PROJECT_SOURCE_DIR}/assets/image/*.jpg"
    "${PROJECT_SOURCE_DIR}/assets/image/*.png"
    "${PROJECT_SOURCE_DIR}/assets/audio/*.mod"
    "${PROJECT_SOURCE_DIR}/assets/audio/*.s3m"
    "${PROJECT_SOURCE_DIR}/assets/font/*.ttf"
)
    

FOREACH(embeddFileName ${embeddFiles})
    
    get_filename_component(filename ${embeddFileName} NAME)
    get_filename_component(path ${embeddFileName} PATH)
    MESSAGE(STATUS "Process file: xxd -i ${filename} > ${filename}.c")
    execute_process(COMMAND xxd -i ${filename} 
    				OUTPUT_FILE ${filename}.c
    				WORKING_DIRECTORY ${path}
    )
    

ENDFOREACH(embeddFileName)


configure_files(${PROJECT_SOURCE_DIR}/test/data/images ${PROJECT_SOURCE_DIR}/bin/${TARGET}/data/images)
configure_files(${PROJECT_SOURCE_DIR}/test/data/fonts ${PROJECT_SOURCE_DIR}/bin/${TARGET}/data/fonts)
configure_files(${PROJECT_SOURCE_DIR}/test/data/musics ${PROJECT_SOURCE_DIR}/bin/${TARGET}/data/musics)
configure_files(${PROJECT_SOURCE_DIR}/test/data/samples ${PROJECT_SOURCE_DIR}/bin/${TARGET}/data/samples)


#----------------------------------------------------------------------------------------------------------------------------------------------------
#
# LINK PARAMS
#



LINK_DIRECTORIES (
	${PROJECT_SOURCE_DIR}/3rdparty/lib
)


# 3rd party libraries
set( LIB_DEPS
		jpeg 
		gif 
		freetype 
		xmp
		SDL2
) 


if ( "${FFMPEG}" STREQUAL "true" )

	set( LIB_DEPS ${LIB_DEPS} avformat avcodec avutil swscale swresample )

endif()

if( "${ZETDEV}" STREQUAL "ON" )
	set( LIB_DEPS ${LIB_DEPS} 
		${PROJECT_SOURCE_DIR}/../zetpacker/bin/${TARGET}/libzetpacker${POST_NAME}.a 
		${PROJECT_SOURCE_DIR}/../zetnet/bin/${TARGET}/libzetnet${POST_NAME}.a
	)
	
else()
 	set( LIB_DEPS ${LIB_DEPS} 
 		zetpacker
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

	#-- System --
	system/zg_system.c
	
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
	
	#-- ECS --
	#ecs/zg_ecs.c

	
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

add_executable(test_mixer${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_mixer.c
)

target_link_libraries(test_mixer${POST_NAME}   			${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )
	
add_executable(test_2d${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_2d.c
)

target_link_libraries(test_2d${POST_NAME}    			${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )



#Graphics
add_executable(test_2d_primitives${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_2d_primitives.c
)

target_link_libraries(test_2d_primitives${POST_NAME}    ${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )


add_executable(test_2d_textbox${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_2d_textbox.c
)

target_link_libraries(test_2d_textbox${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

#add_executable(test_2d_scenegraph${POST_NAME} 
#	${PROJECT_SOURCE_DIR}/test/test_2d_scenegraph.c
#)

#target_link_libraries(test_2d_scenegraph${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

add_executable(test_viewport${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_viewport.c
)

target_link_libraries(test_viewport${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

add_executable(test_gui${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_gui.c
)

target_link_libraries(test_gui${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

add_executable(test_animation${POST_NAME} 
	${PROJECT_SOURCE_DIR}/test/test_animation.c
)

target_link_libraries(test_animation${POST_NAME}    	${EXE_DEPS} zetgame${POST_NAME} "-Wl,--no-as-needed" )

