#include "stdafx.h"
#include "registryhelp.h"

typedef HRESULT STDAPICALLTYPE IEGETWRITEABLEHKCUFN (HKEY *phKey); 
typedef HRESULT STDAPICALLTYPE IEISPROTECTEDMODEPROCESSFN (BOOL *pbResult); 
BOOL CRegistryHelp::IsProtectedModeProcess() 
{ 
   HINSTANCE    Instance; 
   IEISPROTECTEDMODEPROCESSFN *IEIsProtectedModeProcessProc; 
   BOOL         IsProtecedMode = FALSE; 

   Instance = LoadLibrary(L"ieframe.dll"); 
   if (Instance) 
   { 
      IEIsProtectedModeProcessProc = (IEISPROTECTEDMODEPROCESSFN *) GetProcAddress(Instance, "IEIsProtectedModeProcess"); 
      if (IEIsProtectedModeProcessProc) 
      { 
         HRESULT Res = IEIsProtectedModeProcessProc(&IsProtecedMode); 
         if (FAILED(Res)) 
         { 
            if (Res == 0x80070005) 
               IsProtecedMode = TRUE; // access denied, assume protected mode 
            else 
               IsProtecedMode = FALSE; 
         } 
      } 
      FreeLibrary(Instance); 
   } 

   return IsProtecedMode; 
} 

HKEY CRegistryHelp::GetWriteableHKCU() 
{ 
   HINSTANCE    Instance; 
   IEGETWRITEABLEHKCUFN *IEGetWriteableHKCUProc; 
   HKEY         Key = HKEY_CURRENT_USER; 

   if (IsProtectedModeProcess()) 
   { 
      Instance = LoadLibrary(L"ieframe.dll"); 
      if (Instance) 
      { 
         IEGetWriteableHKCUProc = (IEGETWRITEABLEHKCUFN *) GetProcAddress(Instance, "IEGetWriteableHKCU"); 
         if (IEGetWriteableHKCUProc) 
         { 
            HRESULT Res = IEGetWriteableHKCUProc(&Key); 
            if (FAILED(Res)) 
               Key = HKEY_CURRENT_USER; 
         } 
         FreeLibrary(Instance); 
      } 
   } 

   return Key; 
} 

void CRegistryHelp::WriteFirstRun()
{
    HKEY hMyKey;
    LPCTSTR pszKey = REGISTRY_KEYNAME; 
    LPCTSTR pszValue = REGISTRY_VALNAME_FIRSTRUN; 
    LPCTSTR pszData = L"false";

    HKEY hKey = GetWriteableHKCU();

    DWORD dwDisposition;
    LONG lRes = RegCreateKeyEx(
        hKey,         // Low integrity write location
        pszKey,       // Specified sub-key
        0L,           // Reserved
        NULL,         // Class
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,         // Security Descriptor
        &hMyKey,      // NEW or EXISTING key
        &dwDisposition);

    if (ERROR_SUCCESS == lRes)
    {
        lRes = RegSetValueEx(hMyKey, pszValue, NULL, REG_SZ,(CONST BYTE*)pszData, wcslen(pszData)*2 + 1);
        RegCloseKey(hMyKey);
    }

    // Close the low integrity handle
    RegCloseKey(hKey);   
}

void CRegistryHelp::DeleteFirstRun() 
{
    LPCTSTR pszKey = REGISTRY_KEYNAME; 
    HKEY hKey = GetWriteableHKCU();
	LONG lRes = RegDeleteKey(
        hKey,
        pszKey);
    RegCloseKey(hKey);   
}

bool CRegistryHelp::IsAlreadyFirstRun()
{
    HKEY hMyKey;
    LPCTSTR pszKey = REGISTRY_KEYNAME; 
    LPCTSTR pszValue = REGISTRY_VALNAME_FIRSTRUN; 
    LPCTSTR pszData = L"false";

    HKEY hKey = GetWriteableHKCU();

    LONG lRes = RegOpenKeyEx(
        hKey,         // Low integrity write location
        pszKey,       // Specified sub-key
        0L,           // Reserved
        KEY_READ,
        &hMyKey);

    bool bRes = true;
    if (ERROR_SUCCESS == lRes)
    {
        TCHAR buf[512];
        DWORD ss = 512*sizeof(TCHAR);
        ZeroMemory(buf, ss);
        lRes = RegQueryValueEx(hMyKey, pszValue, NULL, NULL, (BYTE*)buf, &ss);
        if (ERROR_SUCCESS == lRes)
        {
            bRes = _wcsicmp(buf, pszData) == 0;
        }
        RegCloseKey(hMyKey);
    }

    // Close the low integrity handle
    RegCloseKey(hKey);   
    return bRes;
}

LONG CRegistryHelp::GetStringRegKey(HKEY hKey, const CString &strValueName, CString &strValue, const CString &strDefaultValue)
{
    strValue = strDefaultValue;
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    ULONG nError;
    nError = RegQueryValueExW(hKey, strValueName.GetString(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
            strValue = szBuffer;
    }
    return nError;
}
 
void CRegistryHelp::DeleteOldKeys() 
{ 
    HKEY hKey;
    if( RegOpenKeyEx( HKEY_CURRENT_USER,
        TEXT("SOFTWARE\\Microsoft\\Internet Explorer\\MenuExt"),
        0,
        KEY_READ,
        &hKey) != ERROR_SUCCESS
        ) 
        return;

    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i, retCode; 
 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    
    for (i=0; i<cSubKeys; i++) 
    { 
        cbName = MAX_KEY_LENGTH;
        retCode = RegEnumKeyEx(hKey, i,
                 achKey, 
                 &cbName, 
                 NULL, 
                 NULL, 
                 NULL, 
                 &ftLastWriteTime); 
        if (retCode != ERROR_SUCCESS)
            continue;
        HKEY hTestKey;
        if( RegOpenKeyEx( hKey,
                achKey,
                0,
                KEY_READ,
                &hTestKey) != ERROR_SUCCESS) 
            continue;

        CString strKeyDefaultValue;
        GetStringRegKey(hTestKey, L"", strKeyDefaultValue, L"");

        if (strKeyDefaultValue.Find(L"contextmenu.htm") > 0) {
            RegDeleteKey(hTestKey, L"");
        }
        RegCloseKey(hTestKey);
    }
    RegCloseKey(hKey);
}

void CRegistryHelp::WriteFirstRunHKCU() {
    HKEY hKey;
    if( RegOpenKeyEx( HKEY_CURRENT_USER,
        REGISTRY_KEYNAME,
        0,
        KEY_READ,
        &hKey) == ERROR_SUCCESS
        ) {
            RegCloseKey(hKey);
            return;
    }

    DWORD dwDisposition;
    LONG lRes = RegCreateKeyEx(
        HKEY_CURRENT_USER,
        REGISTRY_KEYNAME,
        0L,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,
        &hKey,
        &dwDisposition);
    if (lRes != ERROR_SUCCESS)
        return;
    lRes = RegSetValueEx(hKey, REGISTRY_VALNAME_FIRSTRUN, NULL, REG_SZ,(CONST BYTE*)L"true", 9);
    RegCloseKey(hKey);   
}

void CRegistryHelp::DeleteFirstRunHKCU() {
    HKEY hKey;
    if( RegOpenKeyEx( HKEY_CURRENT_USER,
        REGISTRY_KEYNAME,
        0,
        KEY_READ,
        &hKey) == ERROR_SUCCESS
        ) {
            RegDeleteKey(hKey, L"");
            return;
    }
}
