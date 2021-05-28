#ifndef __SHADER_H__
#define __SHADER_H__

typedef struct Shader Shader;

struct Shader{
	void * shader_data;
};


Shader * Shader_Load(const char *filename);
Shader * Shader_LoadFromString(char *vertex_shader_ptr,char *fragment_shader_ptr);
bool Shader_Use(Shader *shader);
void Shader_UnUse(void);
void Shader_Delete(Shader *shader);

#endif
