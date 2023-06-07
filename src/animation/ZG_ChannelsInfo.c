#include "_zg_animation_.h"

#define ZG_MAX_CHANNELS 16
//---------------------------------------------------------------------------------------------------------------------
// STATIC



void ZG_ChannelsInfo_Copy(ZG_ChannelsInfo * dst, ZG_ChannelsInfo * src){
	uint32_t msk = src->msk_active_channels;

	for(unsigned i=0; i < dst->n_channels; i++){
		if(msk & (0x1 << i)){
			dst->channels[i]=(src)->channels[i];
		}
	}

	dst->msk_active_channels |= msk;
}

void ZG_ChannelsInfo_CopyToFloatArray(ZG_ChannelsInfo	*_this,float *_dst){
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

ZG_ChannelsInfo * ZG_ChannelsInfo_New(uint8_t n_channels){
	ZG_ChannelsInfo * c = NULL;

	if(n_channels==0){
		ZG_Log_ErrorF("Invalid num components");
		return NULL;
	}

	c = ZG_NEW(ZG_ChannelsInfo);
	memset(c,0,sizeof(ZG_ChannelsInfo));
	c->channels=malloc(n_channels*sizeof(float));
	c->n_channels=n_channels;
	c->msk_active_channels=0;


	return c;
}



void ZG_ChannelsInfo_Delete(ZG_ChannelsInfo * _this){
	_this->n_channels=0;
	_this->msk_active_channels=0;
	free(_this->channels);

	ZG_FREE(_this);

}

