// SearchPluginIE.cpp : Implementation of DLL Exports.

//
// Note: COM+ 1.0 Information:
//      Please remember to run Microsoft Transaction Explorer to install the component(s).
//      Registration is not done by default. 

#include "stdafx.h"
#include "resource.h"
#include "SearchPluginIE_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "dlldatax.h"

#include <sddl.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#include "registryhelp.h"

// Create an instance of Internet Explorer
IWebBrowser2* RunIe( bool show /*=true*/)
{
	IWebBrowser2 *pIE = 0;
	HRESULT hr = CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pIE);
	if (SUCCEEDED(hr) && show)
		pIE->put_Visible(VARIANT_FALSE);
	return pIE;
}

LPCWSTR LOW_INTEGRITY_SDDL_SACL_W = L"S:(ML;;NW;;;LW)";

bool SetObjectToLowIntegrity(HANDLE hObject, SE_OBJECT_TYPE type = SE_KERNEL_OBJECT)
{
	bool bRet = false;
	DWORD dwErr = ERROR_SUCCESS;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pSacl = NULL;
	BOOL fSaclPresent = FALSE;
	BOOL fSaclDefaulted = FALSE;

	if ( ConvertStringSecurityDescriptorToSecurityDescriptorW (
		LOW_INTEGRITY_SDDL_SACL_W, SDDL_REVISION_1, &pSD, NULL ) )
	{
		if ( GetSecurityDescriptorSacl (
			pSD, &fSaclPresent, &pSacl, &fSaclDefaulted ) )
		{
			dwErr = SetSecurityInfo (
				hObject, type, LABEL_SECURITY_INFORMATION,
				NULL, NULL, NULL, pSacl );

			bRet = (ERROR_SUCCESS == dwErr);
		}

		LocalFree ( pSD );
	}

	return bRet;
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    CRegistryHelp::DeleteOldKeys();
	CRegistryHelp::WriteFirstRunHKCU();
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif
	return hr;
}



// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	CoInitialize(NULL);
	HANDLE hMutex = CreateMutex(NULL, true, MUTEX_NAME);
	if ( hMutex )
        SetObjectToLowIntegrity ( hMutex );
    IWebBrowser2 *pIE = RunIe(false);
	if (pIE != NULL)
	{		
		pIE->Quit();
		pIE->Release();
		CoUninitialize();
	}
	CloseHandle(hMutex);
    CRegistryHelp::DeleteFirstRunHKCU();

	HRESULT hr = _AtlModule.DllUnregisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
	return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.	
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = _T("user");

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}


