#ifndef __GL_DEBUG_H_
#define __GL_DEBUG_H_

#ifdef __DEBUG__


// redefine custom ogl functions...
#define glFrustum(left,right,bottom,top,nearVal,farVal)						__glFrustum__(__FILE__,__LINE__,left,right,bottom,top,nearVal,farVal)
#define glOrtho(left,right,bottom,top,nearVal,farVal)	 					__glOrtho__(__FILE__,__LINE__,left,right,bottom,top,nearVal,farVal)
#define glCullFace(mode) 													__glCullFace__(__FILE__,__LINE__,mode)
#define glFrontFace(mode)													__glFrontFace__(__FILE__,__LINE__,mode)
#define glEnable(cap) 														__glEnable__(__FILE__,__LINE__,cap)
#define glDisable(cap) 														__glDisable__(__FILE__,__LINE__,cap)
//#define glBegin(mode) 														__glBegin__(__FILE__,__LINE__,mode)
#define glEnd()																__glEnd__(__FILE__,__LINE__)
#define glPushAttrib(mask)													__glPushAttrib__(__FILE__,__LINE__,mask)
#define glPopAttrib()														__glPopAttrib__(__FILE__,__LINE__)
#define glPushClientAttrib(mask)											__glPushClientAttrib__(__FILE__,__LINE__,mask)
#define glPopClientAttrib()													__glPopClientAttrib__(__FILE__,__LINE__)
#define glShadeModel(mode)													__glShadeModel__(__FILE__,__LINE__,mode)
#define glHint(target,mode)													__glHint__(__FILE__,__LINE__,target,mode)
#define glDepthMask(flag)													__glDepthMask__(__FILE__,__LINE__,flag)
#define glGetFloatv(pname,params)											__glGetFloatv__(__FILE__,__LINE__,pname,params)
#define glDepthFunc(func)													__glDepthFunc__(__FILE__,__LINE__,func)
#define glAlphaFunc(func,ref)												__glAlphaFunc__(__FILE__,__LINE__,func,ref)
#define glEnableClientState(cap)											__glEnableClientState__(__FILE__,__LINE__,cap)
#define glDisableClientState(cap)											__glDisableClientState__(__FILE__,__LINE__,cap)
#define glReadPixels(x,y,width,height,format,type,data)						__glReadPixels__(__FILE__,__LINE__,x,y,width,height,format,type,data)


// transform matrix
#define glLoadIdentity() 													__glLoadIdentity__(__FILE__,__LINE__)
#define glMatrixMode(mode) 													__glMatrixMode__(__FILE__,__LINE__,mode)
#define glPushMatrix() 														__glPushMatrix__(__FILE__,__LINE__)
#define glPopMatrix() 														__glPopMatrix__(__FILE__,__LINE__)
#define glMultMatrixf(m)													__glMultMatrixf__(__FILE__,__LINE__,m)

#define glTranslatef(x,y,z)													__glTranslatef__(__FILE__,__LINE__,x,y,z)
#define glTranslated(x,y,z)													__glTranslated__(__FILE__,__LINE__,x,y,z)
#define glRotatef(angle,x,y,z)												__glRotatef__(__FILE__,__LINE__,angle,x,y,z)
#define glRotated(angle,x,y,z)												__glRotated__(__FILE__,__LINE__,angle,x,y,z)
#define glScalef(x,y,z)														__glScalef__(__FILE__,__LINE__,x,y,z)
#define glScaled(x,y,z)														__glScaled__(__FILE__,__LINE__,x,y,z)


// list
#define glNewList(list,mode)	 											__glNewList__(__FILE__,__LINE__,list,mode)
#define glGenLists(range)	 												__glGenLists__(__FILE__,__LINE__,range)
#define glEndList()		 													__glEndList__(__FILE__,__LINE__)


//texture
#define glGenTextures(n,textures)											__glGenTextures__(__FILE__,__LINE__,n,textures)
#define glBindTexture(target,texture)										__glBindTexture__(__FILE__,__LINE__,target,texture)
#define glTexParameteri(target,pname,param)									__glTexParameteri__(__FILE__,__LINE__,target,pname,param)
#define glTexParameteriv(target,pname,param)								__glTexParameteriv__(__FILE__,__LINE__,target,pname,params)
#define glTexParameterf(target,pname,param)									__glTexParameterf__(__FILE__,__LINE__,target,pname,param)
#define glTexParameterfv(target,pname,param)								__glTexParameterfv__(__FILE__,__LINE__,target,pname,params)

#define glTexImage1D(target, level,internalformat,\
		width, border, format, type,data)									__glTexImage1D__(__FILE__,__LINE__,target, level,internalformat, width, border, format, type,data)
