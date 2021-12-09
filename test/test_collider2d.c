#include "ZetGame.h"

void draw_collider(int _x, int _y, int _w, int _h, Collider2dType _collider_type, Color4f _color){
	Transform transform=Transform_New();
	transform.translate.x=ViewPort_ScreenToWorldPositionX(_x);
	transform.translate.y=ViewPort_ScreenToWorldPositionY(_y);
	transform.scale.x=ViewPort_ScreenToWorldWidth(_w);
	transform.scale.y=ViewPort_ScreenToWorldHeight(_h);

	Graphics_SetColor4f(_color.r, _color.g, _color.b, _color.a);

	Transform_Apply(&transform);
	switch(_collider_type){
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

	struct{
		int x,y;
		int w,h;
		Collider2dType collider_type;
	}colliders[]={
		{10,10,10,10,COLLIDER2D_TYPE_RECTANGLE}
		,{40,40,5,5,COLLIDER2D_TYPE_CIRCLE}
	};

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	/*Collider2dType mouse_collider_type=COLLIDER2D_TYPE_RECTANGLE;
	Transform mouse_transform=Transform_New();

	int mouse_collider_w=10;
	int mouse_collider_h=10;
	mouse_transform.scale.x=ViewPort_ScreenToWorldWidth(mouse_collider_w);
	mouse_transform.scale.y=ViewPort_ScreenToWorldWidth(mouse_collider_h);*/

	do{
		Graphics_BeginRender();

		/*Vector2i m=Input_GetMousePosition();
		mouse_transform.translate.x=ViewPort_ScreenToWorldPositionX(m.x);
		mouse_transform.translate.y=ViewPort_ScreenToWorldPositionY(m.y);

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
		);*/

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
