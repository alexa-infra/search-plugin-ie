#pragma once

#include <atlbase.h>
struct _ATL_REGMAP_ENTRYKeeper : public _ATL_REGMAP_ENTRY{	_ATL_REGMAP_ENTRYKeeper(){ szKey=NULL; szData=NULL;}	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, LPCOLESTR data) 	{		szKey=key;		LPOLESTR newData;		szData = LPCOLESTR(newData =  new wchar_t[wcslen(data)+1]);		wcscpy(newData,data);	}	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, UINT resid)	{		wchar_t Data[1024];		USES_CONVERSION;		szKey=key;		if( 0 == LoadStringW(_pModule->m_hInstResource, resid, Data, 1024))		{			*Data = '\0';		}		LPOLESTR newData;		szData = LPCOLESTR(newData =  new wchar_t[wcslen(Data)+1]);		wcscpy(newData,Data);	}	_ATL_REGMAP_ENTRYKeeper(LPCOLESTR key, REFGUID guid) 	{		szKey=key;		LPOLESTR newData;		szData = LPCOLESTR(newData =  new wchar_t[40]);		if(szData!=NULL)		{			if(0==StringFromGUID2(guid, newData,40))			{				*newData=NULL;			}		}	}	~_ATL_REGMAP_ENTRYKeeper()	{		delete [] (LPOLESTR)szData;	}};

#define BEGIN_REGISTRY_MAP(x) \
	static struct _ATL_REGMAP_ENTRY *_GetRegistryMap()\
	{\
		static const _ATL_REGMAP_ENTRYKeeper map[] = {
#define REGMAP_ENTRY(x,y) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),OLESTR(##y)),

#define REGMAP_RESOURCE(x,resid) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),resid), 

#define REGMAP_UUID(x,clsid) _ATL_REGMAP_ENTRYKeeper(OLESTR(##x),clsid),

#define END_REGISTRY_MAP() _ATL_REGMAP_ENTRYKeeper() }; return (_ATL_REGMAP_ENTRY *)map;}

#define DECLARE_REGISTRY_RESOURCEID_EX(x)\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)\
	{\
        	__if_exists(_Module) \
			{ \
				return _Module.UpdateRegistryFromResource(x, bRegister, _GetRegistryMap()); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(x, bRegister, _GetRegistryMap()); \
			} \
	}