#define glTexImage2D(target, level,internalformat,\
		width, height, border, format, type,data)							__glTexImage2D__(__FILE__,__LINE__,target, level,internalformat, width, height, border, format, type,data)
#define glTexImage3D target, level,internalformat,\
		width, height, depth, border, format, type, data);					__glTexImage3D__(__FILE__,__LINE__,target, level,internalformat, width, height, depth, border, format, type,data)
#define glBlendFunc(sfactor,dfactor)										__glBlendFunc__(__FILE__,__LINE__,sfactor,dfactor)
#define glActiveTexture(texture)											__glActiveTexture__(__FILE__,__LINE__,texture)
#define glDeleteTextures(n,textures)										__glDeleteTextures__(__FILE__,__LINE__,n,textures)


//pixel
#define glPixelStorei(name,param)											__glPixelStorei__(__FILE__,__LINE__,name,param)

// point
#define glPointSize(size)													__glPointSize__(__FILE__,__LINE__,size)
#define glPointParameteri(pname,param)										__glPointParameteri__(__FILE__,__LINE__,pname,param)
#define glPointParameteriv(pname,params) 									__glPointParameteriv__(__FILE__,__LINE__,pname,params)
#define glPointParameterf(pname,param)										__glPointParameterf__(__FILE__,__LINE__,pname,param)
#define glPointParameterfv(pname,params) 									__glPointParameterfv__(__FILE__,__LINE__,pname,params)


// buffer
#define	glGenBuffers(n,buffers)												__glGenBuffers__(__FILE__,__LINE__,n,buffers)
#define	glBindBuffer(target,buffer)											__glBindBuffer__(__FILE__,__LINE__,target,buffer)
#define	glBufferData(target,size,data,usage)								__glBufferData__(__FILE__,__LINE__,target,size,data,usage)
#define	glNamedBufferData(buffer,size,data,usage)							__glBufferData__(__FILE__,__LINE__,buffer,size,data,usage)
#define	glDeleteBuffers(n,buffers)											__glDeleteBuffers__(__FILE__,__LINE__,n,buffers)

// framebuffer
#define glGenFramebuffers(n,framebuffers) 									__glGenFramebuffers__(__FILE__,__LINE__,n,framebuffers)
#define glBindFramebuffer(target,framebuffer) 								__glBindFramebuffer__(__FILE__,__LINE__,target,framebuffer)
#define glFramebufferTexture(target,attachment,texture,level)				__glFramebufferTexture__(__FILE__,__LINE__,target,attachment,texture,level)
#define glFramebufferTexture1D(target,attachment,textarget,texture,level)	__glFramebufferTexture1D__(__FILE__,__LINE__,target,attachment,textarget,texture,level)
#define glFramebufferTexture2D(target,attachment,textarget,texture,level)	__glFramebufferTexture2D__(__FILE__,__LINE__,target,attachment,textarget,texture,level)
#define glFramebufferTexture3D(target,attachment,textarget,texture,level,\
							layer)											__glFramebufferTexture3D__(__FILE__,__LINE__,target,attachment,textarget,texture,level,layer)

#define glNamedFramebufferTexture(framebuffer,attachment,texture,level)		__glNamedFramebufferTexture__(__FILE__,__LINE__,framebuffer,attachment,texture,level)


#define glCheckFramebufferStatus(target)									__glCheckFramebufferStatus__(__FILE__,__LINE__,target)
#define glCheckNamedFramebufferStatus(framebuffer,target)					__glCheckFramebufferStatus__(__FILE__,__LINE__,framebuffer,target)

#define glDeleteFramebuffers(n,framebuffers)								__glDeleteFramebuffers__(__FILE__,__LINE__,n,framebuffers)


// vertex array
#define glGenVertexArrays(n, arrays)										__glGenVertexArrays__(__FILE__,__LINE__,n,arrays)
#define glEnableVertexAttribArray(index) 									__glEnableVertexAttribArray__(__FILE__,__LINE__,_index)
#define glDisableVertexAttribArray(_index) 									__glDisableVertexAttribArray__(__FILE__,__LINE__,_index)
#define glDrawElements(_mode,_index_length, _type,_ptr)						__glDrawElements__(__FILE__,__LINE__, _mode,_index_length, _type,_ptr)
#define glBindVertexArray(array)											__glBindVertexArray__(__FILE__,__LINE__,array)
#define glDeleteVertexArrays(n,arrays)										__glDeleteVertexArrays__(__FILE__,__LINE__,n,arrays)
#define glVertexAttribPointer(index,size,type, normalized,\
 							stride,pointer)									__glVertexAttribPointer__(__FILE__,__LINE__,index,size,type, normalized, stride,pointer)

