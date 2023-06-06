#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "../gl/glextra.h"

#define GL_DEBUG_MAX_MEM_IDS	4000


#undef glFrustum
#undef glOrtho
#undef glCullFace
#undef glFrontFace
#undef glEnable
#undef glDisable
#undef glBegin
#undef glEnd
#undef glPushAttrib
#undef glPopAttrib
#undef glPushClientAttrib
#undef glPopClientAttrib
#undef glShadeModel
#undef glHint
#undef glDepthMask
#undef glGetFloatv
#undef glDepthFunc
#undef glAlphaFunc
#undef glEnableClientState
#undef glDisableClientState
#undef glReadPixels


// transform matrix
#undef glLoadIdentity
#undef glMatrixMode
#undef glPushMatrix
#undef glPopMatrix
#undef glMultMatrixf

#undef glTranslatef
#undef glTranslated
#undef glRotatef
#undef glRotated
#undef glScalef
#undef glScaled


// list
#undef glNewList
#undef glGenLists
#undef glEndList


//texture
#undef glGenTextures
#undef glBindTexture
#undef glTexParameteri
#undef glTexParameteriv
#undef glTexParameterf
#undef glTexParameterfv

#undef glTexImage1D
#undef glTexImage2D
#undef glTexImage3D
#undef glBlendFunc
#undef glActiveTexture
#undef glDeleteTextures


//pixel
#undef glPixelStorei

// point
#undef glPointSize
#undef glPointParameteri
#undef glPointParameteriv
#undef glPointParameterf
#undef glPointParameterfv


// buffer
#undef	glGenBuffers
#undef	glBindBuffer
#undef	glBufferData
#undef	glNamedBufferData
#undef	glDeleteBuffers

// framebuffer
#undef glGenFramebuffers
#undef glBindFramebuffer
#undef glFramebufferTexture
#undef glFramebufferTexture1D
#undef glFramebufferTexture2D
#undef glFramebufferTexture3D


#undef glNamedFramebufferTexture

#undef glCheckFramebufferStatus
#undef glCheckNamedFramebufferStatus

#undef glDeleteFramebuffers


// vertex array
#undef glGenVertexArrays
#undef glEnableVertexAttribArray
#undef glDisableVertexAttribArray
#undef glBindVertexArray
#undef glDeleteVertexArrays
#undef glVertexAttribPointer
#undef glDrawElements

// program
#undef	glCreateProgram
#undef glUseProgram
#undef glGetUniformLocation
#undef glLinkProgram
#undef	glGetProgramiv
#undef	glValidateProgram
#undef	glGetProgramInfoLog
#undef glDeleteProgram

// shader
#undef	glCreateShader
#undef glAttachShader
#undef glDetachShader
#undef glCompileShader
#undef glShaderSource
#undef glGetShaderiv
#undef glGetShaderInfoLog
#undef	glDeleteShader




typedef struct{
	char 		filename[50];
	int 		line;
	unsigned	method;
	GLuint 		*pointer_id;
	GLuint 		id;
	GLboolean	is_reserved;
}LoadedIdOGL;

static LoadedIdOGL g_loaded_id[GL_DEBUG_MAX_MEM_IDS];
//static GLint g_reserved_slot_id[GL_DEBUG_MAX_MEM_IDS]={0}; // To accelerate find free slot. This apoints a free slot (id must be initialized as GL_INVALID_VALUE)
static GLint g_n_loaded_id=0;

static GLboolean g_gldebug_initialized=GL_FALSE;

void glPrintError(const char *file, int line)
{
	GLenum error=glGetError();
	switch(error)
	{
	case GL_NO_ERROR:
		break;
	default:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] Unknow error (%i)",error);
		break;
	case GL_INVALID_ENUM:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n",file,line);
		break;
	case GL_INVALID_VALUE:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n",file,line);
		break;

	case GL_INVALID_OPERATION:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n",file,line);
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n",file,line);
		break;
	case GL_OUT_OF_MEMORY:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n",file,line);
		break;
	case GL_STACK_UNDERFLOW:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] An attempt has been made to perform an operation that would cause an internal stack to underflow.\n",file,line);
		break;
	case GL_STACK_OVERFLOW:
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_ERROR,"[OGL_ERROR] An attempt has been made to perform an operation that would cause an internal stack to overflow.\n",file,line);
		break;
	}
}

void 	__glFrustum__(const char *file, int line,GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble nearVal,GLdouble farVal){
	glFrustum(left,right,bottom,top,nearVal,farVal);
	glPrintError(file,line);
}

