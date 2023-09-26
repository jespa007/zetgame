#include "zetgame.h"

#define BUTTON_GROUP_SIZE	 	50
#define BUTTON_GROUP_OFFSET_X	200
#define BUTTON_GROUP_OFFSET_Y	40

typedef enum{
	SELECT_COLLIDER_POINT=0,
	SELECT_COLLIDER_RECTANGLE_PORTRAIT,
	SELECT_COLLIDER_RECTANGLE_LANDSCAPE,
	SELECT_COLLIDER_CIRCLE,
	MAX_SELECT_COLLIDERS
}SelectCollider;

void update_options(
		SelectCollider *_selected_collider
		, ZG_Collider2dType *_mouse_collider_type
		, ZG_Vector3f		*_mouse_scale

	){
	ZG_Vector2i m=ZG_Input_GetMousePosition();
	bool is_pressed=ZG_Input_IsLeftButtonPressed();
	int x=BUTTON_GROUP_OFFSET_X-(BUTTON_GROUP_SIZE>>1); // because draws are centered we have to start at -middle
	int y=BUTTON_GROUP_OFFSET_Y-(BUTTON_GROUP_SIZE>>1); // because draws are centered we have to start at -middle


	if(is_pressed && (y <= m.y && m.y <= (y+BUTTON_GROUP_SIZE))){
		for(unsigned i=0; i < MAX_SELECT_COLLIDERS; i++){
			if(x <= m.x && m.x <= (x+BUTTON_GROUP_SIZE)){
				printf("selected collider '%i'\n",i);
				int _mouse_w=0;
				int _mouse_h=0;
				*_selected_collider=i;

				switch(i){
				case SELECT_COLLIDER_POINT:
					*_mouse_collider_type=ZG_COLLIDER2D_TYPE_POINT;
					break;
				case SELECT_COLLIDER_RECTANGLE_PORTRAIT:
					_mouse_w=80;
					_mouse_h=160;
					*_mouse_collider_type=ZG_COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_RECTANGLE_LANDSCAPE:
					_mouse_w=160;
					_mouse_h=80;
					*_mouse_collider_type=ZG_COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_CIRCLE:
					_mouse_w=50;
					_mouse_h=50;
					*_mouse_collider_type=ZG_COLLIDER2D_TYPE_CIRCLE;
					break;
				}

				_mouse_scale->x=ZG_ViewPort_ScreenToWorldWidth(_mouse_w);
				_mouse_scale->y=ZG_ViewPort_ScreenToWorldHeight(_mouse_h);

				return;
			}
			x+=BUTTON_GROUP_SIZE;

		}


	}
}


void draw_options(SelectCollider  _selected_collider, const char *_colliding){

	ZG_Graphics_Print(10,30,ZG_COLOR4F_WHITE,"Selected collider:");
	// draw background rectangles as deactivated
	for(unsigned i=0; i < MAX_SELECT_COLLIDERS; i++){
		ZG_Color4f color=ZG_COLOR4F_GRAY;
		int thickness=1;
		if(i==_selected_collider){
			color=ZG_COLOR4F_WHITE;
			thickness=2;
		}

		// border selection
		ZG_Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,BUTTON_GROUP_SIZE,BUTTON_GROUP_SIZE,color,thickness);
		switch(i){
		case SELECT_COLLIDER_POINT: // point
			ZG_Graphics_DrawPoint2i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,color,thickness*2);
			break;
		case SELECT_COLLIDER_RECTANGLE_PORTRAIT: // rectangle portrait
			ZG_Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,10,40,color,thickness);
			break;
		case SELECT_COLLIDER_RECTANGLE_LANDSCAPE: // rectangle landscape
			ZG_Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,40,10,color,thickness);
			break;
		case SELECT_COLLIDER_CIRCLE: // circle
			ZG_Graphics_DrawCircle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,40,40,color,thickness);
			break;

		}
	}

	ZG_Graphics_Print(10,70,ZG_COLOR4F_WHITE,"Colliding: %s",_colliding);

}

