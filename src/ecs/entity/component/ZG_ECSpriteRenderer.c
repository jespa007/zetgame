#include "ecs/zg_ecs.h"


typedef struct{
	//Shape2d *shape2d;
	ZG_Geometry 		*	geometry;
	ZG_Appearance	 	*  	appearance;
	uint16_t 			width, height;
}ECSpriteRendererData;

static EComponent g_ec_sprite_renderer_required_components[]={
		EC_TRANSFORM
		,EC_GEOMETRY
		,EC_MATERIAL
		,EC_TEXTURE
};

EComponentList ECSpriteRenderer_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_sprite_renderer_required_components;
	cl.n_components=ZG_ARRAY_SIZE(g_ec_sprite_renderer_required_components);

	return cl;
}

void ECSpriteRenderer_Setup(void *_this,ComponentId _id){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ec_sprite_renderer->header.entity=_entity;
	ec_sprite_renderer->header.id=_id;
	_entity->components[EC_SPRITE_RENDERER]=_this;

	ECSpriteRendererData *data=ZG_NEW(ECSpriteRendererData);

	ECGeometry *ec_geometry=_entity->components[EC_GEOMETRY];
	ECMaterial *ec_material=_entity->components[EC_MATERIAL];

	data->appearance=Appearance_New();
	ec_geometry->geometry=data->geometry=ZG_Geometry_NewRectangleFilled(ZG_GEOMETRY_PROPERTY_TEXTURE); // Quad by default ?
	ec_material->material=data->appearance->material=ZG_Material_New(0); // Mat by default ?

	ec_sprite_renderer->data=data;

	ECSpriteRenderer_SetDimensions(ec_sprite_renderer,100,100); // default with/height
}

void ECSpriteRenderer_SetDimensions(ECSpriteRenderer *_this,uint16_t width, uint16_t height){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	if(!(data->width == width && data->height == height)){
		// project dimensions
		ZG_Vector3f p=ZG_ViewPort_ScreenToWorldDimension2i(width>>1,height>>1);

		// setup vertexs...
		float vertexs[ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN]={
				   -p.x,-p.y,0.0f,  // bottom left
				   +p.x,-p.y,0.0f,  // bottom right
				   -p.x,+p.y,0.0f,  // top left
				   +p.x,+p.y,0.0f   // top right

		};


		//.. and set vertex to geometry
	   ZG_Geometry_SetMeshVertex(data->geometry,vertexs,ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN);

		data->width=width;
		data->height=height;

	}
}

void ECSpriteRenderer_SetAlpha(ECSpriteRenderer *_this, float _alpha){
	if(_this == NULL) return;

	ECSpriteRendererData *data=_this->data;
	data->appearance->material->color.a=_alpha;
}

void ECSpriteRenderer_SetTexture(ECSpriteRenderer *_this,ZG_Texture *texture){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	data->appearance->texture=texture;
}


void ECSpriteRenderer_Update(void *_this){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ECSpriteRendererData * data= ec_sprite_renderer->data;
	ZG_Transform *transform = NULL;
	ECTransform *ec_transform=ec_sprite_renderer->header.entity->components[EC_TRANSFORM];
	ECTexture *ec_texture=ec_sprite_renderer->header.entity->components[EC_TEXTURE];
	if(ec_transform){
		transform=&ec_transform->transform;
	}

	if(ec_texture){
		data->appearance->texture=ec_texture->texture;
	}

	Graphics_Draw(transform,data->geometry,data->appearance);
}

void ECSpriteRenderer_Destroy(void *_this){
	ECSpriteRendererData * data= ((ECSpriteRenderer *)_this)->data;
	Appearance_Delete(data->appearance);
	ZG_FREE(data);
}
