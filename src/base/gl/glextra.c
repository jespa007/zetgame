#include "../gl/glextra.h"
#define OGL_MAX_IDS	4000



static GLushort 	g_enabled_extensions=0;
static GLboolean 	g_gl_extra_initialized=0;

// Vertex Buffer Objects (VBO) ...
PFNGLGENBUFFERSPROC 					glGenBuffers= NULL;
PFNGLBINDBUFFERPROC 					glBindBuffer = NULL;
PFNGLBUFFERSUBDATAPROC 					glBufferSubData= NULL;
PFNGLBUFFERDATAPROC 					glBufferData = NULL;
PFNGLDELETEBUFFERSPROC 					glDeleteBuffers= NULL;


// Vertex Buffer Arrays (VBA) ...
PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays=NULL;
PFNGLBINDVERTEXARRAYPROC 				glBindVertexArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC 		glDisableVertexAttribArray = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC 			glDrawArraysInstanced= NULL;
PFNGLVERTEXATTRIBDIVISORPROC 			glVertexAttribDivisor= NULL;
PFNGLVERTEXATTRIBPOINTERPROC 			glVertexAttribPointer= NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray= NULL;
PFNGLVERTEXATTRIB1FNVPROC 				glVertexAttrib1f=NULL;
PFNGLVERTEXATTRIB1FVNVPROC 				glVertexAttrib1fv=NULL;
PFNGLDELETEVERTEXARRAYSPROC 			glDeleteVertexArrays= NULL;


// Shaders...
PFNGLUSEPROGRAMPROC        				glUseProgram = NULL;
PFNGLGETSHADERINFOLOGPROC 				glGetShaderInfoLog=NULL;
PFNGLGETPROGRAMIVPROC 					glGetProgramiv=NULL;
PFNGLCREATEPROGRAMPROC					glCreateProgram=NULL;
PFNGLCREATESHADERPROC					glCreateShader=NULL;
PFNGLDELETEPROGRAMPROC					glDeleteProgram=NULL;
PFNGLDELETESHADERPROC					glDeleteShader=NULL;
PFNGLATTACHSHADERPROC					glAttachShader=NULL;
PFNGLDETACHSHADERPROC					glDetachShader=NULL;
PFNGLVALIDATEPROGRAMPROC				glValidateProgram=NULL;
PFNGLSHADERSOURCEPROC					glShaderSource=NULL;
PFNGLCOMPILESHADERPROC					glCompileShader=NULL;
PFNGLLINKPROGRAMPROC					glLinkProgram=NULL;
PFNGLGETSHADERIVPROC					glGetShaderiv=NULL;
PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog=NULL;
PFNGLPOINTPARAMETERFVPROC				glPointParameterfv=NULL;
PFNGLPOINTPARAMETERFPROC				glPointParameterf=NULL;
PFNGLGETUNIFORMLOCATIONPROC				glGetUniformLocation=NULL;

// VBO stuff...
PFNGLGETBUFFERPARAMETERIVPROC			glGetBufferParameteriv=NULL;
PFNGLMAPBUFFERPROC						glMapBuffer=NULL;
PFNGLUNMAPBUFFERPROC	 				glUnmapBuffer=NULL;
PFNGLUNIFORM2FPROC		 				glUniform2f=NULL;
PFNGLBUFFERSUBDATAARBPROC 				glBufferSubDataARB = NULL;               // VBO Sub Data Loading Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC 		glGetBufferParameterivARB = NULL; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC 					glMapBufferARB = NULL;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC 				glUnmapBufferARB = NULL; // unmap VBO procedure

 // FrameBuffer functions.
PFNGLGENFRAMEBUFFERSPROC 				glGenFramebuffers=NULL;
PFNGLBINDFRAMEBUFFERPROC 				glBindFramebuffer=NULL;
PFNGLFRAMEBUFFERTEXTUREPROC 			glFramebufferTexture=NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC 			glFramebufferTexture2D=NULL;
PFNGLBLITFRAMEBUFFERPROC 				glBlitFramebuffer=NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC 		glCheckFramebufferStatus=NULL;
PFNGLDELETEFRAMEBUFFERSPROC 			glDeleteFramebuffers = NULL;

//"GL_EXT_framebuffer_object"
PFNGLGENRENDERBUFFERSPROC 				glGenRenderbuffers=NULL;
PFNGLBINDRENDERBUFFERPROC 				glBindRenderbuffer=NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample=NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer=NULL;
PFNGLRENDERBUFFERSTORAGEPROC 			glRenderbufferStorage = NULL;
PFNGLDELETERENDERBUFFERSPROC 			glDeleteRenderbuffers = NULL;

