#pragma once

#include <atlbase.h>
struct _ATL_REGMAP_ENTRYKeeper : public _ATL_REGMAP_ENTRY

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

			{ \
				return _Module.UpdateRegistryFromResource(x, bRegister, _GetRegistryMap()); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(x, bRegister, _GetRegistryMap()); \
			} \
	}
