#ifndef __GUID_H__
#define __GUID_H__

#define UUID_LENGTH 37

typedef struct Guid Guid;

struct Guid{
	char uuid[UUID_LENGTH];
};

extern const char *GUID_EMPTY;

Guid * Guid_New(void);
void   Guid_Delete(Guid *guid);

#endif
