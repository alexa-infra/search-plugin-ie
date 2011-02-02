// Plugin.cpp : Implementation of CPlugin

#include "stdafx.h"
#include "Plugin.h"
#include "urlencode.h"
#include "registryhelp.h"

// CPlugin

const wchar_t* PLUGINBHOPRESENTED = L"SearchPluginBHOPresented";

struct fileVersion {
    int major;
    int minor;
    int revision;
    int build;
};

CString FixedModuleVersion(HMODULE hmod)
{
    if (!hmod)
        hmod = ::GetModuleHandle(L"SearchPluginIE");
    TCHAR filepath[MAX_PATH];
    GetModuleFileName(hmod, filepath, sizeof(filepath));
    DWORD dwDummyHandle;
    DWORD len = GetFileVersionInfoSize(filepath, &dwDummyHandle);
    BYTE * buf = new BYTE[len];
    ::GetFileVersionInfo(filepath, 0, len, buf);
    unsigned int verlen;
    LPVOID lpvi;
    ::VerQueryValue(buf, _T("\\"), &lpvi, &verlen);
    VS_FIXEDFILEINFO fileInfo;
    fileInfo = *(VS_FIXEDFILEINFO*)lpvi;
    delete[] buf;
    fileVersion ver;
    ver.major = HIWORD(fileInfo.dwFileVersionMS);
    ver.minor = LOWORD(fileInfo.dwFileVersionMS);
    ver.revision = HIWORD(fileInfo.dwFileVersionLS);
    ver.build = LOWORD(fileInfo.dwFileVersionLS);

    CString ret;
    ret.Format(L"%d.%d.%d.%d", ver.major, ver.minor, ver.revision, ver.build);
    return ret;
}


STDMETHODIMP CPlugin::SetSite(IUnknown* pUnkSite) {

	_bstr_t propName(PLUGINBHOPRESENTED);

    if (pUnkSite != NULL)
    {
        // Cache the pointer to IWebBrowser2.
        m_pBrowser = CComQIPtr<IWebBrowser2>(pUnkSite);
        if (m_pBrowser != NULL) {
            
			CHandle hMutex;
			hMutex.Attach ( OpenMutex ( MUTEX_ALL_ACCESS, false, MUTEX_NAME ) );

			if (hMutex != NULL) {
                CRegistryHelp::DeleteFirstRun();
			}

			((ATL::IDispEventSimpleImpl<1,CPlugin,&DIID_DWebBrowserEvents2>*)this)->DispEventAdvise(m_pBrowser);

			
			_variant_t pThis(this, true);
			m_pBrowser->PutProperty(propName.GetBSTR(), pThis);
        }
    } 
    else 
    {
		_variant_t pNull(NULL);
		m_pBrowser->PutProperty(propName.GetBSTR(), pNull);

        // Release cached pointers and other resources here.
        m_pBrowser.Release();
    }

    // Call base class implementation.
	return IObjectWithSiteImpl<CPlugin>::SetSite(pUnkSite);
}
static bool bChecked = false;
void STDMETHODCALLTYPE CPlugin::OnNavigateComplete(IDispatch *pDisp,
		VARIANT *&url)
{
	if (bChecked)
		return;
	bChecked = true;
	if (m_pBrowser != NULL) {
		if (!CRegistryHelp::IsAlreadyFirstRun()) {
			CRegistryHelp::WriteFirstRun();
			CString navi = setup.GetString(e_Settings::e_searchWelcomePage);
			NavigateBrowser(navi);
		}
	}
}


CComQIPtr<IWebBrowser2> IHTMLWindow2ToIWebBrowser2(CComQIPtr<IHTMLWindow2> spHTMLWindow)
{
     CComQIPtr<IWebBrowser2>     spBrowser;
     CComQIPtr<IServiceProvider> spServiceProvider = spHTMLWindow;

     if (spServiceProvider != NULL)
     {
          spServiceProvider->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&spBrowser);
     }

     return spBrowser;
}

HRESULT CPlugin::Navigate(BSTR text, BSTR escText, IUnknown* pUnk, float ieVers)
{
    CComQIPtr<IHTMLWindow2> win(pUnk);
	m_pBrowser = IHTMLWindow2ToIWebBrowser2(win); 

	if (m_pBrowser == NULL)
		return E_FAIL;

	_bstr_t propName(PLUGINBHOPRESENTED);
	_variant_t var;
	HRESULT hr = m_pBrowser->GetProperty(propName.GetBSTR(),var.GetAddress());
	if (FAILED(hr) || var.pdispVal == NULL)
		return E_FAIL;
    
    CString s = text;
    
    int min = setup.GetInt(e_Settings::e_searchMinChars);
    if (s.GetLength() < min) {
        CString error = setup.GetString(e_Settings::e_searchMinCharsError);
        ::MessageBox(NULL, error, L"", 0);
        return E_FAIL;
    }

    int max = setup.GetInt(e_Settings::e_searchMaxChars);
    if (s.GetLength() > max) {
        CString error = setup.GetString(e_Settings::e_searchMaxCharsError);
        ::MessageBox(NULL, error, L"", 0);
        return E_FAIL;
    }

    CUrlEncode enc;
    CString encoded = escText;//enc.convert(escText);

    CString version = FixedModuleVersion(NULL);

    CString format = setup.GetString(e_Settings::e_searchUrlFormat);

    CString ieVersion;
    ieVersion.Format(L"IE%.1f", ieVers);

    CString res;
    res.Format(format, encoded, version, ieVersion);

    if (FAILED(NavigateBrowser(res)))
    {
        IHTMLWindow2* pnewwnd = NULL;
        _bstr_t url = res.GetString();
        _bstr_t blank = L"_blank";
        _bstr_t empty = L"";
        HRESULT hr = win->open(url,
              blank,
              empty,
              VARIANT_TRUE,
              &pnewwnd);
    }
	return S_OK;
}

HRESULT CPlugin::NavigateBrowser(CString link) {
    if (m_pBrowser == NULL)
        return E_FAIL;
    _bstr_t site = link.GetString();
	_variant_t u(site);
    _variant_t flag, targetFrameName, postData, headers;;
    flag.ChangeType(VT_I4, &flag);
    flag.intVal =  0x800;
    m_pBrowser->Navigate2(&u, &flag, &targetFrameName, &postData, &headers);
    return S_OK;
}