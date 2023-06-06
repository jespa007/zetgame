#ifndef __ZG_GUID_H__
#define __ZG_GUID_H__

#define ZG_UUID_LENGTH 37

typedef struct ZG_Guid ZG_Guid;

struct ZG_Guid{
	char uuid[ZG_UUID_LENGTH];
};

extern const char *ZG_GUID_EMPTY;

ZG_Guid * ZG_Guid_New(void);
void   ZG_Guid_Delete(ZG_Guid *guid);

#endif
