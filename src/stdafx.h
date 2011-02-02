// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <comsvcs.h>

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <comutil.h> 

#pragma comment(lib, "comsuppwd.lib")
#pragma comment(lib, "version.lib")


using namespace ATL;

__declspec(selectany) LPCTSTR MUTEX_NAME = _T("{43759E93-A138-4f5e-A22B-1D18A67E9C4C}");

#define REGISTRY_KEYNAME L"Software\\SearchPluginIE"
#define REGISTRY_VALNAME_FIRSTRUN L"FirstRun"