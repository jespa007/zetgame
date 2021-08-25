#include "zg_sprite.h"

#include "SpriteKeyFramePack.c"

typedef struct{
	MapString 	* 	sprite_keyframes;
	List 	* 	sprite_keyframe_packs;
}SpriteKeyFrameManagerData;

// STATIC
/*void	* SpriteKeyFrameManager_OnDeleteNode(MapStringNode *node){
	SpriteKeyFramePack * keyframepack = node->val;
	if(keyframepack!=NULL){
		Texture_Delete(keyframepack->texture);

		for(unsigned i=0; i < keyframepack->sprite_keyframes_len; i++){
			SpriteKeyFrame_Delete(keyframepack->sprite_keyframes[i]);
		}

		FREE(keyframepack);

	}
	return NULL;
}*/



// MEMBERS
SpriteKeyFrameManager *SpriteKeyFrameManager_New(void){
	SpriteKeyFrameManager *skfm=NEW(SpriteKeyFrameManager);
	SpriteKeyFrameManagerData *data=NEW(SpriteKeyFrameManagerData);

	data->sprite_keyframes = MapString_New();//new std::map<std::string,TTFont *>();

	data->sprite_keyframe_packs= List_New();
	//data->sprite_keyframes->on_delete=SpriteKeyFrameManager_OnDeleteNode;


	skfm->data=data;

	return skfm;
}

bool SpriteKeyFrameManager_LoadFromMemory(
		SpriteKeyFrameManager *_this
		,const char *key_id_prefix
		,uint8_t *buf_texture
		,size_t buf_texture_len
		,uint8_t * buf_json
		,size_t buf_json_len
		){

	SpriteKeyFrameManagerData *data=_this->data;

	// 1. read & parse json
	if(key_id_prefix == NULL){
		Log_Error("key_id_prefix id NULL");
		return false;
	}

	if(buf_texture && buf_json && buf_texture_len && buf_json_len){

		cJSON * root = cJSON_ParseWithLength((char *)buf_json,buf_json_len);
		cJSON * frames = cJSON_GetObjectItem(root,"frames");
		cJSON * meta = cJSON_GetObjectItem(root,"meta");
		cJSON * layers=NULL;


		if(frames == NULL){
			Log_Error("JsonParse '%s' : Cannot get 'frames' identifier in json",key_id_prefix);
			return false;
		}

		if(meta == NULL){
			Log_Error("JsonParse '%s' : Cannot get 'meta' identifier in json",key_id_prefix);
			return false;
		}

		layers = cJSON_GetObjectItem(meta,"layers");

		if(layers == NULL){
			Log_Error("JsonParse '%s' : Cannot get 'meta.layers' identifier in json",key_id_prefix);
			return false;
		}

		// everything ok, so we can proceed with loading
		Texture *texture=Texture_NewFromMemory(buf_texture,buf_texture_len);
		if(texture){
			// create sprite frame packs and add to list/map
			SpriteKeyFramePack *skp=SpriteKeyFramePack_New(cJSON_GetArraySize(layers),texture);
			List_Add(data->sprite_keyframe_packs,skp);

			char sprite_keyframe_key[150];

			skp->texture=texture;


			cJSON *layer = NULL;
			cJSON *layer_element = NULL;
			Log_Info("Loading '%s': %i layers detected",key_id_prefix,skp->sprite_keyframes_len);
			unsigned i=0;

			cJSON_ArrayForEach(layer, layers) {
			    /* Each element is an object with unknown field(s) */
				layer_element = cJSON_GetObjectItem(layer,"name");

				if(layer_element == NULL){
					Log_Error("JsonParse '%s': Cannot get 'name' attribute in 'layers'",key_id_prefix);
					return false;
				}

				sprintf(sprite_keyframe_key,"%s_%s",key_id_prefix,layer_element->valuestring);

				if(strlen(layer_element->valuestring) >= MAX_SPRITE_KEYFRAME_NAME){
					Log_Error("JsonParse '%s' name '%s': length is greater that %i",key_id_prefix,layer_element->valuestring,MAX_SPRITE_KEYFRAME_NAME);
					return false;
				}

				skp->sprite_keyframes[i].texture=skp->texture;
				strcpy(skp->sprite_keyframes[i].name,layer_element->valuestring);

				MapString_SetValue(data->sprite_keyframes,sprite_keyframe_key,&skp->sprite_keyframes[i]);

				i++;

			  }

			// try to get the frames...


			/*cJSON_ArrayForEach(frame, frames) {
			    // Each element is an object with unknown field(s)

			    cJSON *elem;
			    cJSON_ArrayForEach(elem, frame) {

			      //printf("Found key '%s', set to %d\n", elem->string, elem->valueint);
			    }


			  }*/

			// print load status
			for(unsigned i=0; i < skp->sprite_keyframes_len; i++){
				char key_string[150];
				cJSON *frame_element = NULL;
				unsigned nframes=0;
				for(;;){ // get total frames
					sprintf(key_string,"%s (%s) %i.ase",key_id_prefix,skp->sprite_keyframes[i].name,nframes);
					frame_element = cJSON_GetObjectItem(frames,key_string);
					if(frame_element==NULL){
						break;
					}
					nframes++;
				}

				skp->sprite_keyframes[i].frames_len=nframes;
				skp->sprite_keyframes[i].frames=malloc(sizeof(SpriteKeyFrameInfo)*nframes);

				for(unsigned nframe=0; nframe < nframes; nframe++){ // get total frames
					cJSON *frame_property = NULL;
					cJSON *duration_property=NULL;
					cJSON *frame_element_property=NULL;

					sprintf(key_string,"%s (%s) %i.ase",key_id_prefix,skp->sprite_keyframes[i].name,nframe);
					frame_element = cJSON_GetObjectItem(frames,key_string);

					// load stuff here
					frame_property = cJSON_GetObjectItem(frame_element,"frame");
					duration_property = cJSON_GetObjectItem(frame_element,"duration");

					if(frame_property && duration_property){
						if((frame_element_property=cJSON_GetObjectItem(frame_property,"x"))!=NULL){
							skp->sprite_keyframes[i].frames[nframe].frame.u1=frame_element_property->valuedouble/texture->width;
						}
						if((frame_element_property=cJSON_GetObjectItem(frame_property,"y"))!=NULL){
							skp->sprite_keyframes[i].frames[nframe].frame.v1=frame_element_property->valuedouble/texture->height;
						}
						if((frame_element_property=cJSON_GetObjectItem(frame_property,"w"))!=NULL){
							skp->sprite_keyframes[i].frames[nframe].frame.u2=skp->sprite_keyframes[i].frames->frame.u1+frame_element_property->valuedouble/texture->width;
						}
						if((frame_element_property=cJSON_GetObjectItem(frame_property,"h"))!=NULL){
							skp->sprite_keyframes[i].frames[nframe].frame.v2=skp->sprite_keyframes[i].frames->frame.v1+frame_element_property->valuedouble/texture->height;
						}

						skp->sprite_keyframes[i].frames[nframe].duration=duration_property->valueint;
					}


				}

				Log_Info("'%s': %i frames",skp->sprite_keyframes[i].name ,skp->sprite_keyframes[i].frames_len);


			}

			//SpriteKeyFrame *skf=SpriteKeyFrame_New(texture);
			return true;

		}
	}

	return false;

}

