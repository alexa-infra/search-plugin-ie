// dllmain.h : Declaration of module class.

class CSearchPluginIEModule : public CAtlDllModuleT< CSearchPluginIEModule >
{
public :
	DECLARE_LIBID(LIBID_SearchPluginIELib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SEARCHPLUGINIE, "{9AFED535-3E49-42a6-B345-11EB4CEC4F86}")
};

extern class CSearchPluginIEModule _AtlModule;
