
#include "scene/zg_scene.h"

typedef enum{
	IDLE_ST=0,
	MOVE_UP_ST=1,
	MOVE_UP_RIGHT_ST=2,
	MOVE_RIGHT_ST=3,
	MOVE_DOWN_RIGHT_ST=4,
	MOVE_DOWN_ST=5,
	MOVE_DOWN_LEFT_ST=6,
	MOVE_LEFT_ST=7,
	MOVE_UP_LEFT_ST=8
}SPRITE2D_STATE;



typedef struct{
	ZG_Action * action;
	uint32_t start_time;
}Sprite2dAction;


typedef struct{
	bool  last_in_mouse;

	uint32_t timeFrame,start_time;
	uint32_t *apperance_attributes;
	CSprite2dFrameAnimation::Sprite2dFrameAnimationFrameInfo *current_frame,*last_frame;


	unsigned short sprite_properties;
	unsigned int move_mask,old_move_mask;
	unsigned int move_ani_mask;
	float * CosLut, * SinLut;
	bool 	check_collision;



	CFont *font;
	CQuadf		*crop;


	std::map<std::string,CSprite2dFrameAnimation *> 	*	sprite_animation;
	CSprite2dEmitter 									*   sprite_emitter_src;
	const char											*	SPRITE_EMITTER_NAME;
	//CTexture 											*   m_textureImage;

	//CChannelAnimation2d									*	start_translate_animation;
	//CChannelAnimation2d	 								*	translate_animation;
	//CChannelAnimation1d	 								*	alpha_animation;
	//CChannelAnimation1d									*	brightness_animation;
	std::vector<tSprite2dAction>							actions;
	CSprite2dFrameAnimation 							*	current_animation;
	std::string 											text;

	std::map<std::string,float>   							num_property;
	std::map<std::string,bool>  							bool_property;
	std::map<std::string,std::string>  						string_property;

	CallbackSprite2d on_ini_callback;
	CallbackSprite2d on_update_callback;
	CallbackSprite2d on_die_callback;
	CallbackSprite2d on_die_group_callback;
	CallbackSprite2d on_die_element_group_callback;
	CallbackSprite2d on_mouse_entry_callback;
	CallbackSprite2d on_mouse_exit_callback;
	CallbackSprite2d on_mouse_left_click_callback;
	CallbackSprite2d on_change_frame_callback;

	SPRITE2D_TYPES sprite2d_type;
	CPhysics2dCollider *collider;
}Sprite2dData;

CSprite2d * CSprite2d_New(short _index,SPRITE2D_TYPES _sprite2d_type){

	SPRITE_EMITTER_NAME="Unknow";

	idx=_index;
	angle_dir=0;
	x_start=0;
	y_start=0;
	power=0;
	score=0;
	collider=NULL;
	//translate_animation=NULL;
	font=NULL;
	crop = NULL;

	//ini_script=NULL;
	life=0;
	speed=1;
	//animations_mask=0;
	sprite_st=IDLE_ST;

	apperance_attributes=NULL;
	on_ini_callback=NULL;
	on_update_callback=NULL;
	on_die_callback=NULL;
	on_mouse_entry_callback=NULL;
	on_mouse_exit_callback=NULL;
	on_mouse_left_click_callback=NULL;
	on_change_frame_callback=NULL;
	on_die_element_group_callback=NULL;
	on_die_group_callback=NULL;
	sprite_animation=NULL;


	last_in_mouse=false;
	current_animation=NULL;
	timeFrame=0;

	//auto_dir_anim=false;
	sprite_properties=0;

	move_mask=0;
	move_ani_mask=0;
	old_move_mask=0;
	current_frame=NULL;
	last_frame=NULL;
	start_time=0;

	incX=0;
	incY=0;

	CosLut=CLuts::getInstance()->Cos;
	SinLut=CLuts::getInstance()->Sin;

	sprite_emitter_src=NULL;
	//start_translate_animation=NULL;
	//alpha_animation = NULL;
	//brightness_animation=NULL;
	info_group=NULL;

	//m_textureImage=NULL;
	sprite2d_type = _sprite2d_type;

	apperance_attributes = getAppearance()->getAppearanceAttributesPtr();

	switch(sprite2d_type){
	default:
	case  SPRITE2D_TYPES::NORMAL:
		break;

	case  SPRITE2D_TYPES::TEXT2D:
		font = CFont::getEmbeddedFont();
		break;
	}

	check_collision=true;

}

