#ifndef __SPRITE2D_H__
#define __SPRITE2D_H__


#define MAX_SPRITE2D_PER_GROUP				10

#define	KILL_WHEN_OFF_SCREEN					(0x1 << 0)
#define	KILL_WHEN_FRAME_ANIMATION_FINISH		(0x1 << 1)
#define	KILL_WHEN_TRANSLATE_ANIMATION_FINISH	(0x1 << 2)
#define	ALPHA_AS_LIFE							(0x1 << 3)
#define	TRANSLATE_BY_MOUSE						(0x1 << 4)

#define UP_MOVE_MASK							(0x1<<0)
#define RIGHT_MOVE_MASK							(0x1<<1)
#define DOWN_MOVE_MASK							(0x1<<2)
#define LEFT_MOVE_MASK							(0x1<<3)

class CSprite2d{
public:
	typedef enum{
		NORMAL=0,
		TEXT2D
	}SPRITE2D_TYPES;



	struct tInfoSprite2dGroup{
		CSprite2d * sprite[MAX_SPRITE2D_PER_GROUP+1]; //
		char n_active_sprites;
	};





public:

	int life;
	int power;
	int score;
	double x_start, y_start;
	double speed;
	double angle_dir;
	double incX, incY;
	short idx;
	int sprite_st;

	vector<CSprite2d *> colliding_sprite;
	tInfoSprite2dGroup *info_group;

	CSprite2d(short id,SPRITE2D_TYPES sprite2d_type = SPRITE2D_TYPES::NORMAL);

	//void startTextBoxNode2d(std::string text, int x,int y, double angle_dir, double  _speed);

	void setText(const string & _text);
	void start(int x,int y, double angle_dir, double  _speed, tInfoSprite2dGroup * _info_group=NULL);
	void die();
	void update();

	void addAction(CAction * _action);

	inline void setFont(CFont * _font){
		font = _font;
	}

	inline uint8_t getCurrentFrameIdx(){
		if(current_frame != NULL) return current_frame->idx;
		return 0;
	}

	uint32_t getElapsedTime();

	void setSprite2dEmitterSource(CSprite2dEmitter * _sprite_emitter_src);


	inline void onIni(std::function<void(CSprite2d *)> * _on_ini_callback){
		on_ini_callback=_on_ini_callback;
	}

	inline void onUpdate(std::function<void(CSprite2d *)> * _on_update_callback){
		on_update_callback=_on_update_callback;
	}

	inline void onDie(std::function<void(CSprite2d *)> * _on_die_callback){
		on_die_callback=_on_die_callback;
	}

	inline void onDieGroup(std::function<void(CSprite2d *)> * _on_die_group_callback){
		on_die_group_callback=_on_die_group_callback;
	}

	inline void onDieElementGroup(std::function<void(CSprite2d *)> * _on_die_element_group_callback){
		on_die_element_group_callback=_on_die_element_group_callback;
	}

	inline void onMouseEntry(std::function<void(CSprite2d *)> * _on_mouse_entry_callback){
		on_mouse_entry_callback=_on_mouse_entry_callback;
	}
	inline void onMouseExit(std::function<void(CSprite2d *)> * _on_mouse_exit_callback){
		on_mouse_exit_callback=_on_mouse_exit_callback;
	}

	inline void onMouseLeftClick(std::function<void(CSprite2d *)> * _on_mouse_left_click_callback){
		on_mouse_left_click_callback=_on_mouse_left_click_callback;
	}

	inline void onChangeFrame(std::function<void(CSprite2d *)> * _on_change_frame_callback){
		on_change_frame_callback=_on_change_frame_callback;
	}

	inline CPhysics2dCollider * getCollider() {
			return collider;
	}

	inline void setSprite2dProperties(unsigned short _properties) {
		sprite_properties=_properties;
	}

	//void spawnSprite2d(const std::string & key, int x, int y, double angle_dir, double speed);

	bool isCollidable();

	bool checkCollision(CPoint2i *point);

	inline CSprite2dEmitter * getSprite2dEmitter(){
		return this->sprite_emitter_src;
	}

	inline bool belongsTo(CSprite2dEmitter *spr_emitter){

		if(spr_emitter==NULL) return NULL;
		return this->sprite_emitter_src==spr_emitter;
	}

	void reset();

	void updateAnimationCollection(std::map<std::string,CSprite2dFrameAnimation *> *_sprite_animation);
	void setSpriteAnimation(const std::string & key);
	void setAnimationByIndex(uint8_t index);

	void setBooleanProperty(const std::string & id, bool _bool_value);
	bool getBooleanProperty(const std::string & id);

	void   setNumberProperty(const std::string & id, double _num_value);
	double getNumberProperty(const std::string & id);

	void 		   		setStringProperty(const std::string & id, const std::string & _str_value);
	const std::string & getStringProperty(const std::string & id);

	virtual void draw();
	void drawSprite2d();
	void drawTextBoxNode2d();

	void setCheckCollision(bool _check_collision);


	virtual ~CSprite2d();

};

#endif
