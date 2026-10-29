#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioFx.h"

namespace sead {
// RTTI N4sead10AudioFxCtrE @ 0x008D134C
// vtable 0x00904DF4 (vptr 0x00904DFC), offset_to_top 0, 4 entries
// An effect of the 3DS: its type chooses the AppendEffect of nw::snd::SoundSystem (member names are ours).
class AudioFxCtr : public ::sead::AudioFx
{
public:
    // the kinds of effects
    enum Type : u8
    {
        TYPE_NW = 0,             // an nw::snd::FxBase
        TYPE_FX_REVERB = 3,      // nn::snd::CTR::FxReverb
        TYPE_FX_DELAY = 4,       // nn::snd::CTR::FxDelay
        TYPE_DSP_FX_REVERB = 5,  // nn::snd::CTR::DspFxReverb (with mDspParam)
        TYPE_DSP_FX_DELAY = 6,   // nn::snd::CTR::DspFxDelay (with mDspParam)
    };

    AudioFxCtr(); // 0x00132CEC | nintendogs:bytes [tier B]
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00749E14 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioFx::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00749DA0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098339C (guard 0x00983398)
        static const RuntimeTypeInfo::Derive<AudioFx> s_TypeInfo;
        return &s_TypeInfo;
    }
    // deletes the holder
    virtual ~AudioFxCtr(); // 0x00141B98 slot 0x08
    // 0x0053E214 slot 0x0C | nintendogs:bytes (deleting dtor)

    // whether the effect is no effect of nn::snd (those take a bus alone)
    DECOMP_EMIT_COPY bool isNw() const // 0x00749DEC (name is ours)
    {
        return mType != TYPE_FX_REVERB && mType != TYPE_FX_DELAY && mType != TYPE_DSP_FX_REVERB &&
               mType != TYPE_DSP_FX_DELAY;
    }

    const void* mDspParam; // 0x18, the parameters of a DSP effect
    Type mType;            // 0x1C
};
ASSERT_SIZE(AudioFxCtr, 0x20);
} // namespace sead