int main(int argc, char *argv[]){

	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL/*&(ZG_SetupParams){
			.graphics_api=ZG_GRAPHICS_API_GL
			,.width=640
			,.height=640
			,.wcaption_title="zetgame"
			,.wposx=SDL_WINDOWPOS_CENTERED
			,.wposy=SDL_WINDOWPOS_CENTERED
			,.graphic_properties=0
	}*/);

	const char *colliding="none";
	struct{
		ZG_Transform transform;
		ZG_Collider2dType collider_type;
	}colliders[]={
		{
				ZG_Transform_New()
				,ZG_COLLIDER2D_TYPE_POINT
		}
		,{
				ZG_Transform_New()
				,ZG_COLLIDER2D_TYPE_RECTANGLE
		}
		,{
				ZG_Transform_New()
				,ZG_COLLIDER2D_TYPE_CIRCLE
		}
	};

	// init colliders
	// Point
	colliders[0].transform.translate.x=ZG_ViewPort_ScreenToWorldPositionX(100);
	colliders[0].transform.translate.y=ZG_ViewPort_ScreenToWorldPositionY(300);

	// Quad
	colliders[1].transform.translate.x=ZG_ViewPort_ScreenToWorldPositionX(250);
	colliders[1].transform.translate.y=ZG_ViewPort_ScreenToWorldPositionY(300);
	colliders[1].transform.scale.x=ZG_ViewPort_ScreenToWorldWidth(100);
	colliders[1].transform.scale.y=ZG_ViewPort_ScreenToWorldHeight(100);


	// Circle
	colliders[2].transform.translate.x=ZG_ViewPort_ScreenToWorldPositionX(500);
	colliders[2].transform.translate.y=ZG_ViewPort_ScreenToWorldPositionY(300);
	colliders[2].transform.scale.x=ZG_ViewPort_ScreenToWorldWidth(100);
	colliders[2].transform.scale.y=ZG_ViewPort_ScreenToWorldHeight(100);


	ZG_Collider2dType mouse_collider_type=ZG_COLLIDER2D_TYPE_POINT;
	SelectCollider select_collider=SELECT_COLLIDER_POINT;

	ZG_Transform mouse_transform=ZG_Transform_New();


	do{
		ZG_Graphics_BeginRender();

		ZG_Vector2i m=ZG_Input_GetMousePosition();
		mouse_transform.translate.x=ZG_ViewPort_ScreenToWorldPositionX(m.x);
		mouse_transform.translate.y=ZG_ViewPort_ScreenToWorldPositionY(m.y);
		colliding="None";

		for(unsigned i=0; i < ZG_ARRAY_SIZE(colliders); i++){
			ZG_Color4f color=ZG_COLOR4F_WHITE;

			if(ZG_Collider2d_Test(mouse_transform
					,mouse_collider_type
					,colliders[i].transform
					,colliders[i].collider_type
					)){
				switch(colliders[i].collider_type){
				case ZG_COLLIDER2D_TYPE_POINT:
					color=ZG_COLOR4F_RED;
					colliding="Point";
					break;
				case ZG_COLLIDER2D_TYPE_RECTANGLE:
					color=ZG_COLOR4F_RED;
					colliding="Rectangle";
					break;
				case ZG_COLLIDER2D_TYPE_CIRCLE:
					color=ZG_COLOR4F_RED;
					colliding="Circle";
					break;

				}
			}

			ZG_Collider2d_Draw(
					 colliders[i].transform
					,colliders[i].collider_type
					,color
			);

		}

		ZG_Collider2d_Draw(
			mouse_transform
			,mouse_collider_type
			,ZG_COLOR4F_WHITE
		);


		draw_options(select_collider,colliding);
		update_options(
			&select_collider,
			&mouse_collider_type,
			&mouse_transform.scale
		);

		if(ZG_KP_T){
			ZG_Graphics_ToggleFullscreen();
		}

		ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!ZG_KP_ESC);


	ZG_DeInit();

	return 0;
}
