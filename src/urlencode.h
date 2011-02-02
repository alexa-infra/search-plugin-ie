#pragma once

/*
specification for URLs (RFC 1738)

for UNICODE (2-bytes):
'\%HH\%HH' - where HHHH is the hexidemical representation of bytes

mime-type: application/x-www-form-urlencoded
'%20' (the space character can be normally replaced by '+')
'\n' - '%0D%0A'
*/

class CUrlEncode
{
private:
    bool isSafeChar(const wchar_t&);
    CString charToHex(const wchar_t&);
    CString charToSafe(const wchar_t&);

public:
    CUrlEncode() { };
    virtual ~CUrlEncode() { };

    CString convertUnsafeChar(const wchar_t&);
    CString convert(const CString&);
};