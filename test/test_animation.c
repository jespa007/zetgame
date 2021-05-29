#include "../ZetGame.h"

#define STEP_INC 0.016

void normalize_from_relative_pixels(float *v, size_t count){
	size_t it=0;

	for(;it<count;){

		v[it+1]=ViewPort_ScreenToWorldWidth(v[it+1]);
		v[it+2]=ViewPort_ScreenToWorldHeight(v[it+2]);


		it+=3;
	}
}

int main(int argc, char *argv[]){

	ZetGame_Init(NULL);
	float rotate=0;
	float scale=1;
	float x=0.2,y=0.1;

	float inc_x=STEP_INC; // [-1 to 1]
	float inc_y=STEP_INC; // [-1 to 1]
	float inc_scale=STEP_INC; // [0.5 to 1.5]

	Texture_SetTextureResourcePath("data/images");

	//Texture * text_default = Texture_GetDefault();
	Texture * text_png = Texture_LoadFromFile("test.png");
	Texture * text_jpg = Texture_LoadFromFile("test.jpg");
	//TTFont * font = TTFont_GetFontFromName("pf_arma_five.ttf",16);
	//TTFont * font1 = TTFont_GetFontFromName("Trebuchet MS.ttf",36);
	Animation 			*trs_animation=Animation_New(TRANSFORM_CHANNEL_MAX);
	Action			   *act_translate=Action_New(TRANSFORM_CHANNEL_MAX);
	Action			   *act_rotate=Action_New(TRANSFORM_CHANNEL_MAX);
	Action			   *act_scale=Action_New(TRANSFORM_CHANNEL_MAX);



	float 				tr_values[]={
			0,10,10
			,1000,100,100
			,2000,10,10

	};

	normalize_from_relative_pixels(tr_values,ARRAY_SIZE(tr_values));

	Action_SetKeyframesTrackGroup(
			act_translate
			,TRANSFORM_CHANNEL_TRANSLATE_X
			,KEYFRAME_TRACK_GROUP_COMPONENT_X | KEYFRAME_TRACK_GROUP_COMPONENT_Y
			,EASE_IN_OUT_SINE
			,tr_values
			,ARRAY_SIZE(tr_values)
	);

	Animation_StartAction(trs_animation,act_translate,SDL_GetTicks(),-1);

	Appearance *appearance=Appearance_New();
	Appearance_SetColor3i(appearance,1,0,0);
	appearance->texture=text_png;

	Transform *transform=Transform_New();


	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));
	//Shape2d_SetTranslate2f(shape2d,0,0);

	do{
		Graphics_BeginRender();

		//Graphics_DrawRectangleTextured(200,200,100,100,(Color4f){1,1,1,1},text_png,NULL);

		if(Animation_Update(trs_animation,SDL_GetTicks())){ // let animation do the move...
			// transfer last values
			Animation_CopyChannelValues(trs_animation,&transform->translate.x);

			//trs_animation
		}else{ // my custom move
			//Shape2d_SetDimensions(shape2d,100,100);
			transform->translate=Vector3f_New3f(x,y,0);
			Transform_SetScale3f(transform,scale,scale,1);
			Transform_SetRotate3f(transform,0,0,rotate+=5);
		}

		if(K_S){
			Animation_StartTween(
				  trs_animation
				, TRANSFORM_CHANNEL_SCALE_Y
				, SDL_GetTicks()
				, 500
				, EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, false
			);

			Animation_StartTween(
				  trs_animation
				, TRANSFORM_CHANNEL_SCALE_X
				, SDL_GetTicks()
				, 500
				, EASE_OUT_SINE
				, 1.0f
				, 1.5f
				, false
			);
		}

		//Shape2d_Draw(shape2d,transform);
		Transform_Apply(transform);
		Shape_DrawRectangle(0.5f,0.5f,true);
		Transform_Restore(transform);


		x+=inc_x;
		y+=inc_y;
		scale+=inc_scale;

		if(K_LEFT){
			rotate-=4;
			printf("angle %f\n",rotate);
		}

		if(K_RIGHT){
			rotate+=4;
			printf("angle %f\n",rotate);
		}




		if(y<-1 || y > 1) inc_y*=-1;
		if(x<-1 || x > 1) inc_x*=-1;
		if(scale< 0.5f || scale > 2.0f) inc_scale*=-1;


		Graphics_DrawRectangleFilled(10,10,100,100,Color4f_FromHex(0xFF));
		/*Graphics_DrawRectangle(20,20,100,100,Color4f_FromHexa(0xFFFF));
		Graphics_DrawRectangleTextured(200,200,100,100,text_png);
		Graphics_DrawRectangleTextured(300,300,100,100,text_jpg);
		Graphics_DrawRectangleTextured(100,200,100,100,text_default);


		//Shape_SetAppearance(v2d->shape2d->shape,appearance); // 10000 sprites at 4FPS
		Appearance_Apply(appearance);// 10000 sprites at 20FPS

		// test max draw sprites ...
		for(int i=0; i<100000;i++){
			shape2d->shape->transform->translate.x=rand()*0.001f;
			shape2d->shape->transform->translate.y=rand()*0.001f;
			Shape2d_Draw(shape2d);
		}

		Appearance_Restore(appearance);*/

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);

	Animation_Delete(trs_animation);
	Action_Delete(act_translate);
	Action_Delete(act_rotate);
	Action_Delete(act_scale);

	Transform_Delete(transform);


	Texture_Delete(text_png);
	Texture_Delete(text_jpg);
	//TTFont_Delete(font);
	Appearance_Delete(appearance);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
