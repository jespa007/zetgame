#ifndef __ES_SYSTEM_H__
#define __ES_SYSTEM_H__

typedef struct ESSystem ESSystem;

struct ESSystem{
	void *data;
};

ESSystem *		ESSystem_New(void);
void  ESSystem_NewEntityType(ESSystem *_this, const char *_str_entity_type,size_t max_entities,uint32_t msk_ec_types);
Entity  *ESSystem_NewEntity(ESSystem *_this,const char *_str_entity_type);
//uint8_t *ESSystem_NewComponent(ESSystem *_this,int idx_component);
/*void  			ESSystem_RemoveEntity(ESSystem * _this, Entity entity);

// Channels
Transform 	*		ESSystem_GetComponentTransform(ESSystem * _this, Entity entity);
SGNode 		*		ESSystem_GetComponentSGNode(ESSystem * _this, Entity entity);
Appearance 	*		ESSystem_GetComponentAppearance(ESSystem * _this, Entity entity);
Geometry 	*		ESSystem_GetComponentGeometry(ESSystem * _this, Entity entity);
Camera 		*		ESSystem_GetComponentCamera(ESSystem * _this, Entity entity);

*/
ESSystem *		ESSystem_Delete(ESSystem * _this);


#endif
