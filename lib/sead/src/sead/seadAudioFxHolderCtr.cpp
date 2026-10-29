#include "sead/seadAudioFxHolderCtr.h"

// The holders of the effects: the functions that call a function of the effect sit in front of it in the
// original (they fall into it with a nop); the others are with the type information.
namespace sead {
// 0x00460880
template <>
u32 sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::getRequiredMemSize()
{
    return nn::snd::CTR::DspFxDelay::GetRequiredMemSize(*mFx->mParam);
}

// 0x00812534
template <>
bool sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::setParam(const void* param)
{
    mFx->mParam = static_cast<const nn::snd::CTR::DspFxDelay::Param*>(param);
    return true;
}

// 0x00812528
template <>
void sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::releaseWorkBuffer()
{
    // empty (in the original too)
}

// 0x00460DE4
template <>
u32 sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::getRequiredMemSize()
{
    return nn::snd::CTR::DspFxReverb::GetRequiredMemSize(*mFx->mParam);
}

// 0x008125BC
template <>
bool sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::setParam(const void* param)
{
    mFx->mParam = static_cast<const nn::snd::CTR::DspFxReverb::Param*>(param);
    return true;
}

// 0x008125B0
template <>
void sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::releaseWorkBuffer()
{
    // empty (in the original too)
}

} // namespace sead

// 0x0081238C
// 0x00812350 (deleting dtor)
template sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::~AudioFxHolderCtr();
// 0x004655C0
template u32 sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::getRequiredMemSize();
// 0x00465630
template bool sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::setParam(const void*);
// 0x0046558C
template bool sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::assignWorkBuffer(uptr, u32);
// 0x004655AC
template void sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::releaseWorkBuffer();
// 0x00812348
template void* sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>::getFx();

// 0x00812404
// 0x008123C8 (deleting dtor)
template sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::~AudioFxHolderCtr();
// 0x00465EC8
template u32 sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::getRequiredMemSize();
// 0x00465FBC
template bool sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::setParam(const void*);
// 0x00465E94
template bool sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::assignWorkBuffer(uptr, u32);
// 0x00465EB4
template void sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::releaseWorkBuffer();
// 0x008123C0
template void* sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>::getFx();

// 0x0081247C
// 0x00812440 (deleting dtor)
template sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::~AudioFxHolderCtr();
// 0x004C3C14
template u32 sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::getRequiredMemSize();
// 0x004C3C84
template bool sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::setParam(const void*);
// 0x004C3BE0
template bool sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::assignWorkBuffer(uptr, u32);
// 0x004C3C00
template void sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::releaseWorkBuffer();
// 0x00812438
template void* sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>::getFx();

// 0x008124F4
// 0x008124B8 (deleting dtor)
template sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::~AudioFxHolderCtr();
// 0x004C44C8
template u32 sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::getRequiredMemSize();
// 0x004C45C4
template bool sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::setParam(const void*);
// 0x004C4494
template bool sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::assignWorkBuffer(uptr, u32);
// 0x004C44B4
template void sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::releaseWorkBuffer();
// 0x008124B0
template void* sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>::getFx();

// 0x00812580
// 0x00812548 (deleting dtor)
template sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::~AudioFxHolderCtr();
// 0x004607AC
template bool sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::assignWorkBuffer(uptr, u32);
// 0x0081252C
template void* sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>::getFx();

// 0x00812608
// 0x008125D0 (deleting dtor)
template sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::~AudioFxHolderCtr();
// 0x00460D10
template bool sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::assignWorkBuffer(uptr, u32);
// 0x008125B4
template void* sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>::getFx();

