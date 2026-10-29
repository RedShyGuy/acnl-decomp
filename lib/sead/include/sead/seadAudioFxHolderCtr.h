#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nn/snd/CTR/snd_FxDelay.h"
#include "nn/snd/CTR/snd_FxReverb.h"
#include "nw/snd/snd_FxDelay.h"
#include "nw/snd/snd_FxReverb.h"
#include "sead/seadAudioDspFxCtr.h"
#include "sead/seadAudioFxHolder.h"

namespace sead {
// Instantiations found in the binary:
//   sead::AudioFxHolderCtr<nn::snd::CTR::FxDelay, nn::snd::CTR::FxDelay::Param>  typeinfo 0x008D1AAC  vtable 0x00905D2C
//   sead::AudioFxHolderCtr<nn::snd::CTR::FxReverb, nn::snd::CTR::FxReverb::Param>  typeinfo 0x008D1AB8  vtable 0x00905D50
//   sead::AudioFxHolderCtr<nw::snd::FxDelay, nw::snd::FxDelay::Param>  typeinfo 0x008D1AC4  vtable 0x00905D74
//   sead::AudioFxHolderCtr<nw::snd::FxReverb, nw::snd::FxReverb::Param>  typeinfo 0x008D1AD0  vtable 0x00905D98
//   sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param>, nn::snd::CTR::DspFxDelay::Param>  typeinfo 0x008D1ADC  vtable 0x00905DBC
//   sead::AudioFxHolderCtr<sead::AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param>, nn::snd::CTR::DspFxReverb::Param>  typeinfo 0x008D1AE8  vtable 0x00905DE0
//
// The holder of an effect of nn::snd or nw::snd that it deletes. The functions of the effects with a
// work buffer are called directly (the original falls into them with a nop); the DSP effects keep a
// pointer to their parameters (explicit specializations in the .cpp; member name is ours).
template <typename T, typename TParam>
class AudioFxHolderCtr : public AudioFxHolder
{
public:
    explicit AudioFxHolderCtr(T* fx) : AudioFxHolder(), mFx(fx) {}
    virtual ~AudioFxHolderCtr()
    {
        if (mFx != nullptr) {
            delete mFx;
        }
    }
    virtual u32 getRequiredMemSize() { return mFx->GetRequiredMemSize(); }
    virtual bool setParam(const void* param) { return mFx->SetParam(*static_cast<const TParam*>(param)); }
    virtual bool assignWorkBuffer(uptr buffer, u32 size) { return mFx->AssignWorkBuffer(buffer, size); }
    virtual void releaseWorkBuffer() { mFx->ReleaseWorkBuffer(); }
    virtual void* getFx() { return mFx; }

    T* mFx; // 0x4
};

typedef AudioDspFxCtr<nn::snd::CTR::DspFxDelay, nn::snd::CTR::DspFxDelay::Param> AudioDspFxDelayCtr;
typedef AudioDspFxCtr<nn::snd::CTR::DspFxReverb, nn::snd::CTR::DspFxReverb::Param> AudioDspFxReverbCtr;

// the DSP effects (in the .cpp)
template <>
u32 AudioFxHolderCtr<AudioDspFxDelayCtr, nn::snd::CTR::DspFxDelay::Param>::getRequiredMemSize();
template <>
bool AudioFxHolderCtr<AudioDspFxDelayCtr, nn::snd::CTR::DspFxDelay::Param>::setParam(const void* param);
template <>
void AudioFxHolderCtr<AudioDspFxDelayCtr, nn::snd::CTR::DspFxDelay::Param>::releaseWorkBuffer();
template <>
u32 AudioFxHolderCtr<AudioDspFxReverbCtr, nn::snd::CTR::DspFxReverb::Param>::getRequiredMemSize();
template <>
bool AudioFxHolderCtr<AudioDspFxReverbCtr, nn::snd::CTR::DspFxReverb::Param>::setParam(const void* param);
template <>
void AudioFxHolderCtr<AudioDspFxReverbCtr, nn::snd::CTR::DspFxReverb::Param>::releaseWorkBuffer();
} // namespace sead
