#ifndef __ES_SYSTEM_H__
#define __ES_SYSTEM_H__

#define UNLIMITIED_ENTITIES	-1


typedef struct ESSystem ESSystem;


typedef struct{
	EComponent id;
	size_t 	size_data; // len data component
	EComponentList required_components;
	void   (*EComponent_Setup)(void *, Entity *); // function to Setup component
	//void   (*EComponent_Init)(void *, Entity *); // function to Ini component
	void   (*EComponent_Update)(void *); // function to update component
	void   (*EComponent_Destroy)(void *); // function to destroy
}ESSystemRegisterEComponent;

struct ESSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ESSystem_Init(void);
EComponent			ESSystem_RegisterComponent(ESSystemRegisterEComponent es_component_register);
size_t				ESSystem_NumComponents(void);
void 				ESSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ESSystem 		*	ESSystem_New(void);
void 			*	ESSystem_NewEntityManager(ESSystem *_this, const char *_str_entity_manager,uint16_t max_entities, EComponent * entity_components, size_t entity_components_len);
Entity  	*		ESSystem_NewEntityFromManager(ESSystem *_this,const char *_str_entity_manager);

Entity  	*		ESSystem_NewEntity(ESSystem *_this,EComponent * entity_components, size_t entity_components_len);
void				ESSystem_Update(ESSystem *_this);
//uint8_t *ESSystem_NewComponent(ESSystem *_this,int idx_component);
/*void  			ESSystem_RemoveEntity(ESSystem * _this, Entity entity);

// Channels
Transform 	*		ESSystem_GetComponentTransform(ESSystem * _this, Entity entity);
SGNode 		*		ESSystem_GetComponentSGNode(ESSystem * _this, Entity entity);
Appearance 	*		ESSystem_GetComponentAppearance(ESSystem * _this, Entity entity);
Geometry 	*		ESSystem_GetComponentGeometry(ESSystem * _this, Entity entity);
Camera 		*		ESSystem_GetComponentCamera(ESSystem * _this, Entity entity);

*/
void				ESSystem_Delete(ESSystem * _this);




#endif
