#include "scene/zg_scene.h"

Sprite2dEmitter * SpriteEmitter2d_New(void){
	Sprite2dEmitter *sprite_emitter=NEW(Sprite2dEmitter);
	return sprite_emitter;
}

void Sprite2dEmitter_Delete(Sprite2dEmitter *_this){
	FREE(_this);
}

/*void CSprite2dEmitter::DispatchCallbackFuncionSprite2dParam1(const char * obj_name,const char * function_ptr_name, std::function<void (CSprite2d *)> * function_ptr, CSprite2d * param1){
	if(function_ptr!=NULL){

		try{
			(*function_ptr)(param1);
		}
		catch(std::exception & ex){
			Log_Error("%s::%s:%s",obj_name,function_ptr_name,ex.what());
		}

	}
}

void CSprite2dEmitter::DispatchCallbackFuncionSprite2dParam2(const char * obj_name,const char * function_ptr_name, std::function<void (CSprite2d *,CSprite2d *)> * function_ptr, CSprite2d * param1, CSprite2d * param2){
	if(function_ptr!=NULL){

		try{
			(*function_ptr)(param1,param2);
		}
		catch(std::exception & ex){
			Log_Error("%s::%s:%s",obj_name,function_ptr_name,ex.what());
		}

	}
}

CSprite2dEmitter::CSprite2dEmitter(const char * sprite_emitter_name){
	on_ini_callback=NULL;
	on_update_callback=NULL;
	on_die_callback=NULL;
	on_mouse_entry_callback=NULL;
	on_mouse_exit_callback=NULL;
	on_mouse_left_click_callback=NULL;
	on_collide_callback=NULL;
	on_change_frame_callback=NULL;
	on_ini_group_callback=NULL; // on ini group...
	on_die_group_callback=NULL; // spr is the last element
	on_die_element_group_callback=NULL; // spr died
	on_collide_entry_callback = NULL;
	on_collide_exit_callback = NULL;
	font=NULL;
	sprite_animation_has_colliders=false;
	max_sprites = 0;
	sprite = NULL;

	SPRITE_EMITTER_NAME=sprite_emitter_name;
	memset(active_sprite_group,0,sizeof(active_sprite_group));



}

void CSprite2dEmitter::setup(uint8_t _max_sprites, unsigned short properties){

	if(_max_sprites == 0) return;

	//SPRITE_EMITTER_NAME = this->getName().c_str();
	// Remove existing ones...
	destroy();

	max_sprites=_max_sprites;
	sprite=(CSprite2d **)malloc(sizeof(CSprite2d*)*max_sprites);

	// redo
	for(unsigned i = 0; i < max_sprites;i++){
		CSprite2d *spr = new CSprite2d(i);
		sprite[i]=spr;
		spr->setScene(this->getScene()); // set attached scene...
		//spr->setFixedDimensions(true); // to not do auto width height when sets an image ...

		spr->setSprite2dProperties(properties);
		spr->setSprite2dEmitterSource(this);
	}

}

void CSprite2dEmitter::setup(uint8_t max_sprites){
	setup(max_sprites,0);
}

void CSprite2dEmitter::setupAsSGTextbox2d(uint8_t max_texts,const std::string & ttf_font, uint8_t font_size,const CColor3i & color, unsigned short properties ){

	if(max_texts == 0) return;

	font_info.font_name = ttf_font;
	font_info.font_size=font_size;

	//SPRITE_EMITTER_NAME = this->getName().c_str();
	// Remove existing ones...
	destroy();

	max_sprites=max_texts;
	sprite=(CSprite2d **)malloc(sizeof(CSprite2d*)*max_sprites);

	font = CFont::getFont(&font_info);

	// redo
	for(unsigned i = 0; i < max_texts;i++){
		CSprite2d *spr = new CSprite2d(i,CSprite2d::SPRITE2D_TYPES::TEXT2D);

		spr->setFont(font);

		sprite[i]=spr;
		spr->setColor(color);
		//spr->setFixedDimensions(true); // to not do auto width height when sets an image ...
		//spr->setSize(width,height);
		spr->setSprite2dProperties(properties);
		spr->setSprite2dEmitterSource(this);

		spr->setScene(this->getScene());
	}
}

void CSprite2dEmitter::setupAsSGTextbox2d(uint8_t max_texts,const std::string & font_name, uint8_t size,const CColor3i & color ){
	setupAsSGTextbox2d( max_texts, font_name,  size,   color,0 );
}

void CSprite2dEmitter::setScale(float s){
	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->setScale(s);
	}
}

void CSprite2dEmitter::addSprite2dFrameAnimation(const std::string & key,CSprite2dFrameAnimation *_sprite_animation){
	if(sprite_animation.count(key)==1){
		throw std::runtime_error("sprite animation \""+key+"\" already exist in container");
	}

	sprite_animation[key]=_sprite_animation;

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->updateAnimationCollection(&sprite_animation);
	}

	sprite_animation_has_colliders|=_sprite_animation->hasCollidableFrames();
}

bool CSprite2dEmitter::isCollidable(){
	return sprite_animation_has_colliders;
}

void CSprite2dEmitter::addSprite2dEmitter(CSprite2dEmitter * emitter, int x, int y, double angle, double speed){
	tInfoSprite2dEmitterGroup iseg;
	iseg.emitter=emitter;
	iseg.x=x;
	iseg.y=y;
	iseg.angle=angle;
	iseg.speed=speed;

	sprite_emitter_group.push_back(iseg);
}

void CSprite2dEmitter::onIni(const std::function<void(CSprite2d *)> & _on_ini_callback){

	if(on_ini_callback != NULL){ throw std::runtime_error("onIni already set");}

	on_ini_callback=new std::function<void(CSprite2d *)>(_on_ini_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onIni(on_ini_callback);
	}
}

void CSprite2dEmitter::onDie(const std::function<void(CSprite2d *)> & _on_die_callback){

	if(on_die_callback != NULL){ throw std::runtime_error("onDie already set");}

	on_die_callback=new std::function<void(CSprite2d *)>(_on_die_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onDie(on_die_callback);
	}
}

const char * CSprite2dEmitter::getName(){
	return SPRITE_EMITTER_NAME;
}

void  CSprite2dEmitter::clearAll(){
	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->reset();
	}
}

void CSprite2dEmitter::onDieElementGroup(const std::function<void(CSprite2d *)> & _on_die_element_group_callback){
	if(on_die_element_group_callback != NULL){ throw std::runtime_error("onDieElementGroup already set");}

	on_die_element_group_callback=new std::function<void(CSprite2d *)>(_on_die_element_group_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onDieElementGroup(on_die_element_group_callback);
	}
}

void CSprite2dEmitter::onDieGroup(const std::function<void(CSprite2d *)> & _on_die_group_callback){
	if(on_die_group_callback != NULL){ throw std::runtime_error("onDieGroup already set");}

	on_die_group_callback=new std::function<void(CSprite2d *)>(_on_die_group_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onDieGroup(on_die_group_callback);
	}
}


void CSprite2dEmitter::onChangeFrame(const std::function<void(CSprite2d *)> & _on_change_frame_callback){

	if(_on_change_frame_callback != NULL){ throw std::runtime_error("onChangeFrame already set");}

	on_change_frame_callback=new std::function<void(CSprite2d *)>(_on_change_frame_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onChangeFrame(on_change_frame_callback);
	}
}

void CSprite2dEmitter::onUpdate(const std::function<void(CSprite2d *)> & _on_update_callback){

	if(on_update_callback != NULL){ throw std::runtime_error("onUpdate already set");}

	on_update_callback=new std::function<void(CSprite2d *)>(_on_update_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onUpdate(on_update_callback);
	}
}

void CSprite2dEmitter::onCollide(const std::function<void(CSprite2d *,CSprite2d *)> & _on_collide_callback){

	if(on_collide_callback != NULL){ throw std::runtime_error("onCollide already set");}

	on_collide_callback=new std::function<void(CSprite2d *,CSprite2d *)>(_on_collide_callback);

}

void CSprite2dEmitter::onCollideEntry(const std::function<void(CSprite2d *,CSprite2d *)> & _on_collide_entry_callback){
	if(on_collide_entry_callback != NULL){ throw std::runtime_error("on collide entry already set");}

	on_collide_entry_callback=new std::function<void(CSprite2d *,CSprite2d *)>(_on_collide_entry_callback);
}

void CSprite2dEmitter::onCollideExit(const std::function<void(CSprite2d *,CSprite2d *)> & _on_collide_exit_callback){
	if(on_collide_exit_callback != NULL){ throw std::runtime_error("on collide exit already set");}

	on_collide_exit_callback=new std::function<void(CSprite2d *,CSprite2d *)>(_on_collide_exit_callback);
}


std::function<void(CSprite2d *,CSprite2d *)> * CSprite2dEmitter::getOnCollision(){
	return on_collide_callback;
}

std::function<void(CSprite2d *,CSprite2d *)> * CSprite2dEmitter::getOnCollisionEntry(){
	return on_collide_entry_callback;
}


std::function<void(CSprite2d *,CSprite2d *)> * CSprite2dEmitter::getOnCollisionExit(){
	return on_collide_exit_callback;
}


void CSprite2dEmitter::onMouseEntry(const std::function<void(CSprite2d *)> & _on_mouse_entry_callback){
	if(on_mouse_entry_callback != NULL){ throw std::runtime_error("onMouseEntry already set");}

	on_mouse_entry_callback=new std::function<void(CSprite2d *)>(_on_mouse_entry_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onMouseEntry(on_mouse_entry_callback);
	}
}

void CSprite2dEmitter::onMouseExit(const std::function<void(CSprite2d *)> & _on_mouse_exit_callback){
	if(on_mouse_exit_callback != NULL){ throw std::runtime_error("onMouseExit already set");}

	on_mouse_exit_callback=new std::function<void(CSprite2d *)>(_on_mouse_exit_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onMouseExit(on_mouse_exit_callback);
	}
}

void CSprite2dEmitter::onMouseLeftClick(const std::function<void(CSprite2d *)> & _on_mouse_left_click_callback){
	if(on_mouse_left_click_callback != NULL){ throw std::runtime_error("onMouseLeftClick already set");}

	on_mouse_left_click_callback=new std::function<void(CSprite2d *)>(_on_mouse_left_click_callback);

	for(unsigned i = 0; i < max_sprites;i++){
		sprite[i]->onMouseLeftClick(on_mouse_left_click_callback);
	}
}


void  CSprite2dEmitter::checkCollision(CSprite2d *incoming_sprite){
	// Check collision incoming sprite with all active sprites ...
	float sw2=incoming_sprite->absolute_scale->x,
		  sh2 = incoming_sprite->absolute_scale->y;
	CVector3f * pos_2 = incoming_sprite->absolute_translation;
	CSprite2dEmitter * incoming_emitter=incoming_sprite->getSprite2dEmitter();
	CPhysics2dCollider *collider_2 = incoming_sprite->getCollider();
	std::function<void(CSprite2d *,CSprite2d *)> *on_collide_callback_2=incoming_emitter->getOnCollision();
	std::function<void(CSprite2d *,CSprite2d *)> *on_collide_entry_callback_2=incoming_emitter->getOnCollisionEntry();
	const char *SPRITE_EMITTER_NAME_2=incoming_emitter->getName();


	for(unsigned i = 0; i < max_sprites;i++){
		CSprite2d *spr = sprite[i];//->idx;
		if(spr->life > 0){

			CPhysics2dCollider *collider = spr->getCollider();
			float sw1=spr->absolute_scale->x,
					sh1=spr->absolute_scale->y;


			 if(collider!=NULL&&spr->isCollidable()){

			if(CPhysics2dCollider::checkCollision(spr->getTranslate(CSGNode::WORLD_COORDINATES)
												 ,collider,sw1,sh1
												 ,*pos_2
												 ,collider_2
												 ,sw2,sh2)
			){

				if(!this->existCollidingSprite(spr,incoming_sprite)){

					spr->colliding_sprite.push_back(incoming_sprite);
					DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME,"onCollideEntry",on_collide_entry_callback,spr,incoming_sprite);
				}

				if(!incoming_emitter->existCollidingSprite(incoming_sprite,spr)){
					incoming_sprite->colliding_sprite.push_back(spr);
					DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME_2,"onCollideEntry",on_collide_entry_callback_2,incoming_sprite,spr);
				}

				if(on_collide_callback!=NULL){
					DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME,"onCollide",on_collide_callback,spr,incoming_sprite);
				}
				if(on_collide_callback_2!=NULL){
					DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME_2,"onCollide",on_collide_callback_2,incoming_sprite,spr);
				}
			}
		}

			//if(spr->colliding_sprite!=NULL){ // last frame was collided
			//	 DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME,"setOnExitCollideCallback",on_collide_exit_callback,spr,spr->colliding_sprite);
			//	 spr->colliding_sprite=NULL;
			 //}


		}
	}
}

bool  CSprite2dEmitter::existCollidingSprite(CSprite2d *spr, CSprite2d *colliding_spr){
	if(spr == NULL || colliding_spr == NULL) return false;
	if(colliding_spr->life == 0) return false;
	//CSprite2dEmitter *incoming_emitter=incoming_spr->getSprite2dEmitter();

	for(unsigned i=0; i < spr->colliding_sprite.size(); i++){
		if(spr->colliding_sprite[i]->life>0){
			if(spr->colliding_sprite[i]==colliding_spr){
				return true;
			}
		}
	}
	return false;
}

void CSprite2dEmitter::notifyOnExitCollisionAllCollidedSprites(CSprite2d *spr){

	for(unsigned  i = 0; i <  spr->colliding_sprite.size(); i++){

		CSprite2d *colliding_sprite=spr->colliding_sprite[i];
		DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME,"setOnExitCollideCallback",on_collide_exit_callback,colliding_sprite,spr);
	}
	spr->colliding_sprite.clear();

}

void CSprite2dEmitter::checkCollidingSprites(CSprite2d *spr){

	if(spr->life == 0)
		return;

	float sw_1=spr->relative_scale->x,
		  sh_1 = spr->relative_scale->y;
	CVector3f * pos_1 = spr->relative_translation;
	//std::function<void(CSprite2d *,CSprite2d *)> *on_collide_exit_callback_2=incoming_emitter->getOnCollisionExit();

	CPhysics2dCollider *collider_1 = spr->getCollider();

	// foreach sprite check whether the colliding sprite is not colliding anymore...
	for(auto i = spr->colliding_sprite.begin(); i !=  spr->colliding_sprite.end();){
		CSprite2d * colliding_sprite = *i;//spr->colliding_sprite[i];
		CPhysics2dCollider *collider_2 = colliding_sprite->getCollider();
		float sw_2=colliding_sprite->relative_scale->x,
			  sh_2=colliding_sprite->relative_scale->y;

		if(!CPhysics2dCollider::checkCollision(
				  *pos_1
				 ,collider_1
				 ,sw_1
				 ,sh_1
				 ,*colliding_sprite->relative_translation
				 ,collider_2
				 ,sw_2
				 ,sh_2) || colliding_sprite->life==0


		){ // exits collision ...

			DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(SPRITE_EMITTER_NAME,"setOnExitCollideCallback",on_collide_exit_callback,spr, colliding_sprite);

			spr->colliding_sprite.erase(i);
		}
		else{
			i++;
		}
	}
}

void  CSprite2dEmitter::checkCollision(CSprite2dEmitter *spr_emitter){

	if(spr_emitter == this) {
		return; // you cannot collision within the same type (in principle)
	}

	for(unsigned i = 0; i < max_sprites;i++){

		CSprite2d *spr = sprite[i];//->idx;
		if(spr->life > 0 && spr->isCollidable()){
			spr_emitter->checkCollision(spr);//,spr->getCollider());
		}
	}
}

CSprite2d * CSprite2dEmitter::getRandomSprite(){
	if(max_sprites == 0) return NULL;

	for(unsigned i = 0; i < max_sprites;i++){
		if(sprite[i]->life > 0) return sprite[i];
	}

	return NULL;
}

void  CSprite2dEmitter::update(){

	for(unsigned i = 0 ; i < max_sprites;i++){
		CSprite2d *spr=sprite[i];
		if(spr->life>0){
			spr->update();
			checkCollidingSprites(spr);
		}
	}

}

CSprite2d *  CSprite2dEmitter::spawn(int x, int y, double  angle_dir, double  speed, CSprite2d::tInfoSprite2dGroup  * _info_group){

	if(_info_group==NULL && sprite_emitter_group.size()>0){ // manage group
				bool found=true;
			CSprite2d::tInfoSprite2dGroup *grp=getAvailableGroup();

			if(grp!=NULL){

				CSprite2d **it=grp->sprite;
				// it will spawn as many sprites as MAX SPRITES defineed in the emitter itself.
				for(unsigned i = 0; i < sprite_emitter_group.size() && found && i < MAX_SPRITE2D_PER_GROUP;i++){
					CSprite2d* spr = sprite_emitter_group[i].emitter->spawn(sprite_emitter_group[i].x+x,sprite_emitter_group[i].y+y,sprite_emitter_group[i].angle, sprite_emitter_group[i].speed,grp);
					if(spr != NULL){
						*it++=spr;
						grp->n_active_sprites++;
					}else{
						found=false;
					}
				}
			}
	}else{
		for(unsigned i=0; i < max_sprites; i++){
			CSprite2d *spr = sprite[i];
			if(spr->life == 0 && (spr->info_group==NULL)){
				spr->start(x,y,angle_dir,speed,_info_group);
				return spr;
			}
		}
	}

	Log_Error("%s: cannot spawn more sprites. Increase max sprite number (current: %i)",SPRITE_EMITTER_NAME,max_sprites);
	return NULL;
}

CSprite2d * CSprite2dEmitter::spawn(int x, int y){
	return spawn( x,  y,0, 0, NULL);
}

CSprite2d * CSprite2dEmitter::spawn(int x, int y,double angle_dir, double  speed ){
	return spawn( x,  y,   angle_dir,   speed, NULL);
}


CSprite2d *  CSprite2dEmitter::spawn(int x, int y, double angle_dir, double  speed,const std::string & text){

	CSprite2d *spr = spawn(x,y,angle_dir,speed);
	if(spr){
		spr->setText(text);
	}

	return spr;
}


CSprite2d * CSprite2dEmitter::spawn(int x, int y, double  speed,CSprite2d *target){

	CSprite2d *spr2d=NULL;

	//CSprite2d *spr2d = e->getRandomSprite();
	if(target != NULL){
		double angle = 0;
		CPoint2i *p2=target->getPosition();

		angle = atan2_approximation1(p2->y-y, p2->x-x);
		//Log_Info("angle: %f",angle);
		angle=-RADIANS_TO_DEGREES(angle);
		//Log_Info("angle: %f",angle);
		if(angle < 0) angle+=360;
		//Log_Info("angle: %f",angle);

		spr2d=spawn(x,y,angle,speed);
	}
	return spr2d;
}

CSprite2d::tInfoSprite2dGroup * CSprite2dEmitter::getAvailableGroup()
{
	CSprite2d::tInfoSprite2dGroup *it=&active_sprite_group[0];
	CSprite2d::tInfoSprite2dGroup *end=&active_sprite_group[MAX_SPRITE2D_GROUPS-1];

	do{
		if(it->n_active_sprites == 0){
			memset(it->sprite,0,sizeof(it->sprite));
			return it;
		}
		it++;
	}while(it!=end);

	return NULL;
}

void CSprite2dEmitter::freeGroup(CSprite2d::tInfoSprite2dGroup *info_group){
	if(info_group->n_active_sprites==0){
		CSprite2d **it =info_group->sprite;
		while(*it != NULL){
			(*it)->info_group = NULL;
			it++;
		}
	}
}

void CSprite2dEmitter::destroy(){

	for(unsigned i = 0; i < max_sprites;i++){
		if(sprite[i]!=NULL){
			delete sprite[i];
		}
	}

	free(sprite);

}

CSprite2dEmitter::~CSprite2dEmitter(){
	destroy();

	if(on_ini_callback != NULL) delete on_ini_callback;
	if(on_die_callback!= NULL) delete on_die_callback;
	if(on_update_callback!= NULL) delete on_update_callback;
	if(on_mouse_entry_callback!= NULL) delete on_mouse_entry_callback;
	if(on_mouse_exit_callback!= NULL) delete on_mouse_exit_callback;
	if(on_mouse_left_click_callback!= NULL) delete on_mouse_left_click_callback;

	if(on_ini_group_callback!= NULL) delete on_ini_group_callback; // on ini group...
	if(on_die_group_callback!= NULL) delete on_die_group_callback; // spr is the last element
	if(on_die_element_group_callback!= NULL) delete on_die_element_group_callback; // spr died

	// 2P params
	if(on_collide_callback!= NULL) delete on_collide_callback;
	if(on_collide_entry_callback!= NULL) delete on_collide_entry_callback;
	if(on_collide_exit_callback!= NULL) delete on_collide_exit_callback;

}
*/