// program
#define	glCreateProgram()													__glCreateProgram__(__FILE__,__LINE__)
#define glUseProgram(program)												__glUseProgram__(__FILE__,__LINE__,program)
#define glGetUniformLocation(program,name)									__glGetUniformLocation__(__FILE__,__LINE__,program,name)
#define glLinkProgram(program)												__glLinkProgram__(__FILE__,__LINE__,program)
#define	glGetProgramiv(program,pname,params)								__glGetProgramiv__(__FILE__,__LINE__,program,pname,params)
#define	glValidateProgram(program)											__glValidateProgram__(__FILE__,__LINE__,program)
#define	glGetProgramInfoLog(program,maxLength,length,infoLog)				__glGetProgramInfoLog__(__FILE__,__LINE__,program,maxLength,length,infoLog)
#define glDeleteProgram(program)											__glDeleteProgram__(__FILE__,__LINE__,program)

// shader
#define	glCreateShader(shaderType)											__glCreateShader__(__FILE__,__LINE__,shaderType)
#define glAttachShader(program,shader)										__glAttachShader__(__FILE__,__LINE__,program,shader)
#define glDetachShader(program,shader)										__glDetachShader__(__FILE__,__LINE__,program,shader)
#define glCompileShader(shader)												__glCompileShader__(__FILE__,__LINE__,shader)
#define glShaderSource(shader,count,string,length)							__glShaderSource__(__FILE__,__LINE__,shader,count,string,length)
#define glGetShaderiv(shader,pname,params)									__glGetShaderiv__(__FILE__,__LINE__,shader,pname,params)
#define glGetShaderInfoLog(shader,maxLength,length,infoLog)					__glGetShaderInfoLog__(__FILE__,__LINE__,shader,maxLength,length,infoLog)
#define	glDeleteShader(shader)												__glDeleteShader__(__FILE__,__LINE__,shader)

