#include "zetgame.h"

void MS_OnDeleteTexture(void *text){
	ZG_Texture_Delete(text);
}

typedef struct{
	ZG_TransformNode *transform_node;
	ZG_Appearance *appearance;
	ZG_Geometry *geometry;
}ZG_TransformGraphicNode;

ZG_TransformNode *ZG_NewTransformNode(
	ZG_List *_transform_nodes
	, int _posx
	, int _posy
	, bool _set_displacement
){
	ZG_TransformNode *transform_node=ZG_TransformNode_New();

	if(_set_displacement){
		ZG_TransformNode_SetDisplacement2i(transform_node,_posx,_posy);
	}else{
		ZG_TransformNode_SetPosition2i(transform_node,_posx,_posy);
	}

	ZG_List_Add(_transform_nodes,transform_node);

	return transform_node;
}

ZG_TransformGraphicNode *ZG_NewTransformGraphicNode(
	ZG_List *_transform_graphic_nodes
	,int _posx
	, int _posy
	, uint16_t _width
	, uint16_t _height
	, ZG_Texture *_texture
	, bool _set_displacement
){
	ZG_TransformGraphicNode *transform_graphic_node=ZG_NEW(ZG_TransformGraphicNode);
	ZG_TransformNode *transform_node=ZG_TransformNode_New();
	ZG_Geometry *geometry=ZG_Geometry_NewRectangle2d();
	ZG_Appearance *appearance=ZG_Appearance_New();

	if(_set_displacement){
		ZG_TransformNode_SetDisplacement2i(transform_node,_posx,_posy);
	}else{
		ZG_TransformNode_SetPosition2i(transform_node,_posx,_posy);
	}

	ZG_Vector3f p=ZG_ViewPort_ScreenToWorldDimension2i(_width>>1,_height>>1);

	float vertexs[ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN]={
			   -p.x,-p.y,0.0f,  // bottom left
			   +p.x,-p.y,0.0f,  // bottom right
			   -p.x,+p.y,0.0f,  // top left
			   +p.x,+p.y,0.0f   // top right

	};

	//.. and set vertex to geometry
   ZG_Geometry_SetMeshVertex(geometry,vertexs,ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN);

   transform_graphic_node->transform_node=transform_node;
   transform_graphic_node->geometry=geometry;
   transform_graphic_node->appearance=appearance;
   transform_graphic_node->appearance->texture=_texture;

	ZG_List_Add(_transform_graphic_nodes,transform_graphic_node);

	return transform_graphic_node;
}

