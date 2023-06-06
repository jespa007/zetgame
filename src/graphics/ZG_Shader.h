#ifndef __ZG_SHADER_H__
#define __ZG_SHADER_H__

typedef struct ZG_Shader ZG_Shader;

struct ZG_Shader{
	void * shader_data;
};


ZG_Shader * ZG_Shader_Load(const char *filename);
ZG_Shader * ZG_Shader_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr);
bool ZG_Shader_Use(ZG_Shader *_this);
void ZG_Shader_UnUse(void);
void ZG_Shader_Delete(ZG_Shader *_this);

#endif
