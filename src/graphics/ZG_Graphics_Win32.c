#include "_zg_graphics_.h"

#ifdef _WIN32
BOOL ZG_Graphics_Win32_GetMonitorDevice( TCHAR* adapterName, DISPLAY_DEVICE * ddMon )
{
    DWORD devMon = 0;

    while (EnumDisplayDevices(adapterName, devMon, ddMon, 0))
    {
        if ((ddMon->StateFlags & DISPLAY_DEVICE_ACTIVE) &&
            (ddMon->StateFlags & DISPLAY_DEVICE_ATTACHED)) // for ATI, Windows XP
            break;

        devMon++;
    }

    if (ddMon->DeviceString[0] == '\0')
    {
        EnumDisplayDevices(adapterName, 0, ddMon, 0);
        if (ddMon->DeviceString[0] == '\0'){
            _tcscpy_s(ddMon->DeviceString, _countof(ddMon->DeviceString), _T("Default Monitor"));
        }
    }
    return ddMon->DeviceID[0] != '\0';
}

BOOL ZG_Graphics_Win32_GetMonitorSizeFromEDID(TCHAR* adapterName, TCHAR *monitorModel, DWORD * Width, DWORD * Height)
{
    DISPLAY_DEVICE ddMon;
    ZeroMemory(&ddMon, sizeof(ddMon));
    ddMon.cb = sizeof(ddMon);

    //read edid
    bool result = false;
    *Width = 0;
    *Height = 0;
    if (ZG_Graphics_Win32_GetMonitorDevice(adapterName, &ddMon))
    {
        TCHAR model[8];
        TCHAR* s = _tcschr(ddMon.DeviceID, '\\') + 1;
        size_t len = _tcschr(s, '\\') - s;
        if (len >= _countof(model))
            len = _countof(model) - 1;
        _tcsncpy(model, s, len);
        _tcsncpy(monitorModel,s,len);

        TCHAR *path = _tcschr(ddMon.DeviceID, '\\') + 1;
        TCHAR str[MAX_PATH] = _T("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\");
        _tcsncat(str, path, _tcschr(path, '\\')-path);
        path = _tcschr(path, '\\') + 1;
        HKEY hKey;
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, str, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            DWORD i = 0;
            DWORD size = MAX_PATH;
            FILETIME ft;
            while(RegEnumKeyEx(hKey, i, str, &size, NULL, NULL, NULL, &ft) == ERROR_SUCCESS)
            {
                HKEY hKey2;
                if(RegOpenKeyEx(hKey, str, 0, KEY_READ, &hKey2) == ERROR_SUCCESS)
                {
                    size = MAX_PATH;
                    if(RegQueryValueEx(hKey2, _T("Driver"), NULL, NULL, (LPBYTE)&str, &size) == ERROR_SUCCESS)
                    {
                        if (_tcscmp(str, path) == 0)
                        {
                            HKEY hKey3;
                            if(RegOpenKeyEx(hKey2, _T("Device Parameters"), 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
                            {
                                BYTE EDID[256];
                                size = 256;
                                if(RegQueryValueEx(hKey3, _T("EDID"), NULL, NULL, (LPBYTE)&EDID, &size) == ERROR_SUCCESS)
                                {
                                    DWORD p = 8;
                                    TCHAR model2[9];

                                    char byte1 = EDID[p];
                                    char byte2 = EDID[p+1];
                                    model2[0]=((byte1 & 0x7C) >> 2) + 64;
                                    model2[1]=((byte1 & 3) << 3) + ((byte2 & 0xE0) >> 5) + 64;
                                    model2[2]=(byte2 & 0x1F) + 64;
                                    _stprintf(model2 + 3, _T("%X%X%X%X"), (EDID[p+3] & 0xf0) >> 4, EDID[p+3] & 0xf, (EDID[p+2] & 0xf0) >> 4, EDID[p+2] & 0x0f);
                                    if (_tcscmp(model, model2) == 0) // the values are in cm, so we want in mm
                                    {
                                        *Width = EDID[21]*10;
                                        *Height = EDID[22]*10;
                                        result = true;
                                    }
                                    else
                                    {
                                        // EDID incorrect
                                    }
                                }
                                RegCloseKey(hKey3);
                            }
                        }
                    }
                    RegCloseKey(hKey2);
                }
                i++;
            }
            RegCloseKey(hKey);
        }
    }

    return result;
}

BOOL ZG_Graphics_Win32_GetMonitorInfo(int nDeviceIndex, LPSTR lpszMonitorInfo) {
    BOOL bResult = TRUE;
    //FARPROC EnumDisplayDevices;
    //HINSTANCE  hInstUserLib;
    DISPLAY_DEVICE DispDev;
    char szDeviceName[32];


    ZeroMemory(&DispDev, sizeof(DISPLAY_DEVICE));
    DispDev.cb = sizeof(DISPLAY_DEVICE);

    // After first call to EnumDisplayDevices DispDev.DeviceString
    //Contains graphic card name
    if(EnumDisplayDevices(NULL, nDeviceIndex, &DispDev, 0)) {
        lstrcpy(szDeviceName, DispDev.DeviceName);

        // after second call DispDev.DeviceString Contains monitor's name
        EnumDisplayDevices(szDeviceName, 0, &DispDev, 0);

        lstrcpy(lpszMonitorInfo, DispDev.DeviceString);
    }
    else {
        bResult = FALSE;
    }

     return bResult;
}

BOOL CALLBACK ZG_Graphics_Win32_MonitorEnumProc(LPARAM dwData)
{


    int *Count = (int*)dwData;
    (*Count)++;
    return TRUE;
}

void ZG_Graphics_Win32_ShowInTaskBar(HWND hWnd, BOOL _show){


	ShowWindow(hWnd, SW_HIDE);
	LONG_PTR ExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	if(_show){
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, (ExStyle & ~WS_EX_APPWINDOW) | WS_EX_TOOLWINDOW);
	}else{
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, (ExStyle & ~WS_EX_APPWINDOW) | WS_EX_APPWINDOW);
	}
	ShowWindow(hWnd, SW_SHOW);

}

ZG_List * ZG_Graphics_Win32_GetListAdapters(void){
	ZG_List * lst_monitor_info=ZG_List_New();
    DISPLAY_DEVICE dd;
    char device_name[32]; // 32 for the null-terminator
    dd.cb = sizeof(dd);
    int idx_device = 0;
    SDL_Rect rect;


    while(EnumDisplayDevices(NULL, idx_device, &dd, 1))
    {
        int idx_monitor = 0;
        strncpy(device_name, dd.DeviceName,sizeof(dd.DeviceName));
        while(EnumDisplayDevices(device_name, idx_monitor, &dd, 1))
        {
        	ZG_AdapterInfo *mei=ZG_NEW(ZG_AdapterInfo);
        	DWORD width, height;
        	//CHAR monitor_model[1024];

            ZG_Graphics_Win32_GetMonitorSizeFromEDID(device_name,mei->monitor_model, &width, &height);

            //std::cout << mei.MonitorModel << ":" << width <<"x" << height << "." << idx_device << "\n";

        	if(ZG_Graphics_GetScreenBounds( idx_device,&rect)){
        		mei->pixels_width=rect.w;
        		mei->pixels_height=rect.h;
        	}

            mei->physical_width=width;
            mei->physical_height=height;
           // strcpy(mei.MonitorModel,monitor_model);
            ZG_List_Add(lst_monitor_info,mei);

            ++idx_monitor;

        }
        ++idx_device;
    }
	return lst_monitor_info;
}

#endif