void ZG_TransformGraphicNode_Delete(ZG_TransformGraphicNode *_this){
	ZG_Geometry_Delete(_this->geometry);
	ZG_Appearance_Delete(_this->appearance);
	ZG_TransformNode_Delete(_this->transform_node);
	ZG_FREE(_this);
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

	//----------------------------------------------------------------------------------------------------
	//ZG_SetupParams setup;
	//memset(&setup,0,sizeof(setup));
	//setup.width=640;
	//setup.height=480;
	//setup.graphic_properties=ZG_MSK_GRAPHIC_PROPERTY_FULLSCREEN;
	//ZG_Init(&setup);

	// Initializes zetgame with viewport as 640x480 by default
	ZG_Init(NULL);

	ZG_TextureManager *tm=ZG_TextureManager_New();
	//ZG_TTFontManager *ttfm=ZG_TTFontManager_New();
	ZG_TransformGraphicNode
				*spr_image_sun=NULL
				,*spr_image_car_part1=NULL
				,*spr_image_car_part2=NULL
				,*spr_image_car_left_wheel=NULL
				,*spr_image_car_right_wheel=NULL;
	ZG_TransformNode 		*spr_base_car=NULL,*spr_track_car=NULL;


	//ZG_TTFontMananger_SetFontResourcePath("../../../test/data/fonts");
	ZG_TextureManager_SetTextureResourcePath(tm,"../../../test/data/images");

	ZG_Texture * text_ground=ZG_TextureManager_GetTexture(tm,"ground.png");
	ZG_Texture * text_sun=ZG_TextureManager_GetTexture(tm,"sun.png");
	ZG_Texture * text_vane=ZG_TextureManager_GetTexture(tm,"vane.png");
	ZG_Texture * text_wheel=ZG_TextureManager_GetTexture(tm,"wheel.png");

	// setup animations/actions (update material action function)...
	ZG_MaterialAction 	  	 			*mat_act_fade_in_out=ZG_MaterialAction_New();//ZG_MATERIAL_COMPONENT_MAX);

	ZG_List *transform_nodes=ZG_List_New();
	ZG_List *transform_graphic_nodes=ZG_List_New();
	ZG_List *root_nodes=ZG_List_New();

	//---
	// ground
	/*NewEntityTexture(
		nodes
		,ZG_Graphics_GetWidth()>>1
		,ZG_Graphics_GetHeight()>>1
		,ZG_Graphics_GetWidth()
		, ZG_Graphics_GetHeight()
		,text_ground
		,false
	);*/

	//----------------------------------
	// SETUP FAN...
	for(unsigned i=0; i < ZG_ARRAY_SIZE(fan_info); i++){
		_fan_info *info=&fan_info[i];
		ZG_TransformGraphicNode *spr_image_fan_base=NULL;
		ZG_TransformNode *spr_base_van=NULL;

		spr_image_fan_base=ZG_NewTransformGraphicNode(
			transform_graphic_nodes
			,info->x
			,info->y
			,info->w
			,info->h
			,NULL
			,false
		);

		// root node of fan
		ZG_List_Add(root_nodes,spr_image_fan_base->transform_node);


		// van base
		spr_base_van=ZG_NewTransformNode(
				transform_nodes
				,info->vane_disp.x
				,info->vane_disp.y
				,true
		);

		ZG_TransformNode_Attach(
			spr_image_fan_base->transform_node
			,spr_base_van
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
			ZG_TransformGraphicNode *spr_image_van=ZG_NewTransformGraphicNode(
				transform_graphic_nodes
				,info->vane_disp.info_vane[j].x
				,info->vane_disp.info_vane[j].y
				,62
				,9
				,text_vane
				,true
			);

			ZG_TransformNode_SetRotate3f(spr_image_van->transform_node,0,0,info->vane_disp.info_vane[j].rot);
			ZG_TransformNode_Attach(spr_base_van,spr_image_van->transform_node);

		}
	}
	// SETUP ENTITIES WITHOUT PRE CREATION ?

	//----
	// SETUP CAR
	spr_base_car=ZG_NewTransformNode(transform_nodes,ZG_Graphics_GetWidth()>>1,ZG_Graphics_GetHeight()-150,false); // --> empty entity without id ? It can be but then it cannot be referenced
	spr_track_car=ZG_NewTransformNode(transform_nodes,0,0,true);
	spr_image_car_part1=ZG_NewTransformGraphicNode(transform_graphic_nodes,car_info.part1.x,car_info.part1.y,car_info.part1.w,car_info.part1.h,NULL,true);
	spr_image_car_part2=ZG_NewTransformGraphicNode(transform_graphic_nodes,car_info.part2.x,car_info.part2.y,car_info.part2.w,car_info.part2.h,NULL,true);
	spr_image_car_left_wheel=ZG_NewTransformGraphicNode(transform_graphic_nodes,car_info.wheel[0].x,car_info.wheel[0].y,car_info.wheel[0].w,car_info.wheel[0].h,text_wheel,true);
	spr_image_car_right_wheel=ZG_NewTransformGraphicNode(transform_graphic_nodes,car_info.wheel[1].x,car_info.wheel[1].y,car_info.wheel[1].w,car_info.wheel[1].h,text_wheel,true);

	// root node of fan
	ZG_List_Add(root_nodes,spr_base_car);


	ZG_TransformNode_Attach(spr_base_car,spr_track_car);
	ZG_TransformNode_Attach(spr_track_car,spr_image_car_part1->transform_node);
	ZG_TransformNode_Attach(spr_track_car,spr_image_car_part2->transform_node);
	ZG_TransformNode_Attach(spr_track_car,spr_image_car_left_wheel->transform_node);
	ZG_TransformNode_Attach(spr_track_car,spr_image_car_right_wheel->transform_node);

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
	spr_image_sun=ZG_NewTransformGraphicNode(transform_graphic_nodes,ZG_Graphics_GetWidth()-200,100,100,100,text_sun,false);
	ZG_Material_SetAlpha(spr_image_sun->appearance->material,ZG_ALPHA_VALUE_TRANSPARENT);

	// ani
	/*MaterialAction_SetKeyframesTrack(
			 mat_act_fade_in_out
			,ZG_EASE_IN_OUT_SINE
			,alpha_fade_in_out_keyframes
			,ZG_ARRAY_SIZE(alpha_fade_in_out_keyframes)
	);*/

	ZG_Graphics_SetBackgroundColor(ZG_Color4f_FromHex(0xFFFF));


	//ZG_Transform transform_camera=ZG_Transform_DefaultValues();
	do{

		ZG_Input_Update();

		// input management
		if(ZG_KP_F9){
			ZG_Graphics_ToggleFullscreen();
		}

		if(ZG_Input_IsLeftButtonPressed()){
			printf("Mouse coordinates: %i %i\n",ZG_Input_GetMousePositionPtr()->x, ZG_Input_GetMousePositionPtr()->y);
		}

		ZG_Graphics_BeginRender();

		// draw background
		ZG_Graphics_DrawTexturedRectangle4i(
			(ZG_Graphics_GetWidth()>>1)
			,(ZG_Graphics_GetHeight()>>1)
			,ZG_Graphics_GetWidth()
			,ZG_Graphics_GetHeight()
			,ZG_COLOR4F_WHITE
			,text_ground
			,NULL
		);

		for(size_t i=0; i < root_nodes->count;i++){
			ZG_TransformNode_Update(root_nodes->items[i]);
		}

		for(size_t i=0; i < transform_graphic_nodes->count; i++){
			ZG_TransformGraphicNode *transform_graphic_node=transform_graphic_nodes->items[i];
			ZG_Graphics_Draw(
				ZG_TransformNode_GetTransform(transform_graphic_node->transform_node,ZG_TRANSFORM_NODE_TYPE_WORLD)
				,transform_graphic_node->geometry
				,transform_graphic_node->appearance
			);
		}

		/*if(K_SPACE){
			// todo start action from TextureNode_StartMaterialAction
			TextureNode_StartMaterialAction(
					spr_image_sun
					,mat_act_fade_in_out
					,0);
		}*/


		ZG_Graphics_EndRender();


	}while(!ZG_KP_ESC);

	// delete all nodes
	for(size_t i=0; i < transform_nodes->count;i++){
		ZG_TransformNode_Delete(transform_nodes->items[i]);
	}

	for(size_t i=0; i < transform_graphic_nodes->count;i++){
		ZG_TransformGraphicNode_Delete(transform_graphic_nodes->items[i]);
	}

	ZG_List_Delete(transform_nodes);
	ZG_List_Delete(transform_graphic_nodes);
	ZG_List_Delete(root_nodes);


	ZG_TextureManager_Delete(tm);

	//ZG_TTFontManager_Delete(ttfm);
	ZG_MaterialAction_Delete(mat_act_fade_in_out);


	ZG_DeInit();

	return 0;
}
