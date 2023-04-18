#include "zg_graphics.h"

typedef struct{
	GLuint program;
}ShaderDataGL;

Shader *  Shader_GL_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr){
	GLint result = GL_FALSE;
	int info_log_length;
	GLuint vertex_shader = GL_INVALID_VALUE;
	GLuint fragment_shader = GL_INVALID_VALUE;
	GLuint program= GL_INVALID_VALUE;
	ShaderDataGL *shader_data=NULL;
	Shader *shader=NULL;


	// Compile Vertex Shader
	Log_InfoF("Compiling vertex shader");
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1,(const GLchar **) &vertex_shader_ptr , NULL);
	glCompileShader(vertex_shader);

	// Check Vertex Shader
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(vertex_shader, info_log_length, NULL, msg);
		Log_ErrorF(msg);
		free(msg);
		return NULL;
	}


	// Compile Fragment Shader
	Log_InfoF("Compiling fragment shader");
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1,(const GLchar **) &fragment_shader_ptr , NULL);
	glCompileShader(fragment_shader);

	// Check Fragment Shader
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(fragment_shader, info_log_length, NULL, msg);
		Log_ErrorF(msg);
		free(msg);
		return NULL;
	}

	// Link the program
	Log_InfoF("Linking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex_shader);
	glAttachShader(ProgramID, fragment_shader);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(program, info_log_length, NULL, msg);
		Log_ErrorF(msg);
		free(msg);
		return NULL;
	}

	glDetachShader(ProgramID, vertex_shader);
	glDetachShader(ProgramID, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	shader_data=ZG_NEW(ShaderDataGL);
	shader_data->program=program;

	shader=ZG_NEW(Shader);
	shader->shader_data=shader_data;
	return shader;
}


Shader * Shader_GL_Load(const char * filename){
	char file_to_load[1024]={0};
	BufferByte *buffer_vertex_shader=NULL,*buffer_fragment_shader=NULL;
	Shader * shader=NULL;

	sprintf(file_to_load,"%s.vs",filename);
	if((buffer_vertex_shader = File_Read(file_to_load))==NULL) return NULL;
	sprintf(file_to_load,"%s.fs",filename);
	if((buffer_fragment_shader=File_Read(file_to_load))==NULL){
		BufferByte_Delete(buffer_vertex_shader);
		return NULL;
	}


	shader=Shader_GL_LoadFromString((char*)buffer_vertex_shader->ptr,(char*)buffer_fragment_shader->ptr);

	BufferByte_Delete(buffer_vertex_shader);
	BufferByte_Delete(buffer_fragment_shader);

	return shader;

}

bool Shader_GL_Use(Shader *shader){
	if(shader == NULL){
		return false;
	}

	ShaderDataGL *shader_data=(ShaderDataGL *)shader->shader_data;

	glUseProgram(shader_data->program);

	return false;

}

void Shader_GL_UnUse(void){
	glUseProgram(0);
}

void Shader_GL_Delete(Shader *shader){
	ShaderDataGL *shader_data;

	if(shader==NULL){
		return;
	}

	shader_data=shader->shader_data;

	if(shader_data->program != GL_INVALID_VALUE){
		glDeleteProgram(shader_data->program);
	}

	ZG_FREE(shader_data);
}
