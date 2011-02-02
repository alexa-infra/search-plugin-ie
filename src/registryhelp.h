#pragma once

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

class CRegistryHelp{
public:
	static BOOL IsProtectedModeProcess();
	static HKEY GetWriteableHKCU();
	static void WriteFirstRun();
	static void DeleteFirstRun();
	static bool IsAlreadyFirstRun();
	static LONG GetStringRegKey(HKEY hKey, const CString &strValueName, CString &strValue, const CString &strDefaultValue);
	static void DeleteOldKeys();
	static void WriteFirstRunHKCU();
	static void DeleteFirstRunHKCU();
};