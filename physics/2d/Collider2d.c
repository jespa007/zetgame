#include "physics/zg_physics.h"

typedef struct{
	Vector2f dim;
	Collider2dType type;
}Collider2dData;

static bool Collider2d_CheckCollisionRectangleRectangle(
		  Vector3f _position_1
		, Vector2f bounding_rect_1
		, Vector3f _position_2
		, Vector2f bounding_rect_2
){

	if (_position_1.x + bounding_rect_1.x < _position_2.x - bounding_rect_2.x) {
		return false;
	}
	if (_position_1.y + bounding_rect_1.y < _position_2.y - bounding_rect_2.y) {
		return false;
	}
	if (_position_1.x - bounding_rect_1.x > _position_2.x + bounding_rect_2.x) {
		return false;
	}
	if (_position_1.y - bounding_rect_1.y > _position_2.y + bounding_rect_2.y) {
		return false;
	}
	return true;
}

static bool Collider2d_CheckCollisionRectangleCircle(
		  Vector3f _position_1
		, Vector2f bounding_rect_1
		, Vector3f _position_2
		, float diameter_2
){
	return false;
}

static bool Collider2d_CheckCollisionCircleCircle(
		  Vector3f _position_1
		, float _diameter_1
		, Vector3f _position_2
		, float _diameter_2

){
	return false;
}

bool Collider2d_CheckCollision(
		  Transform *_absolute_pos_coll1
		, Collider2d *coll1
		, Transform *_absolute_pos_coll2
		, Collider2d *coll2
){

	// check collision about it transform and it bounding rectangle (the transform will convert it bounding rectangle)

	return false;
}


Collider2d * Collider2d_NewRectangle(uint16_t _width, uint16_t _height){
	Collider2d *collider=NEW(Collider2d);
	Collider2dData *data=NEW(Collider2dData);
	data->type=COLLIDER2D_TYPE_RECTANGLE;
	data->dim.x=ViewPort_ScreenToWorldWidth(_width);
	data->dim.y=ViewPort_ScreenToWorldHeight(_height);
	collider->data=data;
	return collider;
}

Collider2d * Collider2d_NewCircle(uint16_t diameter){
	Collider2d *collider=NEW(Collider2d);
	Collider2dData *data=NEW(Collider2dData);
	data->type=COLLIDER2D_TYPE_CIRCLE;
	data->dim.x=ViewPort_ScreenToWorldWidth(diameter);
	data->dim.y=ViewPort_ScreenToWorldHeight(diameter);

	collider->data=data;
	return collider;
}

void Physics2dCollider_Delete(Collider2d * _this){
	FREE(_this);
}

