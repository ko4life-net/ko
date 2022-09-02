#pragma once

#define _IsKeyDown(exp)   (GetAsyncKeyState(exp) & 0xff00)
#define _IsKeyDowned(exp) (GetAsyncKeyState(exp) & 0x00ff)
