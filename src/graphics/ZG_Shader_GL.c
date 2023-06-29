#include "_zg_graphics_.h"

typedef struct{
	GLuint program;
}ZG_ShaderDataGL;

ZG_Shader *  ZG_Shader_GL_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr){
	GLint result = GL_FALSE;
	int info_log_length;
	GLuint vertex_shader = GL_INVALID_VALUE;
	GLuint fragment_shader = GL_INVALID_VALUE;
	GLuint program= GL_INVALID_VALUE;
	ZG_ShaderDataGL *shader_data=NULL;
	ZG_Shader *shader=NULL;


	// Compile Vertex ZG_Shader
	ZG_LOG_INFOF("Compiling vertex shader");
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1,(const GLchar **) &vertex_shader_ptr , NULL);
	glCompileShader(vertex_shader);

	// Check Vertex ZG_Shader
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(vertex_shader, info_log_length, NULL, msg);
		ZG_LOG_ERRORF(msg);
		free(msg);
		return NULL;
	}


	// Compile Fragment ZG_Shader
	ZG_LOG_INFOF("Compiling fragment shader");
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1,(const GLchar **) &fragment_shader_ptr , NULL);
	glCompileShader(fragment_shader);

	// Check Fragment ZG_Shader
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(fragment_shader, info_log_length, NULL, msg);
		ZG_LOG_ERRORF(msg);
		free(msg);
		return NULL;
	}

	// Link the program
	ZG_LOG_INFOF("Linking program");
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		char *msg=(char *)malloc(info_log_length+1);
		memset(msg,0,info_log_length+1);
		glGetShaderInfoLog(program, info_log_length, NULL, msg);
		ZG_LOG_ERRORF(msg);
		free(msg);
		return NULL;
	}

	glDetachShader(program_id, vertex_shader);
	glDetachShader(program_id, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	shader_data=ZG_NEW(ZG_ShaderDataGL);
	shader_data->program=program;

	shader=ZG_NEW(ZG_Shader);
	shader->shader_data=shader_data;
	return shader;
}


ZG_Shader * ZG_Shader_GL_Load(const char * filename){
	char file_to_load[1024]={0};
	ZG_BufferByte *buffer_vertex_shader=NULL,*buffer_fragment_shader=NULL;
	ZG_Shader * shader=NULL;

	sprintf(file_to_load,"%s.vs",filename);
	if((buffer_vertex_shader = ZG_File_Read(file_to_load))==NULL) return NULL;
	sprintf(file_to_load,"%s.fs",filename);
	if((buffer_fragment_shader=ZG_File_Read(file_to_load))==NULL){
		ZG_BufferByte_Delete(buffer_vertex_shader);
		return NULL;
	}


	shader=ZG_Shader_GL_LoadFromString((char*)buffer_vertex_shader->ptr,(char*)buffer_fragment_shader->ptr);

	ZG_BufferByte_Delete(buffer_vertex_shader);
	ZG_BufferByte_Delete(buffer_fragment_shader);

	return shader;

}

bool ZG_Shader_GL_Use(ZG_Shader *shader){
	if(shader == NULL){
		return false;
	}

	ZG_ShaderDataGL *shader_data=(ZG_ShaderDataGL *)shader->shader_data;

	glUseProgram(shader_data->program);

	return false;

}

void ZG_Shader_GL_UnUse(void){
	glUseProgram(0);
}

void ZG_Shader_GL_Delete(ZG_Shader *shader){
	ZG_ShaderDataGL *shader_data;

	if(shader==NULL){
		return;
	}

	shader_data=shader->shader_data;

	if(shader_data->program != GL_INVALID_VALUE){
		glDeleteProgram(shader_data->program);
	}

	ZG_FREE(shader_data);
}
