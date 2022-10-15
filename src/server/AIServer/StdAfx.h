// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers

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

#include <map>
#include <set>
#include <list>
#include <vector>
#include <format>
#include <string>
#include <sstream>
#include <ranges>
#include <algorithm>

//#include "Mmsystem.h"
//#include "Imm.h"
//#include "N3Base/My_3DStruct.h"

#include "packet.h" // packet 정의
#include "global.h" // 전역 funtion
#include "define.h" // define
//#include "extern.h"            // 전역 객체

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

namespace N3 {
static bool iequals(const std::string_view & lhs, const std::string_view & rhs) {
    auto to_lower{std::ranges::views::transform(::tolower)};
    return std::ranges::equal(lhs | to_lower, rhs | to_lower);
}
} // namespace N3