void CSprite2d::reset(){
	life=0;
	info_group=NULL;
	speed=1;
	//animations_mask=0;
	sprite_st=IDLE_ST;
	last_in_mouse=false;
	current_animation=NULL;
	actions.clear();
	this->num_property.clear();
	this->bool_property.clear();
	this->string_property.clear();
}


void  CSprite2d::start(int x,int y, double _angle_dir, double _speed,  tInfoSprite2dGroup * _info_group ){

	this->reset();

	colliding_sprite.clear();
	life=100;
	move_mask=0;
	old_move_mask=0;
	absolute_old_translation = *absolute_translation;
	timeFrame=m_attached_scene->getTimeScene();
	start_time=m_attached_scene->getTimeScene();
	// set first animation at start...
	setAnimationByIndex(0);

	speed=_speed;
	angle_dir = _angle_dir;

	setPosition(x,y);

	// once we have relative starting xy we can assign starting position....
	x_start=relative_translation->x;
	y_start=relative_translation->y;


	incX=CosLut[CLuts::degrees2fixed(angle_dir)]*speed;
	incY=SinLut[CLuts::degrees2fixed(angle_dir)]*speed;
	info_group=_info_group;

	this->setColor3i(255,255,255);
	this->setOpacity(CMaterial::ZG_ALPHA_VALUE_SOLID);

	DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onIni",on_ini_callback,this);

	check_collision=true;
}

void  CSprite2d::setText(const string & _text){
	text=_text;
}

void CSprite2d::die(){
	life=0;
	// if group != NULL --> group->decElements
	DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onDie",on_die_callback,this);

	// because sprite is died we have to notify other sprites that this was gone...
	sprite_emitter_src->notifyOnExitCollisionAllCollidedSprites(this);


	if(info_group!=NULL){
		if(info_group->n_active_sprites > 0){
			info_group->n_active_sprites--;

			DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onDieElementGroup",on_die_element_group_callback,this);

			if(info_group->n_active_sprites == 0){ // die group...
				DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onDieGroup",on_die_group_callback,this);

				sprite_emitter_src->freeGroup(info_group);
			}
		}


	}
}

void CSprite2d::updateAnimationCollection(std::map<std::string,CSprite2dFrameAnimation *> *_sprite_animation){
	sprite_animation=_sprite_animation;
}

bool CSprite2d::isCollidable(){
	return check_collision;
}

bool CSprite2d::checkCollision(CPoint2i *point){

	if(sprite2d_type != CSprite2d::SPRITE2D_TYPES::NORMAL) return false;
	if(check_collision==false) return false;

	if(collider != NULL){

		float sw=absolute_scale->x, sh=absolute_scale->y;

		float xp=CGraphics::getInstance()->pixelX_to_projection(point->x);
		float yp=CGraphics::getInstance()->pixelY_to_projection(point->y);
		return CPhysics2dCollider::checkCollision(xp,yp,*absolute_translation, collider,sw,sh);
	}
	return false;
}

void CSprite2d::setSpriteAnimation(const std::string & key){

	if(sprite2d_type != CSprite2d::SPRITE2D_TYPES::NORMAL) return;
	if(sprite_animation == NULL) return;


	if(sprite_animation->count(key)==1){
		last_frame=NULL;
		current_animation=NULL;

		current_animation=(*sprite_animation)[key];
		if(current_animation != NULL){

			m_appearance->setTexture(current_animation->getTexture());

			current_frame = current_animation->getFirstFrame();
			if(current_frame != NULL){
				timeFrame=m_attached_scene->getTimeScene()+current_frame->time;
				setDimensions(current_frame->width, current_frame->height);
				collider = current_frame->collider;
			}
		}
	}
}

