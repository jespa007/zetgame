#ifndef __ZG_CHANNELS_INFO_H__
#define __ZG_CHANNELS_INFO_H__

typedef struct ZG_ChannelsInfo ZG_ChannelsInfo;

struct ZG_ChannelsInfo{
	uint32_t msk_active_channels;
	uint8_t n_channels;
	float *channels;

	struct{
		void (*ptr_function)(ZG_ChannelsInfo * c, void *user_data);
		void *user_data;
	}on_reset_channels;

	struct{
		void (*ptr_function)(ZG_ChannelsInfo * c, void *user_data);
		void *user_data;
	}on_set_channels;

};

//---------------------------------------------------------------------------------------------------------------------
// STATIC

void ZG_ChannelsInfo_Copy(ZG_ChannelsInfo * dst,ZG_ChannelsInfo * src);
void ZG_ChannelsInfo_CopyToFloatArray(ZG_ChannelsInfo	*_this,float *_dst);
//---------------------------------------------------------------------------------------------------------------------
// PUBLIC
ZG_ChannelsInfo * ZG_ChannelsInfo_New(uint8_t n_channels);
void 	ZG_ChannelsInfo_Copy(ZG_ChannelsInfo * dst, ZG_ChannelsInfo * src);
void 	  ZG_ChannelsInfo_Delete(ZG_ChannelsInfo * c);

#endif
