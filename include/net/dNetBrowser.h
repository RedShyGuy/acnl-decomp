#pragma once

#include "decomp.h"
#include "sead/seadSafeStringBase.h"

namespace net {
// The internet browser (library applet) with the support page of Nintendo: the game requests it, the
// main loop starts it (names are ours).
bool IsBrowserAvailable(); // 0x0051B6F4
// starts the browser if it is requested (the camera of ModuleAmiiboCamera stops around it)
void UpdateBrowser(); // 0x0051B588
// no request is waiting
bool IsBrowserIdle(); // 0x0051B6F8
void RequestBrowser(); // 0x0051B70C

extern bool s_IsBrowserRequested;
extern const sead::SafeStringBase<char> s_BrowserUrl;
} // namespace net
