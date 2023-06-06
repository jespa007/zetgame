#include "Shader_GL.c"


Shader * Shader_Load(const char *filename){
	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Shader_GL_Load(filename);
		break;
	}

	return NULL;
}

Shader * Shader_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr){
	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Shader_GL_LoadFromString(vertex_shader_ptr,fragment_shader_ptr);
		break;
	}

	return NULL;
}

bool Shader_Use(Shader *shader){
	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Shader_GL_Use(shader);
		break;
	}

	return false;
}

void Shader_UnUse(){
	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Shader_GL_UnUse();
		break;
	}
}


void Shader_Delete(Shader *shader){
	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Shader_GL_Delete(shader);
		break;
	}

	ZG_FREE(shader);

}
