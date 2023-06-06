#include "ZG_Shader_GL.c"


ZG_Shader * ZG_Shader_Load(const char *filename){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		return ZG_Shader_GL_Load(filename);
		break;
	}

	return NULL;
}

ZG_Shader * ZG_Shader_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		return ZG_Shader_GL_LoadFromString(vertex_shader_ptr,fragment_shader_ptr);
		break;
	}

	return NULL;
}

bool ZG_Shader_Use(ZG_Shader *_this){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		return ZG_Shader_GL_Use(_this);
		break;
	}

	return false;
}

void ZG_Shader_UnUse(){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Shader_GL_UnUse();
		break;
	}
}


void ZG_Shader_Delete(ZG_Shader *_this){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Shader_GL_Delete(_this);
		break;
	}

	ZG_FREE(_this);

}
