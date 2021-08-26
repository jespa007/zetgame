struct _PluginStruct
{
    NAMEPROC nameProc;
    PROVIDERPROC providerProc;
    MENUPROC menuProc;
    MENUCATPROC menuCatProc;
    RUNPROC runProc;
    DESTROYPROC destProc;
};

typedef LPPLUGINSTRUCT (*CREATEPROC) (void);
typedef void (*DESTROYPROC) (LPPLUGINSTRUCT);
typedef const gchar* (*NAMEPROC) (void);
typedef const gchar* (*PROVIDERPROC)(void);
typedef const gchar* (*MENUPROC) (void);
typedef const gchar* (*MENUCATPROC) (void);
typedef void (*RUNPROC) (void);

typedef struct _PluginStruct PluginStruct, *LPPLUGINSTRUCT;
