#ifndef __SCENE_H__
#define __SCENE_H__

typedef struct Scene Scene;
//typedef struct SGNode SGNode;

struct Scene{
	uint32_t 			current_time;
	//SGRender 	*		sg_render;
	// on event functions
	Callback	on_start;
	Callback	on_update;
	void  		* 		data;
};

Scene * Scene_New(void);


void Scene_Clean(void);

/*void Scene_AddSceneState(Scene *_this
		, const char *name
		, Callback init
		, Callback update);*/

/*void Scene_AttachNode(Scene *_this,SGNode *node);*/
void Scene_AttachAnimation(Scene *_this,Animation *animation);
void Scene_AttachMoviePlayer(Scene *_this,MoviePlayer *movie_player);
//void Scene_AttachSprite2dEmitter(Scene *_this,Sprite2dEmitter *sprite2d_emitter);

void 	Scene_NewEntityType(Scene *_this, const char *_str_entity_type,size_t max_entities,EntityComponent * entity_components, size_t entity_components_len);
Entity* Scene_NewEntity(Scene *_this, const char *_str_entity_type);

bool Scene_IsRunning(Scene *_this);
void Scene_SetBackgroundColor3i(Scene *_this,uint8_t r,uint8_t g,uint8_t b);
void Scene_Start(Scene *_this);
void Scene_Resume(Scene *_this);
void Scene_Stop(Scene *_this);
void Scene_Pause(Scene *_this);
void Scene_OnPostRender(Scene *_this,Callback cf);

void Scene_Update(Scene *scene);

void Scene_Delete(Scene *_this);

#endif
