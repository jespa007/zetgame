#include "../ZetgineCore.h"

void MS_OnDeleteTexture(void *text){
	Texture_Delete(text);
}


int main(int argc, char * argv[]){

	//---------------------------------
	// ROTATION ANI FRAMES
	float rotate_z_keyframes[]={
	//   t   rz
	//-----------
		 0	 ,0
		,1000,360
	};

	float translate_x_keyframes[]={
	//   t    tx
	//-----------
		    0,-2
	   ,10000, 0
	};

	float translate_y_keyframes[]={
			 // t    ty
			 //---- ---
			 0   ,    -0.24-0.1,
			 100 ,    -0.23-0.1,
			 200 ,    -0.24-0.1,
	};


	float alpha_fade_in_out_keyframes[]={
	//   t   a
	//-----------
		 0	 ,0
		,1000,1
		,2000,0
	};

	float selected_index_keyframes[]={
	//   t   a
	//-----------
		 0	 ,0
		,10000,1
		//,3500,1
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

	//MapString *map_textures = MapString_New();
	//map_textures->on_delete=MS_OnDeleteTexture;
	ZetgineCoreSetupParams setup;
	memset(&setup,0,sizeof(setup));
	setup.width=640;
	setup.height=480;
	setup.graphic_properties=MSK_GRAPHIC_PROPERTY_DESKTOP;


	ZetgineCore_Init(&setup);

	Scene * scene = Scene_New();
	SGViewer2d *sg_image_background=NULL
				,*sg_image_sun=NULL
				,*sg_image_car_part1=NULL
				,*sg_image_car_part2=NULL
				,*sg_image_car_left_wheel=NULL
				,*sg_image_car_right_wheel=NULL;
	TransformNode *sg_base_car=NULL;

	List *sg_viewers2d = List_New(); // list of graph nodes...
	List *sg_nodes = List_New(); // list of graph nodes...



	TTFont_SetFontResourcePath("data/fonts");
	Texture_SetTextureResourcePath("data/images");

	Texture * text_ground=Texture_LoadFromFile("data/images/ground.png");
	Texture * text_sun=Texture_LoadFromFile("data/images/sun.png");
	Texture * text_vane=Texture_LoadFromFile("data/images/vane.png");
	Texture * text_wheel=Texture_LoadFromFile("data/images/wheel.png");

	// setup animations/actions...
	TransformAnimation *transform_ani_fan=TransformAnimation_New();
	TransformAction 	 *transform_act_fan=TransformAction_New();
	TransformAnimation *transform_ani_car=TransformAnimation_New();
	TransformAction 	 *transform_act_car_x=TransformAction_New();
	TransformAction 	 *transform_act_car_y=TransformAction_New();

	MaterialAnimation 	 *mat_ani_sun=MaterialAnimation_New();
	MaterialAction 	  	 *mat_act_fade_in_out=MaterialAction_New();




	//---
/*
	for(unsigned i=0; i < ARRAY_SIZE(text_info); i++){
		_text_info *info=&text_info[i];
		MapString_SetValue(map_textures,info->name,Texture_Load(info->name));
	}*/

	// ground...

	List_Add(sg_viewers2d,sg_image_background=SGViewer2d_New());
	Scene_AttachNode(scene,sg_image_background->node);
	TransformNode_SetPosition2i(sg_image_background->node,Graphics_GetWidth()>>1,Graphics_GetHeight()>>1);

	sg_image_background->node->appearance->texture=text_ground;
	SGViewer2d_SetDimensions(sg_image_background,Graphics_GetWidth(), Graphics_GetHeight());

	//----------------------------------
	// SETUP FAN...
	for(unsigned i=0; i < ARRAY_SIZE(fan_info); i++){
		_fan_info *info=&fan_info[i];
		SGViewer2d *sg_image_fan_base=NULL;
		TransformNode *sg_node_base_van=NULL;

		List_Add(sg_viewers2d,sg_image_fan_base=SGViewer2d_New());
		Scene_AttachNode(scene,sg_image_fan_base->node);

		// by default it sets default texture... set to no paint
		//sg_image_fan_base->node->appearance->texture=NULL;

		TransformNode_SetPosition2i(sg_image_fan_base->node,info->x,info->y);
		SGViewer2d_SetDimensions(sg_image_fan_base,info->w,info->h);

		// van base
		List_Add(sg_nodes,sg_node_base_van=TransformNode_New());
		TransformNode_SetPosition2i(sg_node_base_van,info->vane_disp.x,info->vane_disp.y);
		TransformNode_AttachNode(sg_image_fan_base->node,sg_node_base_van);

		// set animation rotate
		TransformAnimation_AddTransform(transform_ani_fan,sg_node_base_van->transform);

		// setup vans
		for(unsigned j=0; j < 3; j++){
			SGViewer2d *sg_image_van=NULL;
			List_Add(sg_viewers2d,sg_image_van=SGViewer2d_New());

			TransformNode_SetPosition2i(sg_image_van->node,info->vane_disp.info_vane[j].x,info->vane_disp.info_vane[j].y);
			TransformNode_SetRotate3f(sg_image_van->node,0,0,info->vane_disp.info_vane[j].rot);
			SGViewer2d_SetDimensions(sg_image_van,62,9);

			sg_image_van->node->appearance->texture=text_vane;

			TransformNode_AttachNode(sg_node_base_van,sg_image_van->node);
		}
		//sg_image_background->node->appearance->texture=text_ground;
	}

	// setup fan animation...
	TransformAnimation_AddAction(transform_ani_fan,transform_act_fan,true);
	TransformAction_SetKeyFramesRotate(
			transform_act_fan
			,COMPONENT_CHANNEL_Z
			,INTERPOLATOR_TYPE_LINEAR
			,rotate_z_keyframes
			,ARRAY_SIZE(rotate_z_keyframes));



	Scene_AttachAnimation(scene,transform_ani_fan->animation);


	//----
	// SETUP CAR
	List_Add(sg_nodes,sg_base_car=TransformNode_New());
	List_Add(sg_viewers2d,sg_image_car_part1=SGViewer2d_New());
	List_Add(sg_viewers2d,sg_image_car_part2=SGViewer2d_New());
	List_Add(sg_viewers2d,sg_image_car_left_wheel=SGViewer2d_New());
	List_Add(sg_viewers2d,sg_image_car_right_wheel=SGViewer2d_New());

	Scene_AttachNode(scene,sg_base_car);

	TransformNode_AttachNode(sg_base_car,sg_image_car_part1->node);
	TransformNode_SetPosition2i(sg_image_car_part1->node,car_info.part1.x,car_info.part1.y);
	SGViewer2d_SetDimensions(sg_image_car_part1,car_info.part1.w,car_info.part1.h);

	TransformNode_AttachNode(sg_base_car,sg_image_car_part2->node);
	TransformNode_SetPosition2i(sg_image_car_part2->node,car_info.part2.x,car_info.part2.y);
	SGViewer2d_SetDimensions(sg_image_car_part2,car_info.part2.w,car_info.part2.h);

	TransformNode_AttachNode(sg_base_car,sg_image_car_left_wheel->node);
	TransformNode_SetPosition2i(sg_image_car_left_wheel->node,car_info.wheel[0].x,car_info.wheel[0].y);
	SGViewer2d_SetDimensions(sg_image_car_left_wheel,car_info.wheel[0].w,car_info.wheel[0].h);
	sg_image_car_left_wheel->node->appearance->texture=text_wheel;

	TransformNode_AttachNode(sg_base_car,sg_image_car_right_wheel->node);
	TransformNode_SetPosition2i(sg_image_car_right_wheel->node,car_info.wheel[1].x,car_info.wheel[1].y);
	SGViewer2d_SetDimensions(sg_image_car_right_wheel,car_info.wheel[1].w,car_info.wheel[1].h);
	sg_image_car_right_wheel->node->appearance->texture=text_wheel;

	TransformAction_SetKeyFramesTranslate(
		 transform_act_car_x
		,COMPONENT_CHANNEL_X
		,INTERPOLATOR_TYPE_LINEAR
		,translate_x_keyframes
		,ARRAY_SIZE(translate_x_keyframes)
	);

	TransformAction_SetKeyFramesTranslate(
			 transform_act_car_y
			,COMPONENT_CHANNEL_Y
			,INTERPOLATOR_TYPE_LINEAR
			,translate_y_keyframes
			,ARRAY_SIZE(translate_y_keyframes)
	);

	TransformAnimation_AddAction(transform_ani_car,transform_act_car_x,false);
	TransformAnimation_AddAction(transform_ani_car,transform_act_car_y,true);
	Scene_AttachAnimation(scene,transform_ani_car->animation);

	TransformAnimation_AddTransform(transform_ani_car,sg_base_car->transform);

	//----
	// SUN

	List_Add(sg_viewers2d,sg_image_sun=SGViewer2d_New());
	Scene_AttachNode(scene,sg_image_sun->node);
	TransformNode_SetPosition2i(sg_image_sun->node,Graphics_GetWidth()-200,100);

	sg_image_sun->node->appearance->texture=text_sun;
	SGViewer2d_SetDimensions(sg_image_sun,100,100);

	MaterialAnimation_AddMaterial(mat_ani_sun,sg_image_sun->node->appearance->material);
	sg_image_sun->node->appearance->material->color.a=ALPHA_VALUE_TRANSPARENT;


	// ani
	MaterialAction_SetKeyFramesAlpha(
			 mat_act_fade_in_out
			,INTERPOLATOR_TYPE_LINEAR
			,alpha_fade_in_out_keyframes
			,ARRAY_SIZE(alpha_fade_in_out_keyframes)
	);

	Scene_AttachAnimation(scene,mat_ani_sun->animation);



	/*for(unsigned i=0; i < ARRAY_SIZE(fan_info); i++){
		_fan_info *info=&fan_info[i];


	}*/

	// load textures
	/*Texture *text_ground=Texture_Load("ground.png");
	Texture *text_sun=Texture_Load("sun.png");
	Texture *text_vane=Texture_Load("vane.png");
	Texture *text_wheel=Texture_Load("wheel.png");*/

	// create objects
	//Viewer2d *v2d_ground=Viewer2d_New();
	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));
	Scene_Start(scene);
	do{

		Graphics_BeginRender();

		if(K_SPACE){
			MaterialAnimation_TriggerAction(mat_ani_sun,mat_act_fade_in_out,false);
		}

		Scene_Update(scene);
		//Graphics_Draw(sg_image_background->node->transform,sg_image_background->node->geometry,sg_image_background->node->appearance);

		if(Input_IsMouseButtonPressed()){
			printf("Mouse coordinates: %i %i\n",Input_GetMousePositionPtr()->x, Input_GetMousePositionPtr()->y);
		}


		Graphics_EndRender();

		Input_Update();
		SDL_Delay(10);


	}while(!K_ESC);

	Scene_Delete(scene);


	Texture_Delete(text_ground);
	Texture_Delete(text_sun);
	Texture_Delete(text_vane);
	Texture_Delete(text_wheel);

	for(unsigned i = 0; i < sg_viewers2d->count; i++){
		SGViewer2d *sg_viewer2d=sg_viewers2d->items[i];
		SGViewer2d_Delete(sg_viewer2d);
	}

	List_Delete(sg_viewers2d);

	for(unsigned i = 0; i < sg_nodes->count; i++){
		TransformNode *sg_node=sg_nodes->items[i];
		TransformNode_Delete(sg_node);
	}

	List_Delete(sg_nodes);

	TransformAnimation_Delete(transform_ani_fan);
	TransformAction_Delete(transform_act_fan);
	TransformAnimation_Delete(transform_ani_car);
	TransformAction_Delete(transform_act_car_x);
	TransformAction_Delete(transform_act_car_y);
	MaterialAnimation_Delete(mat_ani_sun);
	MaterialAction_Delete(mat_act_fade_in_out);


	ZetgineCore_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif


	return 0;
}
