#ifndef __SCENE_STATE_H__
#define __SCENE_STATE_H__

typedef struct Scene Scene;
typedef struct SceneState SceneState;


struct SceneState{
	uint32_t current_time;
	void *data;
};

SceneState *SceneState_New(
		Scene *scene
		,const char *name
		,Callback init
		,Callback udpate);

void SceneState_Start(SceneState *_this);
void SceneState_Update(SceneState *_this);

void SceneState_Delete(SceneState *_this);


#endif
