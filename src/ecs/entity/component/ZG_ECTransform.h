#ifndef _ZG_ECTRANSFORM_H__
#define _ZG_ECTRANSFORM_H__

typedef struct ZG_ECTransform 	ZG_ECTransform;

struct ZG_ECTransform{
	ZG_Transform				transform; // world coordinates
	ZG_Quaternion				quaternion;
	int 						ac_idx_transform_animation; // in case entity it has associated transform animation component
};

//------------------------------------------------
// Component create/update/destroy
void			ZG_ECTransform_OnCreate(void *_this);
void 	 		ZG_ECTransform_OnDestroy(void *_this);
//------------------------------------------------



#endif
