#pragma once

// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN
#define NOMINMAX
#include <Windows.h>

#include "StdBase.h"
#include "N3Utils.h"

#include "N3Base/My_3DStruct.h"
#include "N3Base/N3Log.h"

// TODO: Since we now use spdlog, we can get ride of traces like these.
#if defined(_DEBUG)
#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define TRACE(fmt, ...) (void)fmt
#endif