// compatibily with mingw
#ifdef __WIN32

PFNGLACTIVETEXTUREPROC 					glActiveTexture = NULL;


void *WIN32_glGetProcAddress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }
  return p;
}

#endif



uintptr_t glGetProcAddress(const char *name){

	uintptr_t result=0;
	#if defined(__MINGW32__)
		result=(uintptr_t)WIN32_glGetProcAddress(name);
	#elif defined(__unix__)
	 	result=(uintptr_t)glXGetProcAddressARB((const GLubyte *)name);
	#else
	#error "No GL platform defined"
	#endif

	return result;
}

//---------------------
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance
void glPerspective(GLdouble fovY, GLdouble aspect, GLdouble z_near, GLdouble z_far ) {
	const GLdouble pi = 3.1415926535897932384626433832795;

	GLdouble xmin, xmax, ymin,ymax;

	ymax = tan( fovY* pi / 360  ) * z_near;
	ymin = -ymax;

	xmin = ymin*aspect;
	xmax = ymax*aspect;

	glFrustum(xmin, xmax, ymin, ymax, z_near, z_far );
}


GLboolean glIsExtensionAvailable(const char *extension)
{
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
	return GL_FALSE;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
	 OpenGL extensions std::string. Don't be fooled by sub-strings,
	 etc. */
  start = extensions;

  if(start == NULL)
  {
	fprintf(stderr,"Cannot get extensions OpenGL. Maybe opengl context is not initialized!");
	return GL_FALSE;
  }

  for (;;) {
	where = (GLubyte *) strstr((const char *) start, extension);
	if (!where)
	  break;
	terminator = where + strlen(extension);
	if (where == start || *(where - 1) == ' ')
	  if (*terminator == ' ' || *terminator == '\0')
		return GL_TRUE;
	start = terminator;
  }
  return GL_FALSE;
}

GLushort glGetEnabledExtensions(){
	return g_enabled_extensions;
}

