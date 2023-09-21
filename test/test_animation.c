#include "zetgame.h"

#define STEP_INC 0.016

void convert_to_displacement2d_x(float *vx, size_t count){
	size_t it=0;

	for(;it<count;){

		vx[it+1]=ZG_ViewPort_ScreenToWorldWidth(vx[it+1]);
		it+=2;
	}
}

void convert_to_displacement2d_y(float *vy, size_t count){
	size_t it=0;

	for(;it<count;){

		vy[it+1]=ZG_ViewPort_ScreenToWorldHeight(vy[it+1]);
		it+=2;
	}
}

int main(int argc, char *argv[]){

	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);
	float scale=1;
	float x=0.2,y=0.1;

	float inc_x=STEP_INC; // [-1 to 1]
	float inc_y=STEP_INC; // [-1 to 1]
	float inc_scale=STEP_INC; // [0.5 to 1.5]

	ZG_Texture 						*text_png = ZG_Texture_NewFromFile("../../../test/data/images/test.png");
	ZG_Texture 						*text_jpg = ZG_Texture_NewFromFile("../../../test/data/images/test.jpg");
	ZG_TransformAnimation 			*trs_animation=ZG_TransformAnimation_New();
	ZG_TransformAction	    		*act_translate=ZG_TransformAction_New();



	float 				tr_values_x[]={
	//  t    x
	//-----+-----
	   0    ,10
	  ,1000 ,100
	  ,2000 ,10

	};

	float 				tr_values_y[]={
		//  t    y
		//-----+----
		   0    ,10
		  ,1000 ,100
		  ,2000 ,10

		};

	convert_to_displacement2d_x(tr_values_x,ZG_ARRAY_SIZE(tr_values_x));
	convert_to_displacement2d_y(tr_values_y,ZG_ARRAY_SIZE(tr_values_y));

	// set key frames x an y
	ZG_TransformAction_SetKeyframesTrack(
		act_translate
		,ZG_TRANSFORM_COMPONENT_TRANSLATE_X
		,ZG_EASE_IN_OUT_SINE
		,tr_values_x
		,ZG_ARRAY_SIZE(tr_values_x)
	);

	ZG_TransformAction_SetKeyframesTrack(
		act_translate
		,ZG_TRANSFORM_COMPONENT_TRANSLATE_Y
		,ZG_EASE_IN_OUT_SINE
		,tr_values_y
		,ZG_ARRAY_SIZE(tr_values_y)
	);

	ZG_TransformAnimation_StartAction(
			trs_animation
			,act_translate
			,SDL_GetTicks()
			,1);

	ZG_Appearance *appearance=ZG_Appearance_New();
	ZG_Appearance_SetColor3i(appearance,1,0,0);
	appearance->texture=text_png;

	ZG_Transform transform=ZG_Transform_New();


	ZG_Graphics_SetBackgroundColor(ZG_Color4f_FromHex(0xFFFF));

	do{
		ZG_Graphics_BeginRender();

		ZG_TransformAnimation_Update(trs_animation,&transform);

		if(ZG_K_S){
			ZG_TransformAnimation_StartTween(
				  trs_animation
				 , SDL_GetTicks()
				, ZG_TRANSFORM_COMPONENT_SCALE_Y
				, ZG_EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, 500
				, false
			);

			ZG_TransformAnimation_StartTween(
				  trs_animation
				, SDL_GetTicks()
				, ZG_TRANSFORM_COMPONENT_SCALE_X
				, ZG_EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, 500
				, false
			);

			ZG_TransformAnimation_StartTween(
				 trs_animation
				, SDL_GetTicks()
				, ZG_TRANSFORM_COMPONENT_ROTATE_Z
				, ZG_EASE_LINEAR
				, 0
				, 90
				, 2000
				, false
			);
		}

		ZG_Graphics_Draw(&transform,ZG_Geometry_GetDefaultTexturedRectangle2d(),appearance);

		x+=inc_x;
		y+=inc_y;
		scale+=inc_scale;

		if(ZG_KP_LEFT){
			transform.rotate.z-=4;
		}

		if(ZG_KP_RIGHT){
			transform.rotate.z+=4;
		}


		if(y<-1 || y > 1) inc_y*=-1;
		if(x<-1 || x > 1) inc_x*=-1;
		if(scale< 0.5f || scale > 2.0f) inc_scale*=-1;


		ZG_Graphics_DrawFilledRectangle4i(10,10,100,100,ZG_Color4f_FromHex(0xFF));

		if(ZG_KP_T){
			ZG_Graphics_ToggleFullscreen();
		}

		ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!ZG_KP_ESC);

	ZG_TransformAnimation_Delete(trs_animation);
	ZG_TransformAction_Delete(act_translate);

	//ZG_Transform_Delete(transform);


	ZG_Texture_Delete(text_png);
	ZG_Texture_Delete(text_jpg);
	//ZG_TTFont_Delete(font);
	ZG_Appearance_Delete(appearance);


	ZG_DeInit();

	return 0;
}