void CSprite2d::setAnimationByIndex(uint8_t idx){

	if(sprite2d_type != CSprite2d::SPRITE2D_TYPES::NORMAL) return;
	if(sprite_animation == NULL) return;

	if(idx < sprite_animation->size()){
		setSpriteAnimation(sprite_animation->begin()->first);
	}
}


void CSprite2d::addAction(CAction * _action){

	actions.push_back({.action=_action,.start_time=getElapsedTime()});
}


uint32_t CSprite2d::getElapsedTime()
{
		return (m_attached_scene->getTimeScene() - start_time);
}

void CSprite2d::setSprite2dEmitterSource(CSprite2dEmitter * _sprite_emitter_src){

	if(_sprite_emitter_src == NULL) return;

	sprite_emitter_src = _sprite_emitter_src;
	SPRITE_EMITTER_NAME = _sprite_emitter_src->getName();
}

void CSprite2d::setBooleanProperty(const std::string & id, bool _bool_value){
	bool_property[id]=_bool_value;
}

bool CSprite2d::getBooleanProperty(const std::string & id){
	if(bool_property.count(id)==0){
		throw std::runtime_error("bool  \""+id+"\" not exist");
	}
	return bool_property[id];
}

void CSprite2d::setNumberProperty(const std::string & id, double _num_value){
	num_property[id]=_num_value;
}

double CSprite2d::getNumberProperty(const std::string & id){
	if(num_property.count(id)==0){
		throw std::runtime_error("number \""+id+"\" not exist");
	}
	return num_property[id];
}

void CSprite2d::setStringProperty(const std::string & id, const std::string & _str_value){
	string_property[id]=_str_value;
}


const std::string & CSprite2d::getStringProperty(const std::string & id){
	if(string_property.count(id)==0){
		throw std::runtime_error("string \""+id+"\" not exist");
	}
	return string_property[id];
}

