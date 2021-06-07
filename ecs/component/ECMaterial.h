#ifndef __EC_MATERIAL_H__
#define __EC_MATERIAL_H__

typedef struct ECMaterial ECMaterial;

struct ECMaterial{
	Material *material;
};

void ECMaterial_Setup(void *_this);
void ECMaterial_Destroy(void *_this);


#endif
