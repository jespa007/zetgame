#ifndef	__PARTICLE_EMITTER_MANAGER__
#define __PARTICLE_EMITTER_MANAGER__

typedef struct ParticleEmitterManager ParticleEmitterManager;

struct ParticleEmitterManager{
	void *data;
};

ParticleEmitterManager 	*	ParticleEmitterManager_New(ZG_TextureManager	* _texture_manager);
bool 						ParticleEmitterManager_Load(ParticleEmitterManager *_this,const char *_json_filename);


#endif
