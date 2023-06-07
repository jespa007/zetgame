#ifndef __GL_EXTRA_H_
#define __GL_EXTRA_H_

#include 	<stdlib.h>
#include 	<stdio.h>
#include 	<string.h>
#include 	<math.h>

#include 	<GL/gl.h>
#include 	<GL/glext.h>

#ifdef __unix__
#include 	<GL/glx.h>
#endif

#include 	"../gl/gldebug.h"


#define		VERTEX_BUFFER_OBJECT_EXT	(0x1<<0)
#define		PIXEL_BUFFER_OBJECT_EXT		(0x1<<1)
#define		FRAMEBUFFER_OBJECT_EXT		(0x1<<2)
#define		RENDERBUFFER_OBJECT_EXT		(0x1<<3)
#define		SHADING_LANGUAGE_EXT		(0x1<<4)



#define VERTEX_BUFFER_OBJECT_EXTENSION_ENABLED 	(glGetEnabledExtensions()&VERTEX_BUFFER_OBJECT_EXT)
#define FRAMEBUFFER_OBJECT_EXTENSION_ENABLED 	(glGetEnabledExtensions()&FRAMEBUFFER_OBJECT_EXT)

typedef struct {
	GLuint id;
	GLuint color;
	GLuint depth;
	GLuint rbo;
} ZG_FBOInfo;

// Vertex arrays ...
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC 		glDisableVertexAttribArray;
extern PFNGLBINDBUFFERPROC 						glBindBuffer;
extern PFNGLGENBUFFERSPROC 						glGenBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC 				glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC 					glDeleteBuffers;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC			glEnableVertexAttribArray;
extern PFNGLBUFFERSUBDATAPROC 					glBufferSubData;
extern PFNGLBUFFERDATAPROC 						glBufferData;
extern PFNGLBINDVERTEXARRAYPROC 				glBindVertexArray;
extern PFNGLGENVERTEXARRAYSPROC					glGenVertexArrays;
extern PFNGLVERTEXATTRIBPOINTERPROC				glVertexAttribPointer;

// Shaders...
extern PFNGLUSEPROGRAMPROC        				glUseProgram;
extern PFNGLGETSHADERINFOLOGPROC 				glGetShaderInfoLog;
extern PFNGLGETPROGRAMIVPROC 					glGetProgramiv;
extern PFNGLCREATEPROGRAMPROC					glCreateProgram;
extern PFNGLCREATESHADERPROC					glCreateShader;
extern PFNGLDELETEPROGRAMPROC					glDeleteProgram;
extern PFNGLDELETESHADERPROC					glDeleteShader;
extern PFNGLATTACHSHADERPROC					glAttachShader;
extern PFNGLDETACHSHADERPROC					glDetachShader;
extern PFNGLVALIDATEPROGRAMPROC					glValidateProgram;
extern PFNGLSHADERSOURCEPROC					glShaderSource;
extern PFNGLCOMPILESHADERPROC					glCompileShader;
extern PFNGLLINKPROGRAMPROC						glLinkProgram;
extern PFNGLGETSHADERIVPROC						glGetShaderiv;
extern PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog;
extern PFNGLPOINTPARAMETERFVPROC				glPointParameterfv;
extern PFNGLPOINTPARAMETERFPROC					glPointParameterf;
extern PFNGLGETUNIFORMLOCATIONPROC				glGetUniformLocation;

// VBO stuff...
extern PFNGLGETBUFFERPARAMETERIVPROC			glGetBufferParameteriv;
extern PFNGLMAPBUFFERPROC						glMapBuffer;
extern PFNGLUNMAPBUFFERPROC	 					glUnmapBuffer;
extern PFNGLUNIFORM2FPROC		 				glUniform2f;
extern PFNGLBUFFERSUBDATAARBPROC 				glBufferSubDataARB;               // VBO Sub Data Loading Procedure
extern PFNGLGETBUFFERPARAMETERIVARBPROC 		glGetBufferParameterivARB; // return various parameters of VBO
extern PFNGLMAPBUFFERARBPROC 					glMapBufferARB;                       // map VBO procedure
extern PFNGLUNMAPBUFFERARBPROC 					glUnmapBufferARB; // unmap VBO procedure

 // FrameBuffer functions.
extern PFNGLGENFRAMEBUFFERSPROC 				glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC 				glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC 				glFramebufferTexture;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC 			glFramebufferTexture2D;
extern PFNGLBLITFRAMEBUFFERPROC 				glBlitFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC 			glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC 				glDeleteFramebuffers;

//"GL_EXT_framebuffer_object"
extern PFNGLGENRENDERBUFFERSPROC 				glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC 				glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC 	glRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC			glFramebufferRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC 			glRenderbufferStorage;
extern PFNGLDELETERENDERBUFFERSPROC 			glDeleteRenderbuffers;

// compatibily with mingw
#ifdef __MINGW32__
extern PFNGLACTIVETEXTUREPROC					glActiveTexture;
#endif


#ifdef __cplusplus
extern "C" {
#endif

// custom gl functions
void 		glPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
GLushort	glGetEnabledExtensions(void);

void 		glExtraIni(void);
GLuint		glGenTexture2D(GLushort width, GLushort height, GLint internalFormat, GLenum format, GLenum type);
GLboolean	glGenFrameBufferExt(ZG_FBOInfo * fboInfo, GLushort width, GLushort height);
void		glDeleteFrameBufferExt(ZG_FBOInfo * fboInfo);

#ifdef __cplusplus
}
#endif



#endif

