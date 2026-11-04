#include "net/dNetBrowser.h"

#include "ambcam/dCameraImports.h"
#include "applet/dAppletState.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/nstd/nstd_String.h"
#include "nn/os/os_Api.h"

namespace net {
// 0x00954660
bool s_IsBrowserRequested;
// 0x00954664
const sead::SafeStringBase<char> s_BrowserUrl("https://i.nintendo.net/");
// (builds s_BrowserUrl)
// 0x0079ECE4 (static initializer)

// 0x0051B588 (name is ours)
void UpdateBrowser()
{
    if (!s_IsBrowserRequested) {
        return;
    }
    bool isCameraActive = applet::IsStateAtLeast2();
    if (isCameraActive) {
        while (!ambcam::StopCamera()) {
            nn::os::YieldThread();
        }
    }
    char url[1024];
    url[sizeof(url) - 1] = sead::SafeStringBase<char>::s_NullChar;
    const char* src = s_BrowserUrl.getCString();
    if (url != src) {
        s32 length = s_BrowserUrl.getLength();
        if (length >= static_cast<s32>(sizeof(url))) {
            length = sizeof(url) - 1;
        }
        nnnstdMemCpy(url, src, length);
        url[length] = sead::SafeStringBase<char>::s_NullChar;
    }
    nn::applet::CTR::StartInternetBrowser(url);
    s_IsBrowserRequested = false;
    applet::RestoreGpuState();
    if (!isCameraActive) {
        return;
    }
    if (nn::applet::CTR::detail::GetOrderToCloseState() != 0 || nn::applet::CTR::IsReceivedWakeupByCancel()) {
        ambcam::DestroyCameraThread();
        return;
    }
    while (!ambcam::RestartCamera()) {
        nn::os::YieldThread();
    }
}

// 0x0051B6F4 (name is ours)
bool IsBrowserAvailable()
{
    return nn::applet::CTR::IsInternetBrowserAvailable();
}

// 0x0051B6F8 (name is ours)
bool IsBrowserIdle()
{
    return !s_IsBrowserRequested;
}

// 0x0051B70C (name is ours)
void RequestBrowser()
{
    if (nn::applet::CTR::IsInternetBrowserAvailable()) {
        s_IsBrowserRequested = true;
    }
}
} // namespace net
