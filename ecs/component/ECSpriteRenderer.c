#include "ecs/zg_ecs.h"


typedef struct{
	//Shape2d *shape2d;
	Geometry 		*	geometry;
	Appearance	 	*  	appearance;
	Material		*   material;
	Texture			*   texture;
	uint16_t 			width, height;
}ECSpriteRendererData;

static EComponent g_ec_sprite_renderer_required_components[]={
		ECOMPONENT_TRANSFORM
		,ECOMPONENT_GEOMETRY
		,ECOMPONENT_MATERIAL
		,ECOMPONENT_TEXTURE
};

EComponentList ECSpriteRenderer_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_sprite_renderer_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_sprite_renderer_required_components);

	return cl;
}

void ECSpriteRenderer_Setup(void *_this){
	ECSpriteRenderer *ec_sprite_renderer=_this;

	ECSpriteRendererData *data=NEW(ECSpriteRendererData);

	data->appearance=Appearance_New();
	data->geometry=Geometry_NewQuad(GEOMETRY_TEXTURE); // Quad by default ?
	ECSpriteRenderer_SetDimensions(ec_sprite_renderer,100,100); // default with/height

	ec_sprite_renderer->entity=NULL;
	ec_sprite_renderer->data=data;
}

void ECSpriteRenderer_Init(void *_this,Entity *_entity){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ECSpriteRendererData *data=ec_sprite_renderer->data;
	ec_sprite_renderer->entity=_entity;

	ECGeometry *ec_geometry=ec_sprite_renderer->entity->components[ECOMPONENT_GEOMETRY];
	ECMaterial *ec_material=ec_sprite_renderer->entity->components[ECOMPONENT_MATERIAL];
	ECTexture *ec_texture=ec_sprite_renderer->entity->components[ECOMPONENT_TEXTURE];

	// set sprite
	data->geometry=ec_geometry->geometry;
	data->appearance->material=ec_material->material;
	data->appearance->texture=ec_texture->texture;



}


void ECSpriteRenderer_SetDimensions(ECSpriteRenderer *_this,uint16_t width, uint16_t height){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	if(!(data->width == width && data->height == height)){
		// project dimensions
		Vector3f p=ViewPort_ScreenToWorldDim2i(width>>1,height>>1);

		// setup vertexs...
		float vertexs[N_VERTEX_QUAD*VERTEX_COORDS_LEN]={
				   -p.x,-p.y,0.0f,   // bottom left
				   -p.x,+p.y,0.0f,   // top left
				   +p.x,+p.y,0.0f,   // top right
				   +p.x,-p.y,0.0f    // bottom right
		};


		//.. and set vertex to geometry
	   Geometry_SetMeshVertex(data->geometry,(BufferFloat){
			.data=vertexs
			,.len=N_VERTEX_QUAD*VERTEX_COORDS_LEN
		});

		data->width=width;
		data->height=height;

	}
}

void ECSpriteRenderer_SetAlpha(ECSpriteRenderer *_this, float _alpha){
	if(_this == NULL) return;

	ECSpriteRendererData *data=_this->data;
	data->appearance->material->color.a=_alpha;
}

void ECSpriteRenderer_SetTexture(ECSpriteRenderer *_this,Texture *texture){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	data->appearance->texture=texture;
}


void ECSpriteRenderer_Update(void *_this){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ECSpriteRendererData * data= ec_sprite_renderer->data;
	Transform *transform = NULL;
	ECTransform *ec_transform=ec_sprite_renderer->entity->components[ECOMPONENT_TRANSFORM];
	if(ec_transform){
		transform=&ec_transform->transform;
	}

	Graphics_Draw(transform,data->geometry,data->appearance);
}

void ECSpriteRenderer_Destroy(void *_this){
	ECSpriteRendererData * data= ((ECSpriteRenderer *)_this)->data;
	Appearance_Delete(data->appearance);
	Geometry_Delete(data->geometry);

	free(data);
}
