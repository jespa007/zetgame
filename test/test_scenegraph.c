#include "zetgame.h"

#define MAX_ENTITIES 10

void MS_OnDeleteTexture(void *text){
	Texture_Delete(text);
}

Entity *NewEntityTransform(
		EntityManager *_em_transforms
		, int _posx
		, int _posy
		, bool _set_displacement
){

	Entity *entity_transform=EntityManager_NewEntity(_em_transforms);

	if(_set_displacement){
		ECTransform_SetDisplacement2i(entity_transform->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ECTransform_SetPosition2i(entity_transform->components[EC_TRANSFORM],_posx,_posy);
	}

	return entity_transform;
}

Entity *NewEntityTexture(
	EntityManager *_em_textures
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, Texture *_texture
	, bool _set_displacement
){
	Entity *entity_texture=EntityManager_NewEntity(_em_textures);

	if(_set_displacement){
		ECTransform_SetDisplacement2i(entity_texture->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ECTransform_SetPosition2i(entity_texture->components[EC_TRANSFORM],_posx,_posy);
	}

	ECTexture_SetTexture(entity_texture->components[EC_TEXTURE],_texture);
	ECSpriteRenderer_SetDimensions(entity_texture->components[EC_SPRITE_RENDERER],_width, _height);

	return entity_texture;
}

Entity *NewEntityTextBox(
	EntityManager *_em_textboxes
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, const char *_text
	, bool _set_displacement
){

	Entity *entity_textbox=EntityManager_NewEntity(_em_textboxes);

	if(_set_displacement){
		ECTransform_SetDisplacement2i(entity_textbox->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ECTransform_SetPosition2i(entity_textbox->components[EC_TRANSFORM],_posx,_posy);
	}

	ECTextBoxRenderer *ec_textbox_renderer=entity_textbox->components[EC_TEXTBOX_RENDERER];
	TextBox_SetText(ec_textbox_renderer->textbox,_text);
	TextBox_SetDimensions(ec_textbox_renderer->textbox,_width,_height);

	return entity_textbox;
}

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	//---------------------------------
	// ALPHA ANIMATION

	float alpha_fade_in_out_keyframes[]={
	//   t   a
	//-----------
		 0	 ,0
		,1000,1
		,2000,0
	};

	//---------------------------------
	// FAN
	typedef struct _fan_info _fan_info;
	struct _fan_info{
		int x,y,w,h;
		struct {
			int x,y;
			struct{
				int x,y,rot;
			}info_vane[3];
		}vane_disp;
	}fan_info[]={
		// 1rst fan
		{
		   // x y w h
		   //----------
			70,210,5,130,
			{
			   // x y
			  //-----

				0,-65,
				{
					 // x y rot
					 //---------
					 {  0,-30,-90}
					,{ 24, 24,45}
					,{-24, 24,-90-90-45}

				}
			}
		},
		// 2nd fan
		{
		   // x y w h
		   //----------
			170,220,5,130,
			{
			   // x y
			  //-----

				0,-65,
				{
					 // x y rot
					 //---------
					 {  0,-30,-90}
					,{ 24,+24,45}
					,{-24,+24,-90-90-45}

				}
			}
		},
		// 3rd fan
		{
			// x y w h
		   //----------
			570,210,5,130,
			{
			   // x y
			   //-----
				0,-65,
				{
					 // x y rot
					 //---------
					 {  0,-30,-90}
					,{ 24,+24,45}
					,{-24,+24,-90-90-45}
				}
			}
		}
	};

	//---------------------------------
	// CAR
	struct
	{
		struct{
			int x,y,w,h;
		}wheel[2];

		struct{
			int x,y,w,h;
		}part1;

		struct{
			int x,y,w,h;
		}part2;

	}car_info= //start struct data
		//  _car_info
		{
			{ // WHEELS

				// WHEEL1
				{
					-50,+40,49,49
				}
				,
				// WHEEL2
				{
					50,+40,49,49
				}
			}

			// PART 1
			,{
				-40,-40,100,100
			}

			// PART 2
			,{
				0,0,200,60
			}
		};

	EComponent transform_components[]={
		EC_TRANSFORM,
	};

	EComponent texture_components[]={
		EC_SPRITE_RENDERER
	};

	EComponent textbox_components[]={
			EC_TEXTBOX_RENDERER
	};
	//----------------------------------------------------------------------------------------------------
	//ZetGameSetupParams setup;
	//memset(&setup,0,sizeof(setup));
	//setup.width=640;
	//setup.height=480;
	//setup.graphic_properties=MSK_GRAPHIC_PROPERTY_FULLSCREEN;
	//ZetGame_Init(&setup);

	// Initializes zetgame with viewport as 640x480 by default
	ZetGame_Init(NULL);

	EntitySystem  *entity_system=EntitySystem_New();


	EntityManager *em_transforms=EntitySystem_NewEntityManager(
		entity_system
		,"nodes"
		,MAX_ENTITIES
		,transform_components
		,ARRAY_SIZE(transform_components)
	);//TextureNode_New());

	EntityManager *em_textures=EntitySystem_NewEntityManager(
		entity_system
		,"textures"
		,MAX_ENTITIES
		,texture_components
		,ARRAY_SIZE(texture_components)
	);//TextureNode_New());




	EntityManager *em_textboxes=EntitySystem_NewEntityManager(
		entity_system
		,"textboxes"
		,MAX_ENTITIES
		,textbox_components
		,ARRAY_SIZE(textbox_components)
	);


	TextureManager *tm=TextureManager_New();
	//TTFontManager *ttfm=TTFontManager_New();
	Entity
				*spr_image_sun=NULL
				,*spr_image_car_part1=NULL
				,*spr_image_car_part2=NULL
				,*spr_image_car_left_wheel=NULL
				,*spr_image_car_right_wheel=NULL;
	Entity *spr_base_car=NULL,*spr_track_car=NULL;


	TTFont_SetFontResourcePath("../../../test/data/fonts");
	TextureManager_SetTextureResourcePath(tm,"../../../test/data/images");

	Texture * text_ground=TextureManager_Get(tm,"ground.png");
	Texture * text_sun=TextureManager_Get(tm,"sun.png");
	Texture * text_vane=TextureManager_Get(tm,"vane.png");
	Texture * text_wheel=TextureManager_Get(tm,"wheel.png");

	// setup animations/actions (update material action function)...
	MaterialAction 	  	 			*mat_act_fade_in_out=MaterialAction_New();//MATERIAL_COMPONENT_MAX);

	//---
	// ground
	NewEntityTexture(
		em_textures
		,Graphics_GetWidth()>>1
		,Graphics_GetHeight()>>1
		,Graphics_GetWidth()
		, Graphics_GetHeight()
		,text_ground
		,false
	);

	//----------------------------------
	// SETUP FAN...
	for(unsigned i=0; i < ARRAY_SIZE(fan_info); i++){
		_fan_info *info=&fan_info[i];
		Entity *spr_image_fan_base=NULL;
		Entity *spr_base_van=NULL;

		spr_image_fan_base=NewEntityTexture(
				em_textures
				,info->x
				,info->y
				,info->w
				,info->h
				,NULL
				,false
				);


		// van base
		spr_base_van=NewEntityTransform(
				em_transforms
				,info->vane_disp.x
				,info->vane_disp.y
				,true
		);

		ECTransform_Attach(
			spr_image_fan_base->components[EC_TRANSFORM]
			,spr_base_van->components[EC_TRANSFORM]
		);

		/*Scene_StartTweenTransform(
			spr_base_van
			,TRANSFORM_COMPONENT_ROTATE_Z
			, EASE_LINEAR
			, 0
			, 360
			, 1000
			, ANIMATION_LOOP_REPEAT_INFINITE
		);*/

		// setup vans & animation
		for(unsigned j=0; j < 3; j++){
			Entity *spr_image_van=NewEntityTexture(
				em_textures
				,info->vane_disp.info_vane[j].x
				,info->vane_disp.info_vane[j].y
				,62
				,9
				,text_vane
				,true
			);

			ECTransform_SetRotate3f(spr_image_van->components[EC_TRANSFORM],0,0,info->vane_disp.info_vane[j].rot);
			ECTransform_Attach(spr_base_van->components[EC_TRANSFORM],spr_image_van->components[EC_TRANSFORM]);

		}
	}
	// SETUP ENTITIES WITHOUT PRE CREATION ?

	//----
	// SETUP CAR
	spr_base_car=NewEntityTransform(em_transforms,Graphics_GetWidth()>>1,Graphics_GetHeight()-150,false); // --> empty entity without id ? It can be but then it cannot be referenced
	spr_track_car=NewEntityTransform(em_transforms,0,0,true);
	spr_image_car_part1=NewEntityTexture(em_textures,car_info.part1.x,car_info.part1.y,car_info.part1.w,car_info.part1.h,NULL,true);
	spr_image_car_part2=NewEntityTexture(em_textures,car_info.part2.x,car_info.part2.y,car_info.part2.w,car_info.part2.h,NULL,true);
	spr_image_car_left_wheel=NewEntityTexture(em_textures,car_info.wheel[0].x,car_info.wheel[0].y,car_info.wheel[0].w,car_info.wheel[0].h,text_wheel,true);
	spr_image_car_right_wheel=NewEntityTexture(em_textures,car_info.wheel[1].x,car_info.wheel[1].y,car_info.wheel[1].w,car_info.wheel[1].h,text_wheel,true);

	ECTransform_Attach(spr_base_car->components[EC_TRANSFORM],spr_track_car->components[EC_TRANSFORM]);
	ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_part1->components[EC_TRANSFORM]);
	ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_part2->components[EC_TRANSFORM]);
	ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_left_wheel->components[EC_TRANSFORM]);
	ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_right_wheel->components[EC_TRANSFORM]);

	/*TransformNode_StartTween(
		spr_base_car
		,TRANSFORM_COMPONENT_TRANSLATE_X
		,EASE_OUT_SINE
		,-2
		,0
		,10000
		,0
	);

	TransformNode_StartTween(
		spr_base_car
		,TRANSFORM_COMPONENT_TRANSLATE_Y
		,EASE_OUT_SINE
		,-0.24-0.1
		,-0.23-0.1
		,100
		,ANIMATION_LOOP_REPEAT_INFINITE
	);*/

	//----
	// SUN
	spr_image_sun=NewEntityTexture(em_textures,Graphics_GetWidth()-200,100,100,100,text_sun,false);
	ECMaterial_SetAlpha(spr_image_sun->components[EC_MATERIAL],ALPHA_VALUE_TRANSPARENT);

	// ani
	/*MaterialAction_SetKeyframesTrack(
			 mat_act_fade_in_out
			,EASE_IN_OUT_SINE
			,alpha_fade_in_out_keyframes
			,ARRAY_SIZE(alpha_fade_in_out_keyframes)
	);*/

	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));


	//Transform transform_camera=Transform_DefaultValues();
	do{

		Graphics_BeginRender();

		/*if(K_SPACE){
			// todo start action from TextureNode_StartMaterialAction
			TextureNode_StartMaterialAction(
					spr_image_sun
					,mat_act_fade_in_out
					,0);
		}*/


		if(Input_IsLeftButtonPressed()){
			printf("Mouse coordinates: %i %i\n",Input_GetMousePositionPtr()->x, Input_GetMousePositionPtr()->y);
		}

		if(K_T){
			Graphics_ToggleFullscreen();
		}

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);


	TextureManager_Delete(tm);
	EntitySystem_Delete(entity_system);
	//TTFontManager_Delete(ttfm);


	MaterialAction_Delete(mat_act_fade_in_out);


	ZetGame_DeInit();

	return 0;
}
