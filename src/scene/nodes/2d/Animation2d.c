#include "scene/jeme_scene.h"




CSprite2dFrameAnimation::CSprite2dFrameAnimation(){
	loop=true; // loop enabled by default...
	texture=NULL;
	inv_v=0;
	inv_u=0;
	has_collidable_frames=false;
	total_time_animation=0;

}


void CSprite2dFrameAnimation::setLoop(bool _loop){
	this->loop=_loop;
}

bool CSprite2dFrameAnimation::hasCollidableFrames(){
	return has_collidable_frames;
}

void CSprite2dFrameAnimation::resume(){

}

void CSprite2dFrameAnimation::setTexture(CTexture * _texture){
	if(_texture != NULL){
		texture = _texture;
		inv_v = 1.0f/(float)texture->getHeight();
		inv_u = 1.0f/(float)texture->getWidth();
	}
}

bool CSprite2dFrameAnimation::addFrame(Sint16 u,Sint16 v, Sint16 width, Sint16 height, uint32_t time,CPhysics2dCollider * _collider){

	uint16_t max_width,max_height;
	if(texture == NULL){
		ZG_LOG_ERROR("Error. You have to set the image before configure frames");
		return false;
	}

	if(this->texture==NULL){
		ZG_LOG_ERROR("setTexture before call addFrame");
		return false;
	}

	max_width = texture->getWidth();
	max_height = texture->getHeight();

	if(u<0 || v < 0 || ((u+width)>max_width) || ((v+height)>max_height)){
		ZG_LOG_ERROR("crop dimensions out of bounds. You passed [u1:%i v1:%i u2:%i v2:%i] and texture limits are [u1:0 v1:0 u2:%i v2:%i]",
				u,v,u+width,v+height,max_width,max_height);
		return false;
	}

	tSprite2dFrameAnimationFrameInfo sfafi;
	sfafi.idx=(uint8_t)sprite_animation_frame.size();
	sfafi.collider=_collider;

	if(_collider != NULL){
		has_collidable_frames=true;
	}

	sfafi.crop=CQuadf(u*inv_u,v*inv_v,(u+width)*inv_u,(v+height)*inv_v);
	sfafi.time = time;
	sfafi.width = width;
	sfafi.height = height;

	sfafi.projection_width = GET_ACTIVE_RENDER->width_to_projection(width)*0.5;//fabs(GET_ACTIVE_RENDER->pixelX_to_projection(width)-GET_ACTIVE_RENDER->pixelX_to_projection(0));
	sfafi.projection_height =GET_ACTIVE_RENDER->height_to_projection(height)*0.5;// fabs(GET_ACTIVE_RENDER->pixelY_to_projection(height)-GET_ACTIVE_RENDER->pixelY_to_projection(0));


	sprite_animation_frame.push_back(sfafi);

	this->total_time_animation+=time;

	return true;
}

bool CSprite2dFrameAnimation::addFrame(Sint16 u,Sint16 v, Sint16 width, Sint16 height, uint32_t time){

	return addFrame(u,v,width,height,time,NULL);
}

CSprite2dFrameAnimation::tSprite2dFrameAnimationFrameInfo * CSprite2dFrameAnimation::getFrame(uint32_t elapsed_time){

	tSprite2dFrameAnimationFrameInfo *frame=NULL;

	if(sprite_animation_frame.size()>0){

		uint32_t interval=elapsed_time;
		if(loop){
			interval%=total_time_animation;
		}

		if(interval>=total_time_animation){ // get last frame...
			return NULL;
		}else{ // find frame...
			uint32_t t1=0;
			uint32_t t2=0;

			for(unsigned i=0; i < sprite_animation_frame.size()&&frame==NULL; i++){

				t2=t1+sprite_animation_frame[i].time;
				if((t1<=interval)&&(interval<=t2)){
					frame=&sprite_animation_frame[i];
				}
				t1=t2;
			}

			if(frame==NULL){
				int i=0;
				i++;
			}
		}
	}

	return frame;
}

CSprite2dFrameAnimation::tSprite2dFrameAnimationFrameInfo * CSprite2dFrameAnimation::getFirstFrame(){
	if(sprite_animation_frame.size() > 0){
		return &sprite_animation_frame[0];
	}
	else{
		return NULL;//throw std::runtime_error("there's no frames in animation");
	}
	return NULL;
}

