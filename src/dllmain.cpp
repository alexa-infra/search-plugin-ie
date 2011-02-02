// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "SearchPluginIE_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "dlldatax.h"

CSearchPluginIEModule _AtlModule;

HINSTANCE g_hinst = NULL;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Don't attach to Windows Explorer

        TCHAR pszLoader[MAX_PATH];
        GetModuleFileName(NULL, pszLoader, MAX_PATH);
        CString sLoader = pszLoader;
        sLoader.MakeLower();
        if (sLoader.Find(_T("explorer.exe")) >= 0)
            return FALSE;

        g_hinst = _AtlBaseModule.m_hInst;
    }
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
