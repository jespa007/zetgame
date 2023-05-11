#include "zetgame.h"

void MS_OnDeleteTexture(void *text){
	Texture_Delete(text);
}


TransformNode *NewTransformNode(Scene *_scene, int posx, int posy, bool set_displacement){

	TransformNode *sg_node=Scene_NewTransformNode(_scene);

	if(set_displacement){
		TransformNode_SetDisplacement2i(sg_node,posx,posy);
	}else{
		TransformNode_SetPosition2i(sg_node,posx,posy);
	}

	return sg_node;
}

TextureNode *NewTextureNode(
	Scene *_scene
	,int posx
	, int posy
	, uint16_t width
	, uint16_t height
	, Texture *texture
	, bool set_displacement
){
	TextureNode *sg_texture=Scene_NewTextureNode(_scene);

	if(set_displacement){
		TransformNode_SetDisplacement2i(sg_texture->sg_node,posx,posy);
	}else{
		TransformNode_SetPosition2i(sg_texture->sg_node,posx,posy);
	}

	TextureNode_SetTexture(sg_texture,texture);
	TextureNode_SetDimensions(sg_texture,width, height);

	return sg_texture;
}

TextBoxNode *NewText2d(
	Scene *_scene
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, const char *_text
	, bool set_displacement
){

	TextBoxNode *textbox=Scene_NewTextBoxNode(_scene);//scene,entity_components,ARRAY_SIZE(entity_components));//TextureNode_New());

	if(set_displacement){
		TransformNode_SetDisplacement2i(textbox->sg_node,_posx,_posy);
	}else{
		TransformNode_SetPosition2i(textbox->sg_node,_posx,_posy);
	}

	TextBox_SetText(textbox->textbox,_text);
	TextBox_SetDimensions(textbox->textbox,_width,_height);

	return textbox;
}

int main(int argc, char * argv[]){
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

	//----------------------------------------------------------------------------------------------------
	//ZetGameSetupParams setup;
	//memset(&setup,0,sizeof(setup));
	//setup.width=640;
	//setup.height=480;
	//setup.graphic_properties=MSK_GRAPHIC_PROPERTY_FULLSCREEN;
	//ZetGame_Init(&setup);

	// Initializes zetgame with viewport as 640x480 by default
	ZetGame_Init(NULL);

	Scene * scene = Scene_New();

	/*EComponent entity_components_node[]={
			EC_TRANSFORM,
			EC_TRANSFORM_ANIMATION
	};


	EComponent entity_components_viewer2d[]={
			EC_SPRITE_RENDERER
			,EC_TRANSFORM_ANIMATION
			,EC_MATERIAL_ANIMATION
			,EC_TRANSFORM
	};

	EntityManager *em_nodes=Scene_NewEntityManager(scene,"nodes",entity_components_node,ARRAY_SIZE(entity_components_node));//TextureNode_New());
	EntityManager *em_viewers=Scene_NewEntityManager(scene,"viewer2d",entity_components_viewer2d,ARRAY_SIZE(entity_components_viewer2d));//TextureNode_New());
*/

	TextureManager *tm=TextureManager_New();
	//TTFontManager *ttfm=TTFontManager_New();
	TextureNode
				*spr_image_sun=NULL
				,*spr_image_car_part1=NULL
				,*spr_image_car_part2=NULL
				,*spr_image_car_left_wheel=NULL
				,*spr_image_car_right_wheel=NULL;
	TransformNode *spr_base_car=NULL,*spr_track_car=NULL;


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
	NewTextureNode(scene
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
		TextureNode *spr_image_fan_base=NULL;
		TransformNode *spr_base_van=NULL;

		spr_image_fan_base=NewTextureNode(
				scene
				,info->x
				,info->y
				,info->w
				,info->h
				,NULL
				,false
				);


		// van base
		spr_base_van=NewTransformNode(scene
							,info->vane_disp.x
							,info->vane_disp.y
							,true
							);

		TransformNode_Attach(
			spr_image_fan_base->sg_node
			,spr_base_van
		);

		TransformNode_StartTween(
			spr_base_van
			,TRANSFORM_COMPONENT_ROTATE_Z
			, EASE_LINEAR
			, 0
			, 360
			, 1000
			, ANIMATION_LOOP_REPEAT_INFINITE
		);

		// setup vans & animation
		for(unsigned j=0; j < 3; j++){
			TextureNode *spr_image_van=NewTextureNode(scene
						,info->vane_disp.info_vane[j].x
						,info->vane_disp.info_vane[j].y
						,62
						,9
						,text_vane
						,true);

			TransformNode_SetRotate3f(spr_image_van,0,0,info->vane_disp.info_vane[j].rot);
			TransformNode_Attach(spr_base_van,spr_image_van->sg_node);

		}
	}
	// SETUP ENTITIES WITHOUT PRE CREATION ?

	//----
	// SETUP CAR
	spr_base_car=NewNode(scene,Graphics_GetWidth()>>1,Graphics_GetHeight()-150,false); // --> empty entity without id ? It can be but then it cannot be referenced
	spr_track_car=NewNode(scene,0,0,true);
	spr_image_car_part1=NewTextureNode(scene,car_info.part1.x,car_info.part1.y,car_info.part1.w,car_info.part1.h,NULL,true);
	spr_image_car_part2=NewTextureNode(scene,car_info.part2.x,car_info.part2.y,car_info.part2.w,car_info.part2.h,NULL,true);
	spr_image_car_left_wheel=NewTextureNode(scene,car_info.wheel[0].x,car_info.wheel[0].y,car_info.wheel[0].w,car_info.wheel[0].h,text_wheel,true);
	spr_image_car_right_wheel=NewTextureNode(scene,car_info.wheel[1].x,car_info.wheel[1].y,car_info.wheel[1].w,car_info.wheel[1].h,text_wheel,true);

	TransformNode_Attach(spr_base_car,spr_track_car);
	TransformNode_Attach(spr_track_car,spr_image_car_part1->sg_node);
	TransformNode_Attach(spr_track_car,spr_image_car_part2->sg_node);
	TransformNode_Attach(spr_track_car,spr_image_car_left_wheel->sg_node);
	TransformNode_Attach(spr_track_car,spr_image_car_right_wheel->sg_node);

	TransformNode_StartTween(
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
	);

	//----
	// SUN
	spr_image_sun=NewTextureNode(scene,Graphics_GetWidth()-200,100,100,100,text_sun,false);
	TextureNode_SetAlpha(spr_image_sun,ALPHA_VALUE_TRANSPARENT);

	// ani
	MaterialAction_SetKeyframesTrack(
			 mat_act_fade_in_out
			,EASE_IN_OUT_SINE
			,alpha_fade_in_out_keyframes
			,ARRAY_SIZE(alpha_fade_in_out_keyframes)
	);

	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));
	Scene_Start(scene);

	//Transform transform_camera=Transform_DefaultValues();
	do{

		Graphics_BeginRender();

		if(K_SPACE){
			// todo start action from TextureNode_StartMaterialAction
			TextureNode_StartMaterialAction(
					spr_image_sun
					,mat_act_fade_in_out
					,0);
		}

		Scene_Update(scene);

		if(Input_IsLeftButtonPressed()){
			printf("Mouse coordinates: %i %i\n",Input_GetMousePositionPtr()->x, Input_GetMousePositionPtr()->y);
		}

		if(K_T){
			Graphics_ToggleFullscreen();
		}

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);

	Scene_Delete(scene);

	TextureManager_Delete(tm);
	//TTFontManager_Delete(ttfm);


	Action_Delete(mat_act_fade_in_out);


	ZetGame_DeInit();

	return 0;
}
