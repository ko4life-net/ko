// StdAfx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma warning(disable : 4786)

#pragma once

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include <map>
#include <set>
#include <list>
#include <vector>
#include <format>
#include <string>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <io.h>

#include "N3Base/My_3DStruct.h"
#include "N3Base/N3Log.h"

namespace fs = std::filesystem;

// TODO: Since we now use spdlog, we can get ride of traces like these.
#if defined(_DEBUG)
#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define TRACE(fmt, ...) (void)fmt
#endif

namespace N3 {
static bool iequals(const std::string_view & lhs, const std::string_view & rhs) {
    auto to_lower{std::ranges::views::transform(::tolower)};
    return std::ranges::equal(lhs | to_lower, rhs | to_lower);
}
} // namespace N3