/**
 * Load a set of SpriteKeyFrame from exported files from Aseprite
 * @_this: SpriteKeyFrameManager object
 * @_texture_filename: Texture filename (it can be .png,jpg,etc)
 * @_json_filename: Json file generated by Aseprite
 * @_extra_json_filename: Json file where it adds some extra information per frame (for instance collider)
 */
bool SpriteKeyFrameManager_Load(SpriteKeyFrameManager *_this,const char *_key_id_prefix, const char *_image_filename,const char *_json_filename, const char *_extra_json_filename){
	//UNUSUED_PARAM(_extra_json);

	BufferByte *img_buffer=NULL, *json_buffer=NULL, *extra_buffer=NULL;bool ok=false;

	/*if(File_Exists(_texture_filename) == false){
		Log_Error("Cannot load sprite key frame. File '%s' not exist",_texture_filename);
		return false;
	}

	if(File_Exists(_json_filename) == false){
		Log_Error("Cannot load sprite key frame. File '%s' not exist",_json_filename);
		return false;
	}

	;*/

	if((img_buffer=File_Read(_image_filename))!=NULL){
		if((json_buffer=File_Read(_json_filename))!=NULL){
			ok=SpriteKeyFrameManager_LoadFromMemory(
					_this
					,_key_id_prefix
					,img_buffer->ptr
					,img_buffer->len
					,json_buffer->ptr
					,json_buffer->len
			);
		}
	}

	if(img_buffer) BufferByte_Delete(img_buffer);
	if(json_buffer) BufferByte_Delete(json_buffer);


	return ok;
}

SpriteKeyFrame *GetSpriteKeyFrame(SpriteKeyFrameManager *_this, const char *key){
	SpriteKeyFrameManagerData *data=_this->data;

	return MapString_GetValue(data->sprite_keyframes,key,NULL);
}

void  SpriteKeyFrameManager_Delete(SpriteKeyFrameManager *_this){
	SpriteKeyFrameManagerData 	*data=_this->data;

	for(unsigned i=0; i < data->sprite_keyframe_packs->count;i++){
		SpriteKeyFramePack_Delete(data->sprite_keyframe_packs->items[i]);
	}


	List_Delete(data->sprite_keyframe_packs);
	MapString_Delete(data->sprite_keyframes);
	FREE(data);
	FREE(_this);
}
