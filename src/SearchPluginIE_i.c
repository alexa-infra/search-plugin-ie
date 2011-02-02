

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Feb 02 23:07:22 2011
 */
/* Compiler settings for .\SearchPluginIE.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IPlugin,0x260860E0,0x09B6,0x4743,0x96,0x50,0x5F,0xB0,0xD9,0x28,0x60,0x9A);


MIDL_DEFINE_GUID(IID, LIBID_SearchPluginIELib,0x1B53859D,0x1350,0x4e87,0xA8,0xD0,0x23,0xA2,0x5F,0xF7,0x5B,0xC5);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0xB414B0D0,0xAA6D,0x49b3,0x83,0x6E,0xF0,0xE4,0xBA,0x93,0xF1,0x4D);


MIDL_DEFINE_GUID(CLSID, CLSID_Plugin,0x110E5031,0xE03B,0x49F1,0xA7,0x8D,0xF5,0x70,0xDE,0x0D,0x02,0x52);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