void CSprite2d::update(){

	// update coord3d  scene graph...

	if(life==0) return;

	//--------------------------------------------------------------------------------------------v
	// 1. Update relative coords ...
	uint32_t elapsed_time = getElapsedTime();
	bool action_translate=false;
	bool action_translate_finished=false;
	float x_action_translate=0;
	float y_action_translate=0;

	// update actions..
	for(unsigned i=0; i<actions.size();i++){
		tSprite2dAction * spr2d_action = &actions[i];
		spr2d_action->action->update(elapsed_time,spr2d_action->start_time,false);
		CChannels *c=spr2d_action->action->components;
		switch(c->components_type){
		default:
			break;
		case CChannels::COMPONENTS_TYPE::COMPONENTS_TYPE_MATERIAL:
			if(c->msk_active_channels & (0x1 << CMaterialComponents::ALPHA)){
				float opacity = c->result[CMaterialComponents::ALPHA];
				setOpacity(opacity);

				if((sprite_properties & ALPHA_AS_LIFE)!= 0 ){
					life = opacity * 100;
				}
			}
			break;
		case CChannels::COMPONENTS_TYPE::COMPONENTS_TYPE_TRANSFORM:
			action_translate=true;
			if(c->msk_active_channels & (0x1 << CTransformComponents::TRANSLATEX)){
				x_action_translate=c->result[CTransformComponents::TRANSLATEX];
			}
			if(c->msk_active_channels & (0x1 << CTransformComponents::TRANSFORM_COMPONENTS::TRANSLATEY)){
				y_action_translate=c->result[CTransformComponents::TRANSLATEY];
			}
			break;
		}
	}

	if(this->sprite_properties & TRANSLATE_BY_MOUSE){
		setPosition(JEME_INPUT->getMouseCoordinates());
	}else{
		if(action_translate){

			relative_translation->x=x_action_translate+x_start;
			relative_translation->y=y_action_translate+y_start;
		}else{
			relative_translation->x+=incX;
			relative_translation->y+=incY;
		}

	}

	//--------------------------------------------------------------------------------------------v
	// 2. update scene graph to get right absolute coordinates
	CTransformNode::updateSceneGraph();


	if(life==0){
		die();
		return;
	}

	if(action_translate){
		if(action_translate_finished && ((this->sprite_properties & KILL_WHEN_TRANSLATE_ANIMATION_FINISH)!=0)){
				die();
				return;
		}
	}

	//--------------------------------------------------------------------------------------------v
	// 3. call on update callback based on its absolute coords..

	if(on_update_callback != NULL){
		DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onUpdate",on_update_callback,this);
	}

	//--------------------------------------------------------------------------------------------v
	// 4. check collisions according absolute coordinates...
	if((check_collision) && sprite2d_type == CSprite2d::SPRITE2D_TYPES::NORMAL) {

		if(on_mouse_entry_callback != NULL || on_mouse_exit_callback != NULL || on_mouse_left_click_callback != NULL){

			bool in_mouse = checkCollision(JEME_INPUT->getMouseCoordinates());

			if(on_mouse_entry_callback != NULL && in_mouse==true && last_in_mouse==false){
				DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"setOnMouseEnterClickCallback",on_mouse_entry_callback,this);
			}

			if(on_mouse_exit_callback != NULL && in_mouse==false && last_in_mouse==true){
				DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onMouseExit",on_mouse_exit_callback,this);
			}

			if(in_mouse && on_mouse_left_click_callback != NULL && JEME_INPUT->m_mouseLeftButtonPressed){
				DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onMouseLeftClick",on_mouse_left_click_callback,this);
			}

			last_in_mouse=in_mouse;
		}


		// update frame...
		if(current_animation !=NULL){
			if(current_frame != NULL){
				// update frame if needed
				if(current_frame->time != SPRITE2D_FRAME_STOP){
					if(m_attached_scene->getTimeScene() > timeFrame){
						current_frame=current_animation->getFrame(elapsed_time);

						if(on_change_frame_callback!=NULL){
							DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(SPRITE_EMITTER_NAME,"onChangeFrame",on_change_frame_callback,this);
						}

						/*if(current_frame != NULL){
								timeFrame=m_attached_scene->getTicks()+current_frame->time;
						}*/
					}
				}
			}

			if(current_frame == NULL && last_frame != NULL){
				die();
				return;
			}
			last_frame=current_frame;
		}
	}



	if(current_frame != NULL){
		if(((sprite_properties & KILL_WHEN_OFF_SCREEN) != 0)){
			if(m_attached_scene->getSceneRender()->quadOutOfBounds(absolute_translation->x, absolute_translation->y,  absolute_scale->x*current_frame->projection_width, absolute_scale->y*current_frame->projection_height) ){// && CGraphics::getInstance()->quadOutOfScreen(current_frame->width,current_frame->height
				die();
				return;
			}
		}
	}

	if(sprite2d_type == CSprite2d::SPRITE2D_TYPES::NORMAL) {

		move_mask=0;
		// Check whether sprite it moves
		if(absolute_translation->x < absolute_old_translation.x ){ // to left...
			move_mask|=LEFT_MOVE_MASK;

		}else if(absolute_translation->x > absolute_old_translation.x ){ // to right...
			move_mask|=RIGHT_MOVE_MASK;
		}

		if(absolute_translation->y > absolute_old_translation.y ){ // to up...
			move_mask|=UP_MOVE_MASK;
		}else if(absolute_translation->y < absolute_old_translation.y ){ // to down...
			move_mask|=DOWN_MOVE_MASK;
		}

		if(old_move_mask != move_mask){

			switch(move_mask){
			case UP_MOVE_MASK:
				setSpriteAnimation("up");
				break;
			case UP_MOVE_MASK | LEFT_MOVE_MASK:
				setSpriteAnimation("up_left");
				break;
			case UP_MOVE_MASK | RIGHT_MOVE_MASK:
				setSpriteAnimation("up_right");
				break;
			case LEFT_MOVE_MASK:
				setSpriteAnimation("left");
				break;
			case RIGHT_MOVE_MASK:
				setSpriteAnimation("right");
				break;
			case DOWN_MOVE_MASK:
				setSpriteAnimation("down");
				break;
			case DOWN_MOVE_MASK | LEFT_MOVE_MASK:
				setSpriteAnimation("down_left");
				break;
			case DOWN_MOVE_MASK | RIGHT_MOVE_MASK:
				setSpriteAnimation("down_right");
				break;
			}

			old_move_mask = move_mask;
		}
	}


	//--------------------------------------------------------------------------------------------v
	// 5. Finally update draw elements...
	CTransformNode::putTransformNodeToDraw();

}

