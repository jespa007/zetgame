#include "zetgame.h"

#define STEP_INC 0.016

void convert_to_displacement2d_x(float *vx, size_t count){
	size_t it=0;

	for(;it<count;){

		vx[it+1]=ViewPort_ScreenToWorldWidth(vx[it+1]);
		it+=2;
	}
}

void convert_to_displacement2d_y(float *vy, size_t count){
	size_t it=0;

	for(;it<count;){

		vy[it+1]=ViewPort_ScreenToWorldHeight(vy[it+1]);
		it+=2;
	}
}

int main(int argc, char *argv[]){

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);
	float scale=1;
	float x=0.2,y=0.1;

	float inc_x=STEP_INC; // [-1 to 1]
	float inc_y=STEP_INC; // [-1 to 1]
	float inc_scale=STEP_INC; // [0.5 to 1.5]

	Texture * text_png = Texture_NewFromFile("../../../test/data/images/test.png");
	Texture * text_jpg = Texture_NewFromFile("../../../test/data/images/test.jpg");
	TransformAnimation 			*trs_animation=TransformAnimation_New();
	TransformAction	    		*act_translate=TransformAction_New();



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

	convert_to_displacement2d_x(tr_values_x,ARRAY_SIZE(tr_values_x));
	convert_to_displacement2d_y(tr_values_y,ARRAY_SIZE(tr_values_y));

	// set key frames x an y
	TransformAction_SetKeyframesTrack(
		act_translate
		,TRANSFORM_CHANNEL_TRANSLATE_X
		,EASE_IN_OUT_SINE
		,tr_values_x
		,ARRAY_SIZE(tr_values_x)
	);

	TransformAction_SetKeyframesTrack(
		act_translate
		,TRANSFORM_CHANNEL_TRANSLATE_Y
		,EASE_IN_OUT_SINE
		,tr_values_y
		,ARRAY_SIZE(tr_values_y)
	);

	TransformAnimation_StartAction(
			trs_animation
			,act_translate
			,SDL_GetTicks()
			,1);

	Appearance *appearance=Appearance_New();
	Appearance_SetColor3i(appearance,1,0,0);
	appearance->texture=text_png;

	Transform transform=Transform_New();


	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));

	do{
		Graphics_BeginRender();

		TransformAnimation_Update(trs_animation,&transform);

		if(K_S){
			TransformAnimation_StartTween(
				  trs_animation
				 , SDL_GetTicks()
				, TRANSFORM_CHANNEL_SCALE_Y
				, EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, 500
				, false
			);

			TransformAnimation_StartTween(
				  trs_animation
				, SDL_GetTicks()
				, TRANSFORM_CHANNEL_SCALE_X
				, EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, 500
				, false
			);

			TransformAnimation_StartTween(
				 trs_animation
				, SDL_GetTicks()
				, TRANSFORM_CHANNEL_ROTATE_Z
				, EASE_LINEAR
				, 0
				, 90
				, 2000
				, false
			);
		}

		Graphics_Draw(&transform,Geometry_GetDefaultRectangleTextured(),appearance);

		x+=inc_x;
		y+=inc_y;
		scale+=inc_scale;

		if(K_LEFT){
			transform.rotate.z-=4;
		}

		if(K_RIGHT){
			transform.rotate.z+=4;
		}


		if(y<-1 || y > 1) inc_y*=-1;
		if(x<-1 || x > 1) inc_x*=-1;
		if(scale< 0.5f || scale > 2.0f) inc_scale*=-1;


		Graphics_DrawRectangleFilled4i(10,10,100,100,Color4f_FromHex(0xFF));

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);

	TransformAnimation_Delete(trs_animation);
	TransformAction_Delete(act_translate);

	//Transform_Delete(transform);


	Texture_Delete(text_png);
	Texture_Delete(text_jpg);
	//TTFont_Delete(font);
	Appearance_Delete(appearance);


	ZetGame_DeInit();

	return 0;
}
