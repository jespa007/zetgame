#ifndef __SPRITE_2D_FRAME_ANIMATION_H__
#define __SPRITE_2D_FRAME_ANIMATION_H__

typedef struct Sprite2dFrameAnimation Sprite2dFrameAnimation;

struct Sprite2dFrameAnimation{

};

/*#pragma once


#define SPRITE2D_FRAME_STOP -1

class CSprite2dFrameAnimation{
public:


	typedef struct{
		uint8_t idx;
		CQuadf crop;
		CPhysics2dCollider * collider;
		int time; // if -1 stops animation
		short width;
		short height;
		float projection_width;
		float projection_height;
	}tSprite2dFrameAnimationFrameInfo;

	CSprite2dFrameAnimation();
	void setTexture(CTexture * texture);
	CTexture * getTexture() { return texture;}

	void setLoop(bool _loop);
	void resume();
	//const string & getImageId();
	bool addFrame(Sint16 u,Sint16 v, Sint16 width, Sint16 height, uint32_t time,CPhysics2dCollider * collider);
	bool addFrame(Sint16 u,Sint16 v, Sint16 width, Sint16 height, uint32_t time);
	bool hasCollidableFrames();
	tSprite2dFrameAnimationFrameInfo * getFirstFrame();
	//tSprite2dFrameAnimationFrameInfo * getFrame(uint8_t idx);
	tSprite2dFrameAnimationFrameInfo * getFrame(uint32_t elapsed_time);

private:

	CTexture *texture;
	bool loop;
	bool has_collidable_frames;
	std::vector<tSprite2dFrameAnimationFrameInfo>	sprite_animation_frame;
	float inv_v,inv_u;
	uint32_t total_time_animation;
};
*/

#endif
