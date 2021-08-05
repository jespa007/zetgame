#ifndef __CHANNELS_INFO_H__
#define __CHANNELS_INFO_H__

typedef struct ChannelsInfo ChannelsInfo;

struct ChannelsInfo{
	uint32_t msk_active_channels;
	uint8_t n_channels;
	float *channels;

	struct{
		void (*ptr_function)(ChannelsInfo * c, void *user_data);
		void *user_data;
	}on_reset_channels;

	struct{
		void (*ptr_function)(ChannelsInfo * c, void *user_data);
		void *user_data;
	}on_set_channels;

};

//---------------------------------------------------------------------------------------------------------------------
// STATIC

void ChannelsInfo_Copy(ChannelsInfo * dst,ChannelsInfo * src);
void ChannelsInfo_CopyToFloatArray(ChannelsInfo	*_this,float *_dst);
//---------------------------------------------------------------------------------------------------------------------
// PUBLIC
ChannelsInfo * ChannelsInfo_New(uint8_t n_channels);
void 	ChannelsInfo_Copy(ChannelsInfo * dst, ChannelsInfo * src);
void 	  ChannelsInfo_Delete(ChannelsInfo * c);

#endif
