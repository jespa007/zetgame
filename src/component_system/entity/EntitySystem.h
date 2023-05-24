#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#define UNLIMITIED_ENTITIES	-1

typedef struct{
	EComponent id;
	size_t 	size_data; // len data component
	EComponentList required_components;
	void   (*EComponent_Setup)(void *, Entity *); // function to Setup component
	//void   (*EComponent_Init)(void *, Entity *); // function to Ini component
	void   (*EComponent_Update)(void *); // function to update component
	void   (*EComponent_Destroy)(void *); // function to destroy
}EntitySystemRegisterEComponent;

struct EntitySystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				EntitySystem_Init(void);
int					EntitySystem_RegisterComponent(EntitySystemRegisterEComponent es_component_register);
size_t				EntitySystem_NumComponents(void);
void 				EntitySystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
EntitySystem 	*		EntitySystem_New(void);
EntityManager 	*		EntitySystem_NewEntityManager(
		EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, EComponent * entity_components
		, size_t entity_components_len
);


//Entity  		*		EntitySystem_NewEntity(EntitySystem *_this,EComponent * entity_components, size_t entity_components_len);
void					EntitySystem_Update(EntitySystem *_this);
//uint8_t *EntitySystem_NewComponent(EntitySystem *_this,int idx_component);
/*void  			EntitySystem_RemoveEntity(EntitySystem * _this, Entity entity);

// Channels
Transform 	*		EntitySystem_GetComponentTransform(EntitySystem * _this, Entity entity);
TransformNode 		*		EntitySystem_GetComponentTransformNode(EntitySystem * _this, Entity entity);
Appearance 	*		EntitySystem_GetComponentAppearance(EntitySystem * _this, Entity entity);
Geometry 	*		EntitySystem_GetComponentGeometry(EntitySystem * _this, Entity entity);
Camera 		*		EntitySystem_GetComponentCamera(EntitySystem * _this, Entity entity);

*/
void				EntitySystem_Delete(EntitySystem * _this);




#endif
