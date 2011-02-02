#include "stdafx.h"
#include "urlencode.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using std::wstring;
using std::wostringstream;

bool CUrlEncode::isSafeChar(const wchar_t& compareChar)
{
    unsigned int val = compareChar;
    return ((val >= 48 && val <= 57) || (val >= 65 && val <= 90) || (val >= 97 && val <= 122));
}

CString CUrlEncode::charToSafe(const wchar_t& _val)
{
    wostringstream ss(wostringstream::out);

    ss << std::hex << std::setw((_val < 256) ? 2 : 4) << std::setfill(L'0') << (unsigned int)_val;

    wstring str = ss.str();
    // 'touppercase' transformation of hex-characters
    transform(
        str.begin(), str.end(),
        str.begin(),
        toupper);

    return CString(str.c_str());
}

CString CUrlEncode::convertUnsafeChar(const wchar_t& val)
{
    if (val == L' ')
        return L"+";

    CString csRet;
    CString safe = charToSafe(val);
    if (val > 255)
    {
        csRet = L'%';
        csRet += safe[0];
        csRet += safe[1];
        csRet += L'%';
        csRet += safe[2];
        csRet += safe[3];
    }
    else 
    {
        csRet = L'%';
        csRet += safe;
    }
    
    return csRet;
}

CString CUrlEncode::convert(const CString& val)
{
    wstring csRet;

    for (int i=0; i<val.GetLength(); i++) {
        wchar_t ch = val[i];
        if (isSafeChar(ch)) {
            csRet.push_back(ch);
        } else {
            csRet.append(convertUnsafeChar(ch));
        }
    }

    return CString(csRet.c_str());
}