void CSprite2d::draw(){
	switch(sprite2d_type){
	default:
	case  SPRITE2D_TYPES::NORMAL:
		drawSprite2d();
		break;

	case  SPRITE2D_TYPES::TEXT2D:
		drawTextBoxNode2d();
		break;
	}
}

void CSprite2d::drawTextBoxNode2d() {

	font->renderText(
			GET_ACTIVE_RENDER->projection_to_pixelX(absolute_translation->x),
			GET_ACTIVE_RENDER->projection_to_pixelY(absolute_translation->y),
			text,
			m_appearance->getMaterial()->getColor(),
			CFont::ZG_HORIZONTAL_ALIGNMENT_CENTER
	);

}

void CSprite2d::drawSprite2d(){

	float u1=0,v1=0,u2=1,v2=1;
	float w=0,h=0;
	if(current_frame != NULL) {
		u1=current_frame->crop.x1;
		v1=current_frame->crop.y1;

		u2=current_frame->crop.x2;
		v2=current_frame->crop.y2;

		w=current_frame->projection_width;
		h=current_frame->projection_height;
	}else{
		CTexture * text=this->m_appearance->getTexture();
		w=GET_ACTIVE_RENDER->width_to_projection(text->getWidth()>>1);
		h=GET_ACTIVE_RENDER->height_to_projection(text->getHeight()>>1);
	}

	// set material or appearance ...
 	m_appearanceAbsolute->apply();

    glPushMatrix();
    {
     	transform_absolute.applyTransform();

		glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
		glTexCoord2f(u1, v1);
		glVertex2f(-w, h);

		glTexCoord2f(u2, v1);
		glVertex2f(w, h);

		glTexCoord2f(u1, v2);
		glVertex2f(-w,-h);

		glTexCoord2f(u2, v2);
		glVertex2f(w,-h);

		glEnd();

     	if(core::isDebugMode()){ // draw quad

			if(current_frame->collider !=NULL && (check_collision)){
				auto i=current_frame->collider->collider.begin();
				auto end=current_frame->collider->collider.end();
				glDisable(GL_TEXTURE_2D); // --> because we drawed a sprite before we have to sure that texture is not to allow paint lines..
				glLineWidth(2); // set line width
				glColor3f(1,1,1);

				for( ; i !=end; i++){
					float sw=i->width,sh=i->height;
					glBegin(GL_LINES); // draw something with the texture on
							glVertex2f(-sw, sh);
							glVertex2f(sw, sh);

							glVertex2f(sw,sh);
							glVertex2f(sw, -sh);

							glVertex2f(sw, -sh);
							glVertex2f(-sw, -sh);

							glVertex2f(-sw, -sh);
							glVertex2f(-sw, sh);

					glEnd();
				}
			}
     	}
	}
    glPopMatrix();

	m_appearanceAbsolute->restore_attribs();
}

void CSprite2d::setCheckCollision(bool check_collision){
	this->check_collision=check_collision;
}

CSprite2d::~CSprite2d(){

	/*if(m_textureImage !=NULL){
		delete m_textureImage;
	}*/

	//m_textureImage=NULL;
}
