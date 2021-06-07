#ifndef __ES_SYSTEM_H__
#define __ES_SYSTEM_H__

#define UNLIMITIED_ENTITIES	-1


typedef struct ESSystem ESSystem;

typedef struct{
	size_t 	size_data; // len data component
	void   (*Component_Setup)(void *); // function to Setup component
	void   (*Component_Init)(void *, Entity *); // function to Ini component
	void   (*Component_Update)(void *); // function to update component
	void   (*Component_Destroy)(void *); // function to destroy
}ESSystemRegisterComponent;

struct ESSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ESSystem_Init(void);
unsigned			ESSystem_RegisterComponent(ESSystemRegisterComponent es_component_register);
size_t				ESSystem_NumComponents(void);
void 				ESSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ESSystem 		*	ESSystem_New(void);
void 			*	ESSystem_NewEntityType(ESSystem *_this, const char *_str_entity_type,uint16_t max_entities, unsigned * entity_components, size_t entity_components_len);
Entity  	*		ESSystem_NewEntityFromType(ESSystem *_this,const char *_str_entity_type);

Entity  	*		ESSystem_NewEntity(ESSystem *_this,unsigned * entity_components, size_t entity_components_len);

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
