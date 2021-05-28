/*
 * CSGNode2d.cpp
 *
 *  Created on: Oct 4, 2014
 *      Author: jespada
 */

#include "scene/jeme_scene.h"

CSGNode2d::CSGNode2d(){

	m_selectedIndex=m_selectedIndexOld=-1;
	// set object centered to screen by default ...
	relative_translation->x=0;
	relative_translation->y=0;
	relative_translation->z=0;
	dim_w=-1;
	dim_h=-1;


	animation = NULL;

	components.onAfterSetChannels=new std::function<void()>([this](){
		uint16_t msk = components.msk_active_channels;

		if(msk & (0x1 << CSGNode2dChannels::SELECTED_INDEX))
			m_selectedIndex = this->components.result[CSGNode2dChannels::SELECTED_INDEX];
	});
}


void  CSGNode2d::setSGNode2dAnimation(CSGNode2dAnimation *_new_animation){


		if(_new_animation != NULL)  {

			if(animation != NULL){
				//Log_Error("object has already controller. You have to do implementation of detach / attach new controller");
				animation->removeChannels(&components);
			}

			animation = _new_animation;
			_new_animation->addChannels(&components);
		}
}

void CSGNode2d::setPositionX(int x){
	relative_translation->x=GET_ACTIVE_RENDER->width_to_projection(x);
}

void CSGNode2d::setPositionY(int y){
	relative_translation->y=-GET_ACTIVE_RENDER->height_to_projection(y);
}


void CSGNode2d::setPosition(int x, int y){
	setPositionX(x);
	setPositionY(y);
}

void CSGNode2d::setPosition(CPoint2i * p){
	setPositionX(p->x);
	setPositionY(p->y);

}

CPoint2i * CSGNode2d::getPosition(){
	GET_ACTIVE_RENDER->world2screen(*absolute_translation,position_aux);
	return &position_aux;
}

void CSGNode2d::setDimensions(uint16_t w, uint16_t h){
	dim_w=w;
	dim_h=h;
}

void CSGNode2d::setWidth(Sint16 w){
	dim_w=w;
}

void CSGNode2d::setHeight(Sint16 h){
	dim_h =h;
}

Sint16 CSGNode2d::getWidth(){
	return dim_w;
}

Sint16 CSGNode2d::getHeight(){
	return dim_h;
}

void CSGNode2d::setRotate(double degress){
	relative_rotation->x = 0;
	relative_rotation->y = 0;
	relative_rotation->z = degress;
}


void CSGNode2d::setScale(float mul){
	relative_scale->x = mul;
	relative_scale->y = mul;

}


void CSGNode2d::setScale(float mul_w, float mul_h){
	relative_scale->x = mul_w;
	relative_scale->y = mul_h;
}

CSGNode2d::~CSGNode2d(){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
