#include "ZetGame.h"

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
		, Collider2dType *_mouse_collider_type
		, Vector3f		*_mouse_scale

	){
	Vector2i m=Input_GetMousePosition();
	bool is_pressed=Input_IsLeftButtonPressed();
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
					*_mouse_collider_type=COLLIDER2D_TYPE_POINT;
					break;
				case SELECT_COLLIDER_RECTANGLE_PORTRAIT:
					_mouse_w=80;
					_mouse_h=160;
					*_mouse_collider_type=COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_RECTANGLE_LANDSCAPE:
					_mouse_w=160;
					_mouse_h=80;
					*_mouse_collider_type=COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_CIRCLE:
					_mouse_w=50;
					_mouse_h=50;
					*_mouse_collider_type=COLLIDER2D_TYPE_CIRCLE;
					break;
				}

				_mouse_scale->x=ViewPort_ScreenToWorldWidth(_mouse_w);
				_mouse_scale->y=ViewPort_ScreenToWorldHeight(_mouse_h);

				return;
			}
			x+=BUTTON_GROUP_SIZE;

		}


	}
}


void draw_options(SelectCollider  _selected_collider, const char *_colliding){

	Graphics_Print(10,30,COLOR4F_WHITE,"Selected collider:");
	// draw background rectangles as deactivated
	for(unsigned i=0; i < MAX_SELECT_COLLIDERS; i++){
		Color4f color=COLOR4F_GRAY;
		int thickness=1;
		if(i==_selected_collider){
			color=COLOR4F_WHITE;
			thickness=2;
		}

		// border selection
		Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,BUTTON_GROUP_SIZE,BUTTON_GROUP_SIZE,color,thickness);
		switch(i){
		case SELECT_COLLIDER_POINT: // point
			Graphics_DrawPoint2i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,color,thickness*2);
			break;
		case SELECT_COLLIDER_RECTANGLE_PORTRAIT: // rectangle portrait
			Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,10,40,color,thickness);
			break;
		case SELECT_COLLIDER_RECTANGLE_LANDSCAPE: // rectangle landscape
			Graphics_DrawRectangle4i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,40,10,color,thickness);
			break;
		case SELECT_COLLIDER_CIRCLE: // circle
			Graphics_DrawCircle3i(BUTTON_GROUP_OFFSET_X+i*BUTTON_GROUP_SIZE,BUTTON_GROUP_OFFSET_Y,BUTTON_GROUP_SIZE>>2,color,thickness);
			break;

		}
	}

	Graphics_Print(10,70,COLOR4F_WHITE,"Colliding: %s",_colliding);

}

int main(int argc, char *argv[]){

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	const char *colliding="none";
	struct{
		Transform transform;
		Collider2dType collider_type;
	}colliders[]={
		{
				Transform_New()
				,COLLIDER2D_TYPE_POINT
		}
		,{
				Transform_New()
				,COLLIDER2D_TYPE_RECTANGLE
		}
		,{
				Transform_New()
				,COLLIDER2D_TYPE_CIRCLE
		}
	};

	// init colliders
	// Point
	colliders[0].transform.translate.x=ViewPort_ScreenToWorldPositionX(100);
	colliders[0].transform.translate.y=ViewPort_ScreenToWorldPositionY(300);

	// Quad
	colliders[1].transform.translate.x=ViewPort_ScreenToWorldPositionX(250);
	colliders[1].transform.translate.y=ViewPort_ScreenToWorldPositionY(300);
	colliders[1].transform.scale.x=ViewPort_ScreenToWorldWidth(100);
	colliders[1].transform.scale.y=ViewPort_ScreenToWorldHeight(100);


	// Circle
	colliders[2].transform.translate.x=ViewPort_ScreenToWorldPositionX(500);
	colliders[2].transform.translate.y=ViewPort_ScreenToWorldPositionY(300);
	colliders[2].transform.scale.x=ViewPort_ScreenToWorldWidth(100);




	Collider2dType mouse_collider_type=COLLIDER2D_TYPE_POINT;
	SelectCollider select_collider=SELECT_COLLIDER_POINT;

	Transform mouse_transform=Transform_New();


	do{
		Graphics_BeginRender();

		Vector2i m=Input_GetMousePosition();
		mouse_transform.translate.x=ViewPort_ScreenToWorldPositionX(m.x);
		mouse_transform.translate.y=ViewPort_ScreenToWorldPositionY(m.y);
		colliding="None";

		for(unsigned i=0; i < ARRAY_SIZE(colliders); i++){
			Color4f color=COLOR4F_WHITE;

			if(Collider2d_Test(mouse_transform
					,mouse_collider_type
					,colliders[i].transform
					,colliders[i].collider_type
					)){
				switch(colliders[i].collider_type){
				case COLLIDER2D_TYPE_POINT:
					color=COLOR4F_RED;
					colliding="Point";
					break;
				case COLLIDER2D_TYPE_RECTANGLE:
					color=COLOR4F_RED;
					colliding="Rectangle";
					break;
				case COLLIDER2D_TYPE_CIRCLE:
					color=COLOR4F_RED;
					colliding="Circle";
					break;

				}
			}

			Collider2d_Draw(
					 colliders[i].transform
					,colliders[i].collider_type
					,color
			);

		}

		Collider2d_Draw(
			mouse_transform
			,mouse_collider_type
			,COLOR4F_WHITE
		);


		draw_options(select_collider,colliding);
		update_options(
			&select_collider,
			&mouse_collider_type,
			&mouse_transform.scale
		);

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	ZetGame_DeInit();

	return 0;
}