void 	__glOrtho__(const char *file, int line,GLdouble left,  GLdouble right,  GLdouble bottom,  GLdouble top,  GLdouble nearVal,  GLdouble farVal){
	glOrtho(left,right,bottom,top,nearVal,farVal);
	glPrintError(file,line);
}

void 	__glEnable__(const char *file, int line,	GLenum  	cap){
	glEnable(cap);
	glPrintError(file,line);
}

void 	__glDisable__(const char *file, int line,	GLenum  	cap){
	glDisable(cap);
	glPrintError(file,line);
}

void 	__glGetFloatv__(const char *file, int line,GLenum pname,	GLfloat * params){
	glGetFloatv(pname,params);
	glPrintError(file,line);
}

void 	__glBegin__(const char *file, int line,	GLenum mode){
	ZG_UNUSUED_PARAM(file);
	ZG_UNUSUED_PARAM(line);
	glBegin(mode);
	// we do not call glPrintError due is not allowed while we are defining drawing points (should do it after glEnd)
}

void 	__glEnd__(const char *file, int line){
	glEnd();
	glPrintError(file,line);
}

void 	__glEnableClientState__(const char *file, int line,GLenum cap){
	glEnableClientState(cap);
	glPrintError(file,line);
}

void __glDrawElements__(const char *file, int line, GLenum _mode,GLsizei _index_length,GLenum  _type, const void *_ptr){
	glDrawElements(_mode,_index_length,  _type, (void *)_ptr);
	glPrintError(file,line);
}

void 	__glDisableClientState__(const char *file, int line,GLenum cap){
	glDisableClientState(cap);
	glPrintError(file,line);
}

void 	__glShadeModel__(const char *file, int line,	GLenum mode){
	glShadeModel(mode);
	glPrintError(file,line);
}

void 	__glHint__(	const char *file, int line,GLenum target, 	GLenum mode){
	glHint(target,mode);
	glPrintError(file,line);
}

void 	__glDepthMask__(const char *file, int line,	GLboolean flag){
	glDepthMask(flag);
	glPrintError(file,line);
}

void 	__glDepthFunc__(const char *file, int line,GLenum func){
	glDepthFunc(func);
	glPrintError(file,line);
}

void 	__glClearDepth__(const char *file, int line,GLdouble depth){
	glClearDepth(depth);
	glPrintError(file,line);
}

void 	__glAlphaFunc__(const char *file, int line,GLenum func,  GLclampf ref){
	glAlphaFunc(func,ref);
	glPrintError(file,line);
}