#ifdef __cplusplus
extern "C"  {
#endif


void 	__glFrustum__(const char *file, int line,GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble nearVal,GLdouble farVal);
void 	__glOrtho__(const char *file, int line,GLdouble left,  GLdouble right,  GLdouble bottom,  GLdouble top,  GLdouble nearVal,  GLdouble farVal);
void 	__glEnable__(const char *file, int line,	GLenum  	cap);
void 	__glDisable__(const char *file, int line,	GLenum  	cap);
void 	__glGetFloatv__(const char *file, int line,GLenum pname,	GLfloat * params);

void 	__glBegin__(const char *file, int line,	GLenum mode);
void 	__glEnd__(const char *file, int line);
void 	__glEnableClientState__(const char *file, int line,GLenum cap);
void 	__glDisableClientState__(const char *file, int line,GLenum cap);


void 	__glShadeModel__(const char *file, int line,	GLenum mode);
void 	__glHint__(const char *file, int line,	GLenum target, 	GLenum mode);
void 	__glDepthMask__(const char *file, int line,	GLboolean flag);
void 	__glDepthFunc__(const char *file, int line,GLenum func);
void 	__glAlphaFunc__(const char *file, int line,GLenum func,  GLclampf ref);
void 	__glReadPixels__(const char *file,int line, GLint x,GLint y,GLsizei width,	GLsizei height,	GLenum format,	GLenum type,	GLvoid * data);

// transforms
void 	__glLoadIdentity__(const char *file, int line);
void 	__glMatrixMode__(const char *file, int line,GLenum mode);
void 	__glTranslatef__(const char *file, int line,GLfloat  x, GLfloat  y, GLfloat  z);
void 	__glScalef__(const char *file, int line,GLfloat  x, GLfloat  y, GLfloat  z);
void 	__glRotatef__(const char *file, int line,GLfloat  angle,GLfloat  x, GLfloat  y, GLfloat  z);
void 	__glMultMatrixf__(const char *file, int line,GLfloat  *matrix4x4);
void 	__glPushMatrix__(const char *file, int line);
void 	__glPopMatrix__(const char *file, int line);



// lists
void 	__glNewList__(const char *file, int line,	GLuint list, GLenum mode);
GLuint 	__glGenLists__(const char *file, int line,GLsizei range);
void 	__glEndList__(const char *file, int line	);

void 	__glPushAttrib__(const char *file, int line,GLbitfield mask);
void 	__glPopAttrib__(const char *file, int line);
void 	__glPushClientAttrib__(const char *file, int line,GLbitfield mask);
void 	__glPopClientAttrib__(const char *file, int line);
void 	__glCullFace__(const char *file, int line,	GLenum mode);
void 	__glFrontFace__(const char *file, int line,	GLenum mode);


// point
void 	__glPixelStorei__(const char *file, int line,	GLenum pname, 	GLint param);


// ZG_Texture
void 	__glBindTexture__(const char *file, int line,GLenum target,	GLuint texture);
void 	__glTexParameteri__(const char *file, int line,GLenum target, GLenum pname,GLint param);
void 	__glTexParameterf__(const char *file, int line,GLenum target, GLenum pname,GLfloat param);
void 	__glTexImage2D__(const char *file, int line,	GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,const GLvoid * data);
void 	__glActiveTexture__(const char *file, int line,	GLenum texture);
void 	__glGenTextures__(const char *file, int line,	GLsizei n, 	GLuint * textures);
void 	__glDeleteTextures__(const char *file, int line,	GLsizei n, const GLuint * textures);
void 	__glBlendFunc__(const char *file, int line,	GLenum sfactor, 	GLenum dfactor);

// buffers
void 	__glGenBuffers__( const char *file, int line,	GLsizei n,	GLuint * buffers);
void 	__glBindBuffer__(const char *file, int line,	GLenum target,GLuint buffer);
void 	__glBufferData__(const char *file, int line,	GLenum target, GLsizeiptr size,	const GLvoid * data, GLenum usage);
void 	__glDeleteBuffers__( const char *file, int line,	GLsizei n, 	const GLuint * buffers);

// framebuffers
void 	__glGenFramebuffers__(const char *file, int line,	GLsizei n,	GLuint *framebuffers);
void 	__glBindFramebuffer__(const char *file, int line,GLenum target, GLuint framebuffer);
void 	__glFramebufferTexture__(const char *file, int line,	GLenum target, 	GLenum attachment,	GLuint texture,	GLint level);
void 	__glFramebufferTexture2D__(const char *file, int line,	GLenum target,	GLenum attachment, 	GLenum textarget,	GLuint texture,	GLint level);
GLenum	__glCheckFramebufferStatus__(const char *file, int line,	GLenum target);
void 	__glDeleteFramebuffers__(const char *file, int line,GLsizei n,  const GLuint * framebuffers);

// point
void 	__glPointSize__(const char *file, int line,GLfloat size);
void 	__glPointParameterf__(const char *file, int line,	GLenum pname,	GLfloat param);
void 	__glPointParameterfv__(const char *file, int line,GLenum pname,	const GLfloat * params);

// vertexs
void 	__glGenVertexArrays__(const char *file, int line, GLsizei n,	GLuint *arrays);
void 	__glDrawElements__(const char *file, int line, GLenum _mode,GLsizei _index_length,GLenum  _type, const void *_ptr);
void 	__glVertexAttribPointer__(const char *file, int line, 	GLuint index,	GLint size,	GLenum type,GLboolean normalized,GLsizei stride,const GLvoid * pointer);
void 	__glBindVertexArray__(const char *file, int line, 	GLuint array);
void 	__glEnableVertexAttribArray__( const char *file, int line,	GLuint index);
void 	__glDisableVertexAttribArray__(const char *file, int line, 	GLuint index);
void 	__glDeleteVertexArrays__(const char *file,	int line,GLsizei n,	const GLuint *arrays);


// programs
GLuint 	__glCreateProgram__(const char *file, int line);
void 	__glLinkProgram__( const char *file, int line,	GLuint program);
void 	__glValidateProgram__(const char *file, int line, GLuint program);

void 	__glUseProgram__(const char *file, int line,	GLuint program);
GLint 	__glGetUniformLocation__(const char *file, int line,	GLuint program, 	const GLchar *name);
void 	__glGetProgramiv__(const char *file, int line,GLuint program,	GLenum pname, 	GLint *params);
void 	__glGetProgramInfoLog__(const char *file, int line,GLuint program,GLsizei maxLength,GLsizei *length,GLchar *infoLog);

void 	__glDeleteProgram__(const char *file, int line,	GLuint program);

// shaders
GLuint 	__glCreateShader__(const char *file, int line,GLenum shaderType);
void 	__glCompileShader__(const char *file, int line,GLuint shader);
void 	__glShaderSource__( const char *file, int line,	GLuint shader,	GLsizei count,	const GLchar **string, 	const GLint *length);
void 	__glAttachShader__(const char *file, int line,GLuint program,	GLuint shader);
void 	__glDetachShader__(const char *file, int line,GLuint program,	GLuint shader);
void 	__glGetShaderiv__(	const char *file, int line,GLuint shader, 	GLenum pname, GLint *params);
void 	__glGetShaderInfoLog__(const char *file, int line,GLuint shader,	GLsizei maxLength, 	GLsizei *length,GLchar *infoLog);
void 	__glDeleteShader__(const char *file, int line,GLuint shader);

//---------------------


void 	glDebugIni(void);

#ifdef __cplusplus
}
#endif

#endif
#endif

