#include "mw/qrd/qrd_Internal.h"

#include "nn/os/os_Thread.h"

namespace mw {
namespace qrd {
namespace detail {

// 0x00163CFC
bool PrepareImage(Params* params, const ImageInfo* image, SymbolPosition* position)
{
    Yield();
    s_SampleState.moduleSize = 0.0f;
    // the luminance image stays from the previous attempt
    if (s_GrayImage.pixels == NULL)
    {
        s_SourceImage.width = image->width;
        s_SourceImage.height = image->height;
        s_SourceImage.pixels = image->pixels;
        if (!ExtractLuminance(&s_SourceImage, &s_GrayImage, params, &image->rect))
        {
            return false;
        }
    }
    return Binarize(params, &s_GrayImage, &s_Context.bitImage, position) != 0;
}

// 0x00163D94
bool UnmaskSymbol()
{
    Yield();
    if (Unmask(&s_Context.matrix, &s_Context.format))
    {
        s_Context.symbolSize = s_Context.matrix.size;
        return true;
    }
    ReleaseModules();
    return false;
}

// 0x00163EE0
DECOMP_NOINLINE void Yield()
{
    if (s_WorkState.isYieldDisabled)
    {
        return;
    }
    if (s_WorkState.isYieldRequested)
    {
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(16));
        s_WorkState.isYieldRequested = 0;
    }
}

// 0x00163F28
bool Cleanup()
{
    Yield();
    ResetContext(&s_Context);
    return true;
}

} // namespace detail
} // namespace qrd
} // namespace mw
