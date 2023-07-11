#include "zetgame.h"

#define MAX_ENTITIES 10

void MS_OnDeleteTexture(void *text){
	ZG_Texture_Delete(text);
}

ZG_Entity *NewEntityTransform(
		ZG_EntityType *_em_transforms
		, int _posx
		, int _posy
		, bool _set_displacement
){

	ZG_Entity *entity_transform=ZG_ZG_EntityType_NewEntity(_em_transforms);

	if(_set_displacement){
		ZG_ECTransform_SetDisplacement2i(entity_transform->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ZG_ECTransform_SetPosition2i(entity_transform->components[EC_TRANSFORM],_posx,_posy);
	}

	return entity_transform;
}

ZG_Entity *NewEntityTexture(
	ZG_EntityType *_em_textures
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, ZG_Texture *_texture
	, bool _set_displacement
){
	ZG_Entity *entity_texture=ZG_ZG_EntityType_NewEntity(_em_textures);

	if(_set_displacement){
		ZG_ECTransform_SetDisplacement2i(entity_texture->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ZG_ECTransform_SetPosition2i(entity_texture->components[EC_TRANSFORM],_posx,_posy);
	}

	ZG_ECTexture_SetTexture(entity_texture->components[EC_TEXTURE],_texture);
	ZG_ECSpriteRenderer_SetDimensions(entity_texture->components[EC_SPRITE_RENDERER],_width, _height);

	return entity_texture;
}

ZG_Entity *NewEntityTextBox(
	ZG_EntityType *_em_textboxes
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, const char *_text
	, bool _set_displacement
){

	ZG_Entity *entity_textbox=ZG_ZG_EntityType_NewEntity(_em_textboxes);

	if(_set_displacement){
		ZG_ECTransform_SetDisplacement2i(entity_textbox->components[EC_TRANSFORM],_posx,_posy);
	}else{
		ZG_ECTransform_SetPosition2i(entity_textbox->components[EC_TRANSFORM],_posx,_posy);
	}

	ZG_ECTextBoxRenderer *ec_textbox_renderer=entity_textbox->components[EC_TEXTBOX_RENDERER];
	ZG_TextBox_SetText(ec_textbox_renderer->textbox,_text);
	ZG_TextBox_SetDimensions(ec_textbox_renderer->textbox,_width,_height);

	return entity_textbox;
}

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

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

	ZG_EComponent transform_components[]={
		EC_TRANSFORM,
	};

	ZG_EComponent texture_components[]={
		EC_SPRITE_RENDERER
	};

	ZG_EComponent textbox_components[]={
			EC_TEXTBOX_RENDERER
	};
	//----------------------------------------------------------------------------------------------------
	//ZG_SetupParams setup;
	//memset(&setup,0,sizeof(setup));
	//setup.width=640;
	//setup.height=480;
	//setup.graphic_properties=ZG_MSK_GRAPHIC_PROPERTY_FULLSCREEN;
	//ZG_Init(&setup);

	// Initializes zetgame with viewport as 640x480 by default
	ZG_Init(NULL);

	ZG_EntitySystem  *entity_system=ZG_EntitySystem_New();


	ZG_EntityType *em_transforms=ZG_EntitySystem_NewEntityType(
		entity_system
		,"nodes"
		,MAX_ENTITIES
		,transform_components
		,ZG_ARRAY_SIZE(transform_components)
	);//TextureNode_New());

	ZG_EntityType *em_textures=ZG_EntitySystem_NewEntityType(
		entity_system
		,"textures"
		,MAX_ENTITIES
		,texture_components
		,ZG_ARRAY_SIZE(texture_components)
	);//TextureNode_New());




	ZG_EntityType *em_textboxes=ZG_EntitySystem_NewEntityType(
		entity_system
		,"textboxes"
		,MAX_ENTITIES
		,textbox_components
		,ZG_ARRAY_SIZE(textbox_components)
	);


	ZG_TextureManager *tm=ZG_TextureManager_New();
	//ZG_TTFontManager *ttfm=ZG_TTFontManager_New();
	ZG_Entity
				*spr_image_sun=NULL
				,*spr_image_car_part1=NULL
				,*spr_image_car_part2=NULL
				,*spr_image_car_left_wheel=NULL
				,*spr_image_car_right_wheel=NULL;
	ZG_Entity *spr_base_car=NULL,*spr_track_car=NULL;


	ZG_TTFont_SetFontResourcePath("../../../test/data/fonts");
	ZG_TextureManager_SetTextureResourcePath(tm,"../../../test/data/images");

	ZG_Texture * text_ground=ZG_TextureManager_Get(tm,"ground.png");
	ZG_Texture * text_sun=ZG_TextureManager_Get(tm,"sun.png");
	ZG_Texture * text_vane=ZG_TextureManager_Get(tm,"vane.png");
	ZG_Texture * text_wheel=ZG_TextureManager_Get(tm,"wheel.png");

	// setup animations/actions (update material action function)...
	ZG_MaterialAction 	  	 			*mat_act_fade_in_out=MaterialAction_New();//ZG_MATERIAL_COMPONENT_MAX);

	//---
	// ground
	NewEntityTexture(
		em_textures
		,ZG_Graphics_GetWidth()>>1
		,ZG_Graphics_GetHeight()>>1
		,ZG_Graphics_GetWidth()
		, ZG_Graphics_GetHeight()
		,text_ground
		,false
	);

	//----------------------------------
	// SETUP FAN...
	for(unsigned i=0; i < ZG_ARRAY_SIZE(fan_info); i++){
		_fan_info *info=&fan_info[i];
		ZG_Entity *spr_image_fan_base=NULL;
		ZG_Entity *spr_base_van=NULL;

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

		ZG_ECTransform_Attach(
			spr_image_fan_base->components[EC_TRANSFORM]
			,spr_base_van->components[EC_TRANSFORM]
		);

		/*Scene_StartTweenTransform(
			spr_base_van
			,ZG_TRANSFORM_COMPONENT_ROTATE_Z
			, ZG_EASE_LINEAR
			, 0
			, 360
			, 1000
			, ANIMATION_LOOP_REPEAT_INFINITE
		);*/

		// setup vans & animation
		for(unsigned j=0; j < 3; j++){
			ZG_Entity *spr_image_van=NewEntityTexture(
				em_textures
				,info->vane_disp.info_vane[j].x
				,info->vane_disp.info_vane[j].y
				,62
				,9
				,text_vane
				,true
			);

			ZG_ECTransform_SetRotate3f(spr_image_van->components[EC_TRANSFORM],0,0,info->vane_disp.info_vane[j].rot);
			ZG_ECTransform_Attach(spr_base_van->components[EC_TRANSFORM],spr_image_van->components[EC_TRANSFORM]);

		}
	}
	// SETUP ENTITIES WITHOUT PRE CREATION ?

	//----
	// SETUP CAR
	spr_base_car=NewEntityTransform(em_transforms,ZG_Graphics_GetWidth()>>1,ZG_Graphics_GetHeight()-150,false); // --> empty entity without id ? It can be but then it cannot be referenced
	spr_track_car=NewEntityTransform(em_transforms,0,0,true);
	spr_image_car_part1=NewEntityTexture(em_textures,car_info.part1.x,car_info.part1.y,car_info.part1.w,car_info.part1.h,NULL,true);
	spr_image_car_part2=NewEntityTexture(em_textures,car_info.part2.x,car_info.part2.y,car_info.part2.w,car_info.part2.h,NULL,true);
	spr_image_car_left_wheel=NewEntityTexture(em_textures,car_info.wheel[0].x,car_info.wheel[0].y,car_info.wheel[0].w,car_info.wheel[0].h,text_wheel,true);
	spr_image_car_right_wheel=NewEntityTexture(em_textures,car_info.wheel[1].x,car_info.wheel[1].y,car_info.wheel[1].w,car_info.wheel[1].h,text_wheel,true);

	ZG_ECTransform_Attach(spr_base_car->components[EC_TRANSFORM],spr_track_car->components[EC_TRANSFORM]);
	ZG_ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_part1->components[EC_TRANSFORM]);
	ZG_ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_part2->components[EC_TRANSFORM]);
	ZG_ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_left_wheel->components[EC_TRANSFORM]);
	ZG_ECTransform_Attach(spr_track_car->components[EC_TRANSFORM],spr_image_car_right_wheel->components[EC_TRANSFORM]);

	/*TransformNode_StartTween(
		spr_base_car
		,ZG_TRANSFORM_COMPONENT_TRANSLATE_X
		,EASE_OUT_SINE
		,-2
		,0
		,10000
		,0
	);

	TransformNode_StartTween(
		spr_base_car
		,ZG_TRANSFORM_COMPONENT_TRANSLATE_Y
		,EASE_OUT_SINE
		,-0.24-0.1
		,-0.23-0.1
		,100
		,ANIMATION_LOOP_REPEAT_INFINITE
	);*/

	//----
	// SUN
	spr_image_sun=NewEntityTexture(em_textures,ZG_Graphics_GetWidth()-200,100,100,100,text_sun,false);
	ZG_ECMaterial_SetAlpha(spr_image_sun->components[EC_MATERIAL],ZG_ALPHA_VALUE_TRANSPARENT);

	// ani
	/*MaterialAction_SetKeyframesTrack(
			 mat_act_fade_in_out
			,ZG_EASE_IN_OUT_SINE
			,alpha_fade_in_out_keyframes
			,ZG_ARRAY_SIZE(alpha_fade_in_out_keyframes)
	);*/

	Graphics_SetBackgroundColor(ZG_Color4f_FromHex(0xFFFF));


	//ZG_Transform transform_camera=ZG_Transform_DefaultValues();
	do{

		ZG_Graphics_BeginRender();

		/*if(K_SPACE){
			// todo start action from TextureNode_StartMaterialAction
			TextureNode_StartMaterialAction(
					spr_image_sun
					,mat_act_fade_in_out
					,0);
		}*/


		if(ZG_Input_IsLeftButtonPressed()){
			printf("Mouse coordinates: %i %i\n",ZG_Input_GetMousePositionPtr()->x, ZG_Input_GetMousePositionPtr()->y);
		}

		if(K_T){
			Graphics_ToggleFullscreen();
		}

		ZG_Graphics_EndRender();

		ZG_Input_Update();

	}while(!ZG_KP_ESC);


	ZG_TextureManager_Delete(tm);
	ZG_EntitySystem_Delete(entity_system);
	//ZG_TTFontManager_Delete(ttfm);


	MaterialAction_Delete(mat_act_fade_in_out);


	ZG_DeInit();

	return 0;
}