void glExtraIni(){


	if(!g_gl_extra_initialized){
		g_gl_extra_initialized=GL_TRUE;


// compatibily with mingw
#ifdef __MINGW32__
		glActiveTexture = (PFNGLACTIVETEXTUREPROC) glGetProcAddress("glActiveTexture");

		if(glActiveTexture == NULL) {
			fprintf(stderr,"Fatal error. Cannot get glActiveTexture function!");
			exit(-1);
		}
#endif

		// Check For VBOs Supported
		if( glIsExtensionAvailable( "GL_ARB_vertex_buffer_object" ) )
		{
			// Get Pointers To The GL Functions
			glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)glGetProcAddress("glGetBufferParameteriv");
			glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glGetProcAddress("glDisableVertexAttribArray");
			glBindBuffer = (PFNGLBINDBUFFERPROC) glGetProcAddress("glBindBuffer");
			glGenBuffers = (PFNGLGENBUFFERSPROC) glGetProcAddress("glGenBuffers");
			glBufferData = (PFNGLBUFFERDATAPROC) glGetProcAddress("glBufferData");
			glBufferSubData = (PFNGLBUFFERSUBDATAPROC) glGetProcAddress("glBufferSubData");
			glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) glGetProcAddress("glDeleteBuffers");
			glGenVertexArrays =(PFNGLGENVERTEXARRAYSPROC)glGetProcAddress("glGenVertexArrays");
			glBindVertexArray =(PFNGLBINDVERTEXARRAYPROC)glGetProcAddress("glBindVertexArray");
			glEnableVertexAttribArray= (PFNGLENABLEVERTEXATTRIBARRAYPROC	)glGetProcAddress("glEnableVertexAttribArray");
			glVertexAttribPointer= (PFNGLVERTEXATTRIBPOINTERPROC 	)glGetProcAddress("glVertexAttribPointer");
			glDeleteVertexArrays=(PFNGLDELETEVERTEXARRAYSPROC)glGetProcAddress("glDeleteVertexArrays");

			if(glGetBufferParameteriv
					&& glDisableVertexAttribArray
					&& glBindBuffer
					&& glGenBuffers
					&& glBufferData
					&& glBufferSubData
					&& glDeleteBuffers
					&& glGenVertexArrays
					&& glBindVertexArray
					&& glEnableVertexAttribArray
					&& glVertexAttribPointer
					&& glDeleteVertexArrays){
				g_enabled_extensions|=VERTEX_BUFFER_OBJECT_EXT;
			}

			// Load Vertex Data Into The Graphics Card Memory
		}


		// check PBO is supported by your video card
		if(glIsExtensionAvailable("GL_ARB_pixel_buffer_object"))
		{
			// get pointers to GL functions
			glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)glGetProcAddress("glBufferSubDataARB");
			glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)glGetProcAddress("glGetBufferParameterivARB");
			glMapBufferARB = (PFNGLMAPBUFFERARBPROC)glGetProcAddress("glMapBufferARB");
			glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)glGetProcAddress("glUnmapBufferARB");

			// check once again PBO extension
			if( glBufferSubDataARB
				&& glMapBufferARB
				&& glUnmapBufferARB
				&& glGetBufferParameterivARB)
			{
				g_enabled_extensions|=PIXEL_BUFFER_OBJECT_EXT;
			}
		}

		if(glIsExtensionAvailable("GL_ARB_framebuffer_object"))
		{
			glGenFramebuffers= (PFNGLGENFRAMEBUFFERSPROC)glGetProcAddress("glGenFramebuffers");
			glBindFramebuffer= (PFNGLBINDFRAMEBUFFERPROC)glGetProcAddress("glBindFramebuffer");
			glFramebufferTexture= (PFNGLFRAMEBUFFERTEXTUREPROC)glGetProcAddress("glFramebufferTexture");
			glFramebufferTexture2D= (PFNGLFRAMEBUFFERTEXTURE2DPROC)glGetProcAddress("glFramebufferTexture2D");
			glBlitFramebuffer= (PFNGLBLITFRAMEBUFFERPROC)glGetProcAddress("glBlitFramebuffer");
			glCheckFramebufferStatus= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glGetProcAddress("glCheckFramebufferStatus");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glGetProcAddress("glDeleteFramebuffers");;

			if(glGenFramebuffers
			   && glBindFramebuffer
			   && glFramebufferTexture
			   && glFramebufferTexture2D
			   && glCheckFramebufferStatus
			   && glDeleteFramebuffers
			   &&glBlitFramebuffer){
				g_enabled_extensions|=FRAMEBUFFER_OBJECT_EXT;
			}
		}


		//"GL_EXT_framebuffer_object"
		if(glIsExtensionAvailable("GL_EXT_framebuffer_object"))
		{
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glGetProcAddress("glGenRenderbuffers");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glGetProcAddress("glBindRenderbuffer");
			glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)glGetProcAddress("glRenderbufferStorageMultisample");
			glFramebufferRenderbuffer=(PFNGLFRAMEBUFFERRENDERBUFFERPROC)glGetProcAddress("glFramebufferRenderbuffer");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) glGetProcAddress ("glRenderbufferStorage");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glGetProcAddress("glDeleteRenderbuffers");



			if(	glGenRenderbuffers
				&& glBindRenderbuffer
				&& glRenderbufferStorageMultisample
				&& glFramebufferRenderbuffer
				&& glRenderbufferStorage
				&& glDeleteRenderbuffers
			){
				g_enabled_extensions|=RENDERBUFFER_OBJECT_EXT;
			}
		}

		if(
				glIsExtensionAvailable("GL_ARB_shading_language_100")
				&& glIsExtensionAvailable("GL_ARB_vertex_shader")
				&& glIsExtensionAvailable("GL_ARB_fragment_shader") )
		{
			glUseProgram = (PFNGLUSEPROGRAMPROC)glGetProcAddress("glUseProgramObjectARB");
			glGetShaderInfoLog= (PFNGLGETSHADERINFOLOGPROC)glGetProcAddress("glGetShaderInfoLog");
			glGetProgramiv= (PFNGLGETPROGRAMIVPROC)glGetProcAddress("glGetProgramiv");

			glCreateProgram= (PFNGLCREATEPROGRAMPROC)glGetProcAddress("glCreateProgram");
			glCreateShader=(PFNGLCREATESHADERPROC)glGetProcAddress("glCreateShader");
			glDeleteProgram=(PFNGLDELETEPROGRAMPROC)glGetProcAddress("glDeleteProgram");
			glDeleteShader=(PFNGLDELETESHADERPROC)glGetProcAddress("glDeleteShader");

			glAttachShader =(PFNGLATTACHSHADERPROC)glGetProcAddress("glAttachShader");
			glDetachShader=(PFNGLDETACHSHADERPROC)glGetProcAddress("glDetachShader");
			glValidateProgram   =(PFNGLVALIDATEPROGRAMPROC)glGetProcAddress("glValidateProgram");
			glShaderSource  =(PFNGLSHADERSOURCEPROC)glGetProcAddress("glShaderSource");
			glCompileShader  =(PFNGLCOMPILESHADERPROC)glGetProcAddress("glCompileShader");
			glLinkProgram  =(PFNGLLINKPROGRAMPROC)glGetProcAddress("glLinkProgram");
			glGetShaderiv  =(PFNGLGETSHADERIVPROC)glGetProcAddress("glGetShaderiv");
			glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC)glGetProcAddress("glGetProgramInfoLog");

			glPointParameterfv=(PFNGLPOINTPARAMETERFVPROC)glGetProcAddress("glPointParameterfv");
			glPointParameterf=(PFNGLPOINTPARAMETERFPROC)glGetProcAddress("glPointParameterf");
			glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)glGetProcAddress("glGetUniformLocation");

			glVertexAttrib1f= (PFNGLVERTEXATTRIB1FNVPROC)glGetProcAddress("glVertexAttrib1f");
			glVertexAttrib1fv= (PFNGLVERTEXATTRIB1FVNVPROC)glGetProcAddress("glVertexAttrib1fv");
			glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)glGetProcAddress("glVertexAttribPointer");
			glUniform2f=(PFNGLUNIFORM2FPROC)glGetProcAddress("glUniform2f");

			if(
				glUseProgram
				&& glGetShaderInfoLog
				&& glGetProgramiv
				&& glCreateProgram
				&& glCreateShader
				&& glDeleteProgram
				&& glDeleteShader
				&& glAttachShader
				&& glValidateProgram
				&& glShaderSource
				&& glCompileShader
				&& glLinkProgram
				&& glGetShaderiv
				&& glGetProgramInfoLog
				&& glPointParameterfv
				&& glPointParameterf
				&& glBufferSubData
				&& glGetUniformLocation
				&& glVertexAttrib1fv
				&& glVertexAttrib1f
				&& glUniform2f
			){

			  g_enabled_extensions|=SHADING_LANGUAGE_EXT;
			}
		}

		g_gl_extra_initialized=GL_TRUE;
	}
}


