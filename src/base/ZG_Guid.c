#include "_zg_base_.h"

const char *ZG_GUID_EMPTY="00000000-0000-0000-0000-000000000000";

ZG_Guid * ZG_Guid_New(void)
{
	ZG_Guid *guid=ZG_NEW(ZG_Guid);
#ifdef _WIN32
    UUID uuid;
    UuidCreate ( &uuid );

    unsigned char * str;
    UuidToStringA ( &uuid, &str );

    strcpy(guid->uuid,(char *)str);

    RpcStringFreeA ( &str );
#else
    uuid_t uuid;
    uuid_generate_random ( uuid );

    uuid_unparse ( uuid, guid->uuid );
#endif
    return guid;
}

void ZG_Guid_Delete(ZG_Guid * _this){
	if(_this==NULL) return;

	ZG_FREE(_this);
}
