#pragma once

// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN
#define NOMINMAX

#include <afxwin.h>   // MFC core and standard components
#include <afxext.h>   // MFC extensions
#include <afxdisp.h>  // MFC Automation classes
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // MFC support for Windows Common Controls
#endif              // _AFX_NO_AFXCMN_SUPPORT

#include <winsock2.h> // Winsock 2
#include <afxtempl.h>
#include <afxdb.h>

#include "StdBase.h"
#include "N3Utils.h"

//#include <mmsystem.h>
//#include <imm.h>
//#include "N3Base/My_3DStruct.h"

#include "Packet.h" // packet 정의
#include "global.h" // 전역 funtion
#include "Define.h" // define
//#include "Extern.h"            // 전역 객체
