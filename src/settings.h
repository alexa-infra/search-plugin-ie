#pragma once

#include "resource.h"

namespace e_Settings
{
    enum Enum {
        e_searchMinChars = IDS_SEARCHMINCHARS,
        e_searchMaxChars = IDS_SEARCHMAXCHARS,
        e_searchUrlFormat = IDS_SEARCHURLFORMAT,
        e_searchWelcomePage = IDS_WELCOMEPAGEURL,
        e_searchMenuText = IDS_MENUITEMTEXT,
        e_searchMinCharsError = IDS_SEARCHMINCHARSERROR,
        e_searchMaxCharsError = IDS_SEARCHMAXCHARSERROR
    };
}

class CSettings {
public:
    CString GetString(e_Settings::Enum s);
	int GetInt(e_Settings::Enum s);
};