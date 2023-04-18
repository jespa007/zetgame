#include "zg_animation.h"

#define MAX_CHANNELS 16
//---------------------------------------------------------------------------------------------------------------------
// STATIC



void ChannelsInfo_Copy(ChannelsInfo * dst, ChannelsInfo * src){
	uint32_t msk = src->msk_active_channels;

	for(unsigned i=0; i < dst->n_channels; i++){
		if(msk & (0x1 << i)){
			dst->channels[i]=(src)->channels[i];
		}
	}

	dst->msk_active_channels |= msk;
}

void ChannelsInfo_CopyToFloatArray(ChannelsInfo	*_this,float *_dst){
	float *src=_this->channels;

	// Set all components to each transform3d vars...
	for(unsigned j=0; j < _this->n_channels; j++){
		if(_this->msk_active_channels & (0x1<<j)){ // match components...
			*_dst=*src;
		}

		src++;
		_dst++;
	}
}

//---------------------------------------------------------------------------------------------------------------------
// PUBLIC

ChannelsInfo * ChannelsInfo_New(uint8_t n_channels){
	ChannelsInfo * c = NULL;

	if(n_channels==0){
		Log_ErrorF("Invalid num components");
		return NULL;
	}

	c = ZG_NEW(ChannelsInfo);
	memset(c,0,sizeof(ChannelsInfo));
	c->channels=malloc(n_channels*sizeof(float));
	c->n_channels=n_channels;
	c->msk_active_channels=0;


	return c;
}



void ChannelsInfo_Delete(ChannelsInfo * _this){
	_this->n_channels=0;
	_this->msk_active_channels=0;
	free(_this->channels);

	ZG_FREE(_this);

}