void __glReadPixels__(const char *file,int line, GLint x,GLint y,GLsizei width,	GLsizei height,	GLenum format,	GLenum type,	GLvoid * data){
	glReadPixels(x,y,width,height,format,type,data);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// transform
void __glTranslatef__(const char *file, int line, float x, float y, float z){
	glTranslatef(x,y,z);
	glPrintError(file,line);
}

void __glRotatef__(const char *file, int line,float angle,float x, float y, float z){
	glRotatef(angle,x,y,z);
	glPrintError(file,line);
}

void __glScalef__(const char *file, int line,float x, float y, float z){
	glScalef(x,y,z);
	glPrintError(file,line);
}

void 	__glLoadIdentity__(const char *file, int line){
	glLoadIdentity();
	glPrintError(file,line);
}

void __glMultMatrixf__(const char *file, int line,float *matrix4x4){
	glMultMatrixf(matrix4x4);
	glPrintError(file,line);
}

void 	__glPushMatrix__(const char *file, int line){
	glPushMatrix();
	glPrintError(file,line);
}

void 	__glPopMatrix__(const char *file, int line){
	glPopMatrix();
	glPrintError(file,line);
}

void 	__glMatrixMode__(const char *file, int line,GLenum mode){
	glMatrixMode(mode);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// lists
void 	__glNewList__(const char *file, int line,	GLuint list, GLenum mode){
	glNewList(list,mode);
	glPrintError(file,line);
}

GLuint 	__glGenLists__(const char *file, int line,GLsizei range){
	GLuint r=glGenLists(range);
	glPrintError(file,line);
	return r;
}
void 	__glEndList__(const char *file, int line){
	glEndList();
	glPrintError(file,line);
}

void 	__glPushAttrib__(const char *file, int line,GLbitfield mask){
	glPushAttrib(mask);
	glPrintError(file,line);
}

void 	__glPopAttrib__(const char *file, int line){
	glPopAttrib();
	glPrintError(file,line);
}

void 	__glPushClientAttrib__(const char *file, int line,GLbitfield mask){
	glPushClientAttrib(mask);
	glPrintError(file,line);
}

void 	__glPopClientAttrib__(const char *file, int line){
	glPopClientAttrib();
	glPrintError(file,line);
}

void 	__glCullFace__(	const char *file, int line,GLenum mode){
	glCullFace(mode);
	glPrintError(file,line);
}

void 	__glFrontFace__(	const char *file, int line,GLenum mode){
	glFrontFace(mode);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// point
void 	__glPixelStorei__(const char *file, int line,	GLenum pname, 	GLint param){
	glPixelStorei(pname,param);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// ZG_Texture
void 	__glGenTextures__(const char *file, int line,	GLsizei n, 	GLuint * textures){
	glGenTextures(n,textures);
	glPrintError(file,line);
}

void 	__glBindTexture__(const char *file, int line,GLenum target,	GLuint texture){
	glBindTexture(target,texture);
	glPrintError(file,line);
}

void 	__glTexParameteri__(const char *file, int line,GLenum target, GLenum pname,GLint param){
	glTexParameteri(target,pname,param);
	glPrintError(file,line);
}

void 	__glTexParameterf__(const char *file, int line,GLenum target, GLenum pname,GLfloat param){
	glTexParameterf(target,pname,param);
	glPrintError(file,line);
}

void 	__glTexImage2D__(const char *file, int line,	GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,const GLvoid * pixels){
	glTexImage2D(target,level,internalFormat,width,height,border, format, type, pixels);
	glPrintError(file,line);
}

void 	__glActiveTexture__(const char *file, int line,	GLenum texture){
	glActiveTexture(texture);
	glPrintError(file,line);
}

void 	__glBlendFunc__(	const char *file, int line,GLenum sfactor, 	GLenum dfactor){
	glBlendFunc(sfactor,dfactor);
	glPrintError(file,line);
}

void 	__glDeleteTextures__(const char *file, int line,	GLsizei n, const GLuint * textures){
	glDeleteTextures(n,textures);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// buffers
void 	__glGenBuffers__( const char *file, int line,	GLsizei n,	GLuint * buffers){
	glGenBuffers(n,buffers);
	glPrintError(file,line);
}

void 	__glBindBuffer__(const char *file, int line,	GLenum target,GLuint buffer){
	glBindBuffer(target,buffer);
	glPrintError(file,line);
}

void 	__glBufferData__(const char *file, int line,	GLenum target, GLsizeiptr size,	const GLvoid * data, GLenum usage){
	glBufferData(target,size,data,usage);
	glPrintError(file,line);
}

void 	__glDeleteBuffers__( const char *file, int line,	GLsizei n, 	const GLuint * buffers){
	glDeleteBuffers(n,buffers);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// framebuffers
void 	__glGenFramebuffers__(const char *file, int line,	GLsizei n,	GLuint *ids){
	glGenFramebuffers(n,ids);
	glPrintError(file,line);
}
void 	__glBindFramebuffer__(const char *file, int line,GLenum target, GLuint framebuffer){
	glBindFramebuffer(target,framebuffer);
	glPrintError(file,line);
}

void 	__glFramebufferTexture__(const char *file, int line,	GLenum target, 	GLenum attachment,	GLuint texture,	GLint level){
	glFramebufferTexture(target,attachment,texture,level);
	glPrintError(file,line);
}

void 	__glFramebufferTexture2D__(const char *file, int line,	GLenum target,	GLenum attachment, 	GLenum textarget,	GLuint texture,	GLint level){
	glFramebufferTexture2D(target,attachment, textarget,texture,level);
	glPrintError(file,line);
}

GLenum	__glCheckFramebufferStatus__(const char *file, int line,	GLenum target){
	GLenum r=glCheckFramebufferStatus(target);
	glPrintError(file,line);
	return r;
}
void 	__glDeleteFramebuffers__(const char *file, int line,GLsizei n,  const GLuint * framebuffers){
	glDeleteFramebuffers(n,framebuffers);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// point
void 	__glPointSize__(const char *file, int line,GLfloat size){
	glPointSize(size);
	glPrintError(file,line);
}

void 	__glPointParameterf__(const char *file, int line,	GLenum pname,	GLfloat param){
	glPointParameterf(pname,param);
	glPrintError(file,line);
}

void 	__glPointParameterfv__(const char *file, int line,GLenum pname,	const GLfloat * params){
	glPointParameterfv(pname,params);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// vertexs
void 	__glGenVertexArrays__(const char *file, int line, GLsizei n,	GLuint *arrays){
	glGenVertexArrays(n,arrays);
	glPrintError(file,line);
}

void 	__glBindVertexArray__(const char *file, int line, 	GLuint array){
	glBindVertexArray(array);
	glPrintError(file,line);
}

void 	__glVertexAttribPointer__(const char *file, int line, 	GLuint index,	GLint size,	GLenum type,GLboolean normalized,GLsizei stride,const GLvoid * pointer){
	glVertexAttribPointer(index,	size,	type,normalized,stride,pointer);
	glPrintError(file,line);
}

void 	__glEnableVertexAttribArray__(const char *file, int line, 	GLuint index){
	glEnableVertexAttribArray(index);
	glPrintError(file,line);
}

void 	__glDisableVertexAttribArray__(const char *file, int line, 	GLuint index){
	glDisableVertexAttribArray(index);
	glPrintError(file,line);
}

void 	__glDeleteVertexArrays__(const char *file,	int line,GLsizei n,	const GLuint *arrays){
	glDeleteVertexArrays(n,arrays);
	glPrintError(file,line);
}

//------------------------------------------------------------------------------------------------------------------------------
// programs
GLuint 	__glCreateProgram__(const char *file, int line){
	GLuint r=glCreateProgram();
	glPrintError(file,line);
	return r;
}

void 	__glLinkProgram__(const char *file, int line, 	GLuint program){
	glLinkProgram(program);
	glPrintError(file,line);
}

void 	__glValidateProgram__( const char *file, int line,GLuint program){
	glValidateProgram(program);
	glPrintError(file,line);
}

void 	__glUseProgram__(const char *file, int line,	GLuint program){
	glUseProgram(program);
	glPrintError(file,line);
}

GLint 	__glGetUniformLocation__(const char *file, int line,	GLuint program, 	const GLchar *name){
	GLint r=glGetUniformLocation(program,name);
	glPrintError(file,line);
	return r;
}

void 	__glGetProgramiv__(const char *file, int line,GLuint program,	GLenum pname, 	GLint *params){
	glGetProgramiv(program,pname,params);
	glPrintError(file,line);
}

void 	__glGetProgramInfoLog__(const char *file, int line,GLuint program,GLsizei maxLength,GLsizei *length,GLchar *infoLog){
	glGetProgramInfoLog(program,maxLength,length,infoLog);
	glPrintError(file,line);
}

void 	__glDeleteProgram__(const char *file, int line,	GLuint program){
	glDeleteProgram(program);
	glPrintError(file,line);
}
//------------------------------------------------------------------------------------------------------------------------------
// shaders
GLuint 	__glCreateShader__(const char *file, int line,GLenum shaderType){
	GLuint r=glCreateShader(shaderType);
	glPrintError(file,line);
	return r;
}

void 	__glCompileShader__(const char *file, int line,GLuint shader){
	glCompileShader(shader);
	glPrintError(file,line);
}

void 	__glShaderSource__( const char *file, int line,	GLuint shader,	GLsizei count,	const GLchar **string, 	const GLint *length){
	glShaderSource(shader,count,string,length);
	glPrintError(file,line);
}

void 	__glAttachShader__(const char *file, int line,GLuint program,	GLuint shader){
	glAttachShader(program,shader);
	glPrintError(file,line);
}
void 	__glDetachShader__(const char *file, int line,GLuint program,	GLuint shader){
	glDetachShader(program,shader);
	glPrintError(file,line);
}
void 	__glGetShaderiv__(	const char *file, int line,GLuint shader, 	GLenum pname, GLint *params){
	glGetShaderiv(shader,pname,params);
	glPrintError(file,line);
}
void 	__glGetShaderInfoLog__(const char *file, int line,GLuint shader,	GLsizei maxLength, 	GLsizei *length,GLchar *infoLog){
	glGetShaderInfoLog(shader,maxLength,length,infoLog);
	glPrintError(file,line);
}
void 	__glDeleteShader__(const char *file, int line,GLuint shader){
	glDeleteShader(shader);
	glPrintError(file,line);
}

//--------------------------

void glDebugIni(void){
	if(g_gldebug_initialized){

		// ok its time to initialize opengl memory/functions stuff
		memset(&g_loaded_id,0,sizeof(g_loaded_id));

		g_gldebug_initialized=GL_TRUE;

	}
}

void glDebugViewStatus(void) {

	if(g_n_loaded_id > 0) {

		printf( "----------- MEM OGL: Mempointers to deallocate %i ---------------------\n",g_n_loaded_id);

		// for all registers get if any is not freed...
		for(unsigned i = 0; i < GL_DEBUG_MAX_MEM_IDS; i++) {
			if(g_loaded_id[i].is_reserved) {
				printf("Mem allocated at filename %s line %i with [id=%i] has not been freed\n",g_loaded_id[i].filename, g_loaded_id[i].line,g_loaded_id[i].id);

			}
		}
	}
	else{
		printf("OGL:ok\n");
	}
}

//#endif
