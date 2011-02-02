// Plugin.h : Declaration of the CPlugin

#pragma once
#include "resource.h"       // main symbols

#include "SearchPluginIE_i.h"

#include <shlguid.h>
#include <docobj.h>
#include <mshtml.h>
#include <urlmon.h>
#include <shlobj.h>
#include <exdispid.h>

#include "settings.h"
#include "regmap.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CPlugin

class ATL_NO_VTABLE CPlugin :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPlugin, &CLSID_Plugin>,
	public IObjectWithSiteImpl<CPlugin>,
	public IDispatchImpl<IPlugin, &IID_IPlugin, &LIBID_SearchPluginIELib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1, CPlugin, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
	CPlugin()
	{
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_PLUGIN)

DECLARE_REGISTRY_RESOURCEID_EX(IDR_PLUGIN)

BEGIN_REGISTRY_MAP(CPlugin)
    REGMAP_ENTRY("PROGID",      "SearchPluginIE.Plugin")
    REGMAP_ENTRY("VERSION",     "1")
    REGMAP_ENTRY("DESCRIPTION", "Search IE plugin")
    REGMAP_UUID ("CLSID",       CLSID_Plugin)
    REGMAP_UUID ("LIBID",       LIBID_SearchPluginIELib)
    REGMAP_ENTRY("THREADING",   "Apartment")
    REGMAP_RESOURCE("MENUITEM", IDS_MENUITEMTEXT)
END_REGISTRY_MAP()

DECLARE_NOT_AGGREGATABLE(CPlugin)

BEGIN_COM_MAP(CPlugin)
	COM_INTERFACE_ENTRY(IPlugin)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

BEGIN_SINK_MAP(CPlugin)
	SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete)
END_SINK_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(Navigate)(BSTR text, BSTR escText, IUnknown* disp, float vers);
    STDMETHOD(SetSite)(IUnknown *pUnkSite);
// IWebBowser2
public:
    void STDMETHODCALLTYPE OnNavigateComplete(IDispatch *pDisp,
		VARIANT *&url);

private:
    CComPtr<IWebBrowser2> m_pBrowser;
    HRESULT NavigateBrowser(CString link);
    CSettings setup;
};

OBJECT_ENTRY_AUTO(__uuidof(Plugin), CPlugin)
