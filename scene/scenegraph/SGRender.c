#include "core/scene/zg_scene.h"

#include "SGRender_GL.c"


#define		MAX_LIGHTS_TO_PROCES			8
#define		MAX_OBJECTS_TO_DRAW 			500
#define		MAX_SCENES_TO_DRAW 			 	10
#define		MAX_DRAWABLE_OBJECTS 		 	30
#define		TRANSLATION_BY_PARAMETER 	 	0x1 << 1
#define		MAX_Z 						 	100



typedef struct{
	List 		*	map_render_z[MAX_Z]; // a map of lists of maximum of MAX_Z
	SGCamera 	* 	sg_camera;
	Color4f			background_color;
}SGRenderData;

static SGCamera * default_camera_render = NULL;

void SGRender_Init(void){
	default_camera_render=SGCamera_New();
	default_camera_render->projection_mode=PROJECTION_MODE_ORTHO;
}

void SGRender_DeInit(void){
	SGCamera_Delete(default_camera_render);
}
//--------
// PRIVATE
static void SGRender_SetSGCamera(SGCamera *_camera){
	SGCamera * sg_camera=_camera;

	if(sg_camera==NULL){
		sg_camera=default_camera_render;
	}

	Graphics_SetProjectionMode(sg_camera->projection_mode);

	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			SGRender_GL_SetSGCamera(sg_camera);
			break;
	}
}



//--------
// PUBLIC
SGRender * SGRender_New(void){
	SGRender *render=NEW(SGRender);
	SGRenderData *data=NEW(SGRenderData);

	// init data...
	for(unsigned z=0; z < MAX_Z; z++){
		data->map_render_z[z]=List_New();
	}


	data->background_color.a=1;

	render->data = data;

	return render;
}


void SGRender_Begin(SGRender *_this,SGCamera * sg_camera){
	SGRenderData *data=(SGRenderData *)_this->data;
	data->sg_camera=sg_camera;
}

void SGRender_SetBackgroundColor3i(SGRender *_this, uint8_t r, uint8_t g, uint8_t b){
	SGRenderData *data=(SGRenderData *)_this->data;
	data->background_color.r=r*ONE_OVER_256;
	data->background_color.g=g*ONE_OVER_256;
	data->background_color.b=b*ONE_OVER_256;
}

void SGRender_AddNodeToDraw(SGRender *_this, SGNode *sg_node){
	int idx_z =0;
	Transform *transform_camera,*transform_node;
	Vector3f origin,camera_space;
	SGRenderData *data=NULL;
	SGCamera *sg_camera=NULL;

	if(_this == NULL) return;
	if(sg_node == NULL) return;

	//transform_shape=node->transform_absolute;
	//if(transform_shape == NULL) return; // invalid shape...

	data=(SGRenderData *)_this->data;
	sg_camera=data->sg_camera;

	if(sg_camera==NULL){
		sg_camera=default_camera_render;
	}



	transform_camera=SGNode_GetTransform(sg_camera->node,SGNODE_TRANSFORM_TYPE_WORLD);
	transform_node=SGNode_GetTransform(sg_node,SGNODE_TRANSFORM_TYPE_WORLD);
	origin = Vector3f_Sub(transform_node->translate,transform_camera->translate);
	camera_space=Quaternion_InverseTransformV3f(transform_camera->quaternion,origin);
	// get z with resolution 10
	idx_z = -camera_space.z;

	// check intervals (if any get outside set to bounding interval)
	if(idx_z >= (MAX_Z)) idx_z = MAX_Z-1;
	if(idx_z < 0) idx_z = 0;//MAX_Z-1;



	List_Add(data->map_render_z[idx_z],sg_node);

}

void SGRender_End(SGRender *_this){
	SGRenderData *data=NULL;

	data=(SGRenderData *)_this->data;
	SGRender_SetSGCamera(data->sg_camera);

	for(unsigned z=0; z < MAX_Z; z++){

		List * sg_nodes = data->map_render_z[z];

		for(int n=0; n < sg_nodes->count ;n++)
		{
			SGNode *node=sg_nodes->items[n];
			Transform *transform_node=SGNode_GetTransform(node,SGNODE_TRANSFORM_TYPE_WORLD);

			Graphics_Draw(transform_node, node->geometry, node->appearance);
		}

		List_Clear(sg_nodes);
	}

}

void SGRender_Delete(SGRender *_this){
	if(_this==NULL)return;

	SGRenderData *data=_this->data;

	for(unsigned z=0; z < MAX_Z; z++){
		List_Delete(data->map_render_z[z]);
	}


	free(_this->data);
	free(_this);
}


