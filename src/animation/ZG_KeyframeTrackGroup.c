#include "zg_animation.h"

bool ZG_KeyframeTrackGroup_DecomposeXYZW(

		   const float * v_input
		,  size_t v_input_len
		, ZG_KeyframeTrackGroup * _vx
		, ZG_KeyframeTrackGroup * _vy
		, ZG_KeyframeTrackGroup * _vz
		, ZG_KeyframeTrackGroup * _vw
		){

	// check keyframe_tracks. Possible combinations are:
	// 1. xy
	// 2. xy
	// 3. xyz
	// 4. xyzw
	ZG_KeyframeTrackGroup *vx=NULL,*vy=NULL,*vz=NULL,*vw=NULL;

	ZG_KeyframeTrackGroup * di_src[]={
			_vx,_vy,_vz,_vw
	};

	ZG_KeyframeTrackGroup ** di_dst[]={
			&vx,&vy,&vz,&vw
	};

	ZG_KeyframeTrackGroup ***di_dst_ptr = di_dst;

	for(unsigned i=0; i < ZG_ARRAY_SIZE(di_src);i++){
		if(di_src[i] != NULL){ // assign
			**di_dst_ptr++=di_src[i];
		}
	}


	if(    vx!=NULL
		&& vy==NULL
		&& vz==NULL
		&& vw==NULL
	) {
		int packed_channels=2;

		if((v_input_len % packed_channels)!=0){
			ZG_Log_ErrorF("Error invalid dimension 1!");
			return false;
		}

		size_t dim=(v_input_len/packed_channels)*2;

		vx->keyframe_points=malloc(sizeof(float)*dim);
		vx->keyframe_points_count=v_input_len;

		float *aux_ptr=vx->keyframe_points;

		for(unsigned i = 0; i < v_input_len; i+=packed_channels){
			//---
			// t
			*aux_ptr++=*v_input++;

			//---
			// x
			*aux_ptr++=*v_input++;
		}
	}else if(vx!=NULL // dimension 2
			&& vy!=NULL
			&& vz==NULL
			&& vw==NULL){
		int packed_channels=3;

		if((v_input_len % packed_channels)!=0){
			ZG_Log_ErrorF("Error invalid dimension 2!");
			return false;
		}

		size_t dim=(v_input_len/packed_channels)*2; // *2 because is the t and v

		vx->keyframe_points=malloc(sizeof(float)*dim);
		vx->keyframe_points_count=dim;

		vy->keyframe_points=malloc(sizeof(float)*dim);
		vy->keyframe_points_count=dim;

		float * vx_aux=vx->keyframe_points;
		float * vy_aux=vy->keyframe_points;

		for(unsigned i = 0; i < v_input_len; i+=packed_channels){
			//---
			// t
			*vx_aux++=*vy_aux++=*v_input++;

			//---
			// x
			*vx_aux++=*v_input++;

			//---
			// y
			*vy_aux++=*v_input++;
		}
	}
	else if(vx!=NULL
			&& vy!=NULL
			&& vz!=NULL
			&& vw==NULL){

		int packed_channels=4;

		if((v_input_len % packed_channels)!=0){
			ZG_Log_ErrorF("Error invalid dimension 3!");
			return false;
		}

		size_t dim=(v_input_len/packed_channels)*2;

		vx->keyframe_points=malloc(sizeof(float)*dim);
		vx->keyframe_points_count=dim;

		vy->keyframe_points=malloc(sizeof(float)*dim);
		vy->keyframe_points_count=dim;

		vz->keyframe_points=malloc(sizeof(float)*dim);
		vz->keyframe_points_count=dim;


		float * vx_aux=vx->keyframe_points;
		float * vy_aux=vy->keyframe_points;
		float * vz_aux=vz->keyframe_points;

		for(unsigned i = 0; i < v_input_len; i+=packed_channels){
			//---
			// t
			*vx_aux++=*vy_aux++=*v_input++;
			//---
			// x
			*vx_aux++=*v_input++;
			//---
			// y
			*vy_aux++=*v_input++;
			//----
			// z
			*vz_aux++=*v_input++;
		}
	}
	else{

		int packed_channels=5;

		if((v_input_len % packed_channels)!=0){
			ZG_Log_ErrorF("Error invalid dimension 5!");
			return false;
		}

		size_t dim=(v_input_len/packed_channels)*2;

		vx->keyframe_points=malloc(sizeof(float)*dim);
		vx->keyframe_points_count=dim;

		vy->keyframe_points=malloc(sizeof(float)*dim);
		vy->keyframe_points_count=dim;

		vz->keyframe_points=malloc(sizeof(float)*dim);
		vz->keyframe_points_count=dim;

		vw->keyframe_points=malloc(sizeof(float)*dim);
		vw->keyframe_points_count=dim;


		float * vx_aux=vx->keyframe_points;
		float * vy_aux=vy->keyframe_points;
		float * vz_aux=vz->keyframe_points;
		float * vw_aux=vw->keyframe_points;

		for(unsigned i = 0; i < v_input_len; i+=packed_channels){
			//---
			// t
			*vx_aux++=*vy_aux++=*v_input++;
			//---
			// x
			*vx_aux++=*v_input++;
			//---
			// y
			*vy_aux++=*v_input++;
			//---
			// z
			*vz_aux++=*v_input++;
			//---
			// w
			*vw_aux++=*v_input++;
		}
	}

	return true;
}

void ZG_KeyframeTrackGroup_FreeXYZW(ZG_KeyframeTrackGroup ** vx
		, ZG_KeyframeTrackGroup ** vy
		, ZG_KeyframeTrackGroup ** vz
		, ZG_KeyframeTrackGroup ** vw
){

	ZG_KeyframeTrackGroup **lst_decompose_info[]={
			vx,vy,vz,vw
	};

	for(unsigned i = 0; i < ZG_ARRAY_SIZE(lst_decompose_info); i++){
		 ZG_KeyframeTrackGroup ** v=lst_decompose_info[i];

		if(v!=NULL && *v !=NULL){
			if((*v)->keyframe_points!=NULL){
				ZG_FREE((*v)->keyframe_points);
			}
			ZG_FREE(*v);
			*v=NULL;
		}
	}

}

//----
ZG_KeyframeTrackGroup *ZG_KeyframeTrackGroup_New(){
	ZG_KeyframeTrackGroup *cli=ZG_NEW(ZG_KeyframeTrackGroup);
	memset(cli,0,sizeof(ZG_KeyframeTrackGroup));
	return cli;
}

void ZG_KeyframeTrackGroup_Delete(ZG_KeyframeTrackGroup *_this){
	if(_this == NULL) return;

	if(_this->keyframe_points!=NULL){
		free(_this->keyframe_points);
	}
	ZG_FREE(_this);
}