GLuint glGenTexture2D(GLushort width, GLushort height, GLint internalFormat, GLenum format, GLenum type)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	//NOTE: You should use GL_NEAREST here. Other values can cause problems
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (GL_DEPTH_COMPONENT == format) {
		//sample like regular texture, value is in all keyframe_tracks
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

GLboolean glGenFrameBufferExt(FBOInfo * fboInfo, GLushort width, GLushort height)
{
	GLuint result = GL_FRAMEBUFFER_UNDEFINED;


	if(FRAMEBUFFER_OBJECT_EXTENSION_ENABLED){

		//generate textures for FBO usage. You could use other formats here, e.g. GL_RGBA8 for color
		fboInfo->color = glGenTexture2D(width, height, GL_RGBA16F, GL_RGB, GL_FLOAT);
		fboInfo->depth = glGenTexture2D(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
		//generate and bind FBO
		glGenFramebuffers(1, &fboInfo->id);
		glBindFramebuffer(GL_FRAMEBUFFER, fboInfo->id);
		//bind color and depth texture to FBO you could also use glFrameBufferTexture2D with GL_TEXTURE_2D
		//glFrameBufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboInfo.color, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fboInfo->depth, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,fboInfo->color, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,fboInfo->depth, 0);
		//check if FBO was created ok
		if (((result = glCheckFramebufferStatus(GL_FRAMEBUFFER))==GL_FRAMEBUFFER_COMPLETE)) {
			fprintf(stderr,"FBO %d set up successfully!", fboInfo->id);
		}
		else {
			fprintf(stderr,"FBO %d NOT set up properly! (result:0x%X)", fboInfo->id,result);
		}
		//unbind FBO for now
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	return result==GL_FRAMEBUFFER_COMPLETE;
}


void glDeleteFrameBufferExt(FBOInfo * fboInfo)
{
	glDeleteFramebuffers(1,&fboInfo->id);
	glDeleteTextures(1,&fboInfo->color);
	glDeleteTextures(1,&fboInfo->depth);

}


#include 	"../gl/gldebug.c"
