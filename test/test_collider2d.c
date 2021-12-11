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
		, Collider2dType *_mouse_collider
		, int *_mouse_w
		, int *_mouse_h
	){
	Vector2i m=Input_GetMousePosition();
	bool is_pressed=Input_IsLeftButtonPressed();
	int x=BUTTON_GROUP_OFFSET_X-(BUTTON_GROUP_SIZE>>1); // because draws are centered we have to start at -middle
	int y=BUTTON_GROUP_OFFSET_Y-(BUTTON_GROUP_SIZE>>1); // because draws are centered we have to start at -middle


	if(is_pressed && (y <= m.y && m.y <= (y+BUTTON_GROUP_SIZE))){
		for(unsigned i=0; i < MAX_SELECT_COLLIDERS; i++){
			if(x <= m.x && m.x <= (x+BUTTON_GROUP_SIZE)){
				printf("selected collider '%i'\n",i);
				*_selected_collider=i;

				switch(i){
				case SELECT_COLLIDER_POINT:
					*_mouse_collider=COLLIDER2D_TYPE_POINT;
					break;
				case SELECT_COLLIDER_RECTANGLE_PORTRAIT:
					*_mouse_w=80;
					*_mouse_h=280;
					*_mouse_collider=COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_RECTANGLE_LANDSCAPE:
					*_mouse_w=280;
					*_mouse_h=80;
					*_mouse_collider=COLLIDER2D_TYPE_RECTANGLE;
					break;
				case SELECT_COLLIDER_CIRCLE:
					*_mouse_w=80;
					*_mouse_h=80;
					*_mouse_collider=COLLIDER2D_TYPE_CIRCLE;
					break;
				}
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

void draw_collider(int _x, int _y, int _w, int _h, Collider2dType _collider_type, Color4f _color){
	Transform transform=Transform_New();
	transform.translate.x=ViewPort_ScreenToWorldPositionX(_x);
	transform.translate.y=ViewPort_ScreenToWorldPositionY(_y);
	transform.scale.x=ViewPort_ScreenToWorldWidth(_w);
	transform.scale.y=ViewPort_ScreenToWorldHeight(_h);

	Graphics_SetColor4f(_color.r, _color.g, _color.b, _color.a);

	Transform_Apply(&transform);
	switch(_collider_type){
	case COLLIDER2D_TYPE_POINT:
		Geometry_Draw(Geometry_GetDefaultPoint());
		break;
	case COLLIDER2D_TYPE_RECTANGLE:
		Geometry_Draw(Geometry_GetDefaultRectangle());
		break;
	case COLLIDER2D_TYPE_CIRCLE:
		Geometry_Draw(Geometry_GetDefaultCircle());
		break;
	}
	Transform_Restore(&transform);
}

int main(int argc, char *argv[]){

	const char *colliding="none";
	struct{
		int x,y;
		int w,h;
		Collider2dType collider_type;
	}colliders[]={
		{100,100,100,100,COLLIDER2D_TYPE_RECTANGLE}
		,{400,400,50,50,COLLIDER2D_TYPE_CIRCLE}
	};

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	Collider2dType mouse_collider_type=COLLIDER2D_TYPE_POINT;
	SelectCollider select_collider=SELECT_COLLIDER_POINT;

	Transform mouse_transform=Transform_New();

	int mouse_collider_w=100;
	int mouse_collider_h=100;
	mouse_transform.scale.x=ViewPort_ScreenToWorldWidth(mouse_collider_w);
	mouse_transform.scale.y=ViewPort_ScreenToWorldWidth(mouse_collider_h);

	do{
		Graphics_BeginRender();

		Vector2i m=Input_GetMousePosition();
		mouse_transform.translate.x=ViewPort_ScreenToWorldPositionX(m.x-10);
		mouse_transform.translate.y=ViewPort_ScreenToWorldPositionY(m.y-10);

		for(unsigned i=0; i < ARRAY_SIZE(colliders); i++){
			Color4f color=COLOR4F_WHITE;

			// check collision and set line red if collides
			switch(mouse_collider_type){
			case COLLIDER2D_TYPE_RECTANGLE:
				switch(colliders[i].collider_type){
				case COLLIDER2D_TYPE_RECTANGLE:
					if(Collider2d_TestIntersectionRectangleRectangle(
							mouse_transform.translate
							,mouse_transform.scale.x
							,mouse_transform.scale.y
							,Vector3f_New3f(ViewPort_ScreenToWorldPositionX(colliders[i].x),ViewPort_ScreenToWorldPositionY(colliders[i].y),0)
							,ViewPort_ScreenToWorldWidth(colliders[i].w)
							,ViewPort_ScreenToWorldHeight(colliders[i].h)
					)){
						color=COLOR4F_RED;
					}
					break;
				case COLLIDER2D_TYPE_CIRCLE:
					if(Collider2d_TestIntersectionRectangleCircle(
							mouse_transform.translate
							,mouse_transform.scale.x
							,mouse_transform.scale.y
							,Vector3f_New3f(ViewPort_ScreenToWorldPositionX(colliders[i].x),ViewPort_ScreenToWorldPositionY(colliders[i].y),0)
							,ViewPort_ScreenToWorldWidth(colliders[i].w)
					)){
						color=COLOR4F_RED;
					}
					break;
				default:
					break;
				}
				break;
			case COLLIDER2D_TYPE_POINT:
				break;
			case COLLIDER2D_TYPE_CIRCLE:
				break;
			default:
				break;


			}

			draw_collider(
					colliders[i].x
					,colliders[i].y
					,colliders[i].w
					,colliders[i].h
					,colliders[i].collider_type
					,color
			);

		}

		draw_collider(
			m.x
			,m.y
			,mouse_collider_w
			,mouse_collider_h
			,mouse_collider_type
			,COLOR4F_WHITE
		);


		draw_options(select_collider,colliding);
		update_options(
			&select_collider,
			&mouse_collider_type,
			&mouse_collider_w,
			&mouse_collider_h
		);

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
