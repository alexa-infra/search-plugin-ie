#include "stdafx.h"
#include "settings.h"

extern HINSTANCE g_hinst;

CString CSettings::GetString(e_Settings::Enum s)
{
	TCHAR buff[1024];
	LoadString(g_hinst, s, buff, 1024);
	CString str = buff;
	return str;
}

int CSettings::GetInt(e_Settings::Enum s)
{
	CString str = GetString(s);
	int n = _ttoi(str);
	return n;
}
