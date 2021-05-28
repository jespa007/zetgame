#ifndef __SPRITE2D_EMITTER_H__
#define __SPRITE2D_EMITTER_H__

typedef struct Sprite2dEmitter Sprite2dEmitter;

struct Sprite2dEmitter{
	void *data;
};

Sprite2dEmitter * Sprite2dEmitter_New(void);
void Sprite2dEmitter_Delete(Sprite2dEmitter *_this);

/*#define MAX_SPRITE2D_GROUPS					10
#define DISPATCH_FUNCTION_SPRITE2D_CALLBACK_1P(obj_name,fun_name,callback,p1)     		CSprite2dEmitter::DispatchCallbackFuncionSprite2dParam1(obj_name,fun_name,callback,p1)
#define DISPATCH_FUNCTION_SPRITE2D_CALLBACK_2P(obj_name,fun_name,callback,p1,p2) 		CSprite2dEmitter::DispatchCallbackFuncionSprite2dParam2(obj_name,fun_name,callback,p1,p2)


class CFont;
class CSprite2dEmitter:public CSceneElement{


public:

	static void DispatchCallbackFuncionSprite2dParam1(const char * obj_name, const char * function_ptr_name, std::function<void (CSprite2d *)> * function_ptr, CSprite2d * param1);
	static void DispatchCallbackFuncionSprite2dParam2(const char * obj_name,const char * function_ptr_name, std::function<void (CSprite2d *,CSprite2d *)> * function_ptr, CSprite2d * param1, CSprite2d * param2);
	static bool  existCollidingSprite(CSprite2d *spr, CSprite2d *colliding_spr);

	CSprite2dEmitter(const char * sprite_emitter_name);


	bool isCollidable();

	CSprite2d::tInfoSprite2dGroup * getAvailableGroup();

	void setup(uint8_t max_sprites);
	void setup(uint8_t max_sprites, unsigned short properties);
	void setScale(float );

	void setupAsSGTextbox2d(uint8_t max_texts,const std::string & font, uint8_t size,const CColor3i & color);
	void setupAsSGTextbox2d(uint8_t max_texts,const std::string & font, uint8_t size,const CColor3i & color, unsigned short properties);
	void addSprite2dFrameAnimation(const std::string & key,CSprite2dFrameAnimation *_sprite_animation);

	//
	// base group creation. it adds an sprite emitter
	//
	void addSprite2dEmitter( CSprite2dEmitter * emitter, int x, int y, double angle, double speed);

	 //
	 // get random sprite for active sprite list (live > 0)
	 //
	CSprite2d * getRandomSprite();

	void onIni(const std::function<void(CSprite2d *)> & on_ini_callback);
	void onChangeFrame(const std::function<void(CSprite2d *)> & on_change_frame_callback);
	void onDieElementGroup(const std::function<void(CSprite2d *)> & on_die_element_group_callback);
	void onDieGroup(const std::function<void(CSprite2d *)> & on_die_group_callback);
	void onDie(const std::function<void(CSprite2d *)> & on_die_callback);
	void onUpdate(const std::function<void(CSprite2d *)> & on_update_callback);
	void onCollide(const std::function<void(CSprite2d *,CSprite2d *)> & on_collide_callback);
	void onCollideEntry(const std::function<void(CSprite2d *,CSprite2d *)> & on_collide_entry_callback);
	void onCollideExit(const std::function<void(CSprite2d *,CSprite2d *)> & on_collide_exit_callback);
	std::function<void(CSprite2d *,CSprite2d *)> * getOnCollision();
	std::function<void(CSprite2d *,CSprite2d *)> * getOnCollisionEntry();
	std::function<void(CSprite2d *,CSprite2d *)> * getOnCollisionExit();
	void onMouseEntry(const std::function<void(CSprite2d *)> & on_mouse_entry_callback);
	void onMouseExit(const std::function<void(CSprite2d *)> & on_mouse_exit_callback);
	void onMouseLeftClick(const std::function<void(CSprite2d *)> & on_mouse_left_click_callback);

	const char * getName();
	void  clearAll();
	void notifyOnExitCollisionAllCollidedSprites(CSprite2d *spr);
	void  checkCollidingSprites(CSprite2d *spr);
	void  checkCollision(CSprite2dEmitter *spr_emitter);


	//
	// spawn sprite/sprite group at xy coords
	//
	CSprite2d * spawn(int x, int y);

	//
	//  spawn sprite/group at xy coords with angle and speed vars (sprite group ignores angle and speed)
	//
	CSprite2d * spawn(int x, int y,double angle_dir, double  speed );

	//
	// spawn sprite with text, it should be text!
	//
	CSprite2d * spawn(int x, int y, double angle_dir, double  speed,const std::string & text);

	//
	// spawns sprite to target. It calcules automatically the angle. It should set the speed.
	//
	CSprite2d * spawn(int x, int y,double  speed, CSprite2d *target);

	void freeGroup(CSprite2d::tInfoSprite2dGroup *info_group);

	void update();

	virtual ~CSprite2dEmitter();

private:

	typedef struct{
		int x;
		int y;
		double angle;
		double speed;
		CSprite2dEmitter * emitter;
	}tInfoSprite2dEmitterGroup;


	CSprite2d::tInfoSprite2dGroup active_sprite_group[MAX_SPRITE2D_GROUPS];

	CSprite2d **sprite;
	uint8_t max_sprites;

	bool 											sprite_animation_has_colliders;
	const 	char 										* 	SPRITE_EMITTER_NAME;
	std::map<std::string,CSprite2dFrameAnimation *> sprite_animation;
	std::vector<tInfoSprite2dEmitterGroup> 			sprite_emitter_group;
	std::string										sprite_emitter_name;
	CFont *font;
	CFont::FontInfo font_info;

	// 1P params
	std::function<void(CSprite2d *)> *on_ini_callback;
	std::function<void(CSprite2d *)> *on_die_callback;
	std::function<void(CSprite2d *)> *on_change_frame_callback;
	std::function<void(CSprite2d *)> *on_update_callback;
	std::function<void(CSprite2d *)> *on_mouse_entry_callback;
	std::function<void(CSprite2d *)> *on_mouse_exit_callback;
	std::function<void(CSprite2d *)> *on_mouse_left_click_callback;

	// groups
	std::function<void()> *on_ini_group_callback; // on ini group...
	std::function<void(CSprite2d *)> *on_die_group_callback; // spr is the last element
	std::function<void(CSprite2d *)> *on_die_element_group_callback; // spr died

	// 2P params
	std::function<void(CSprite2d *,CSprite2d *)> *on_collide_callback;
	std::function<void(CSprite2d *,CSprite2d *)> *on_collide_entry_callback;
	std::function<void(CSprite2d *,CSprite2d *)> *on_collide_exit_callback;

	CSprite2d * spawn(int x, int y, double angle_dir, double  speed, CSprite2d::tInfoSprite2dGroup  * _info_group);
	void  checkCollision(CSprite2d *incoming_sprite);

	void destroy();


};
*/
#endif
