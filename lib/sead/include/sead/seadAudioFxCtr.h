#pragma once

#include "decomp.h"
#include "sead/seadAudioFx.h"

namespace sead {
// RTTI N4sead10AudioFxCtrE @ 0x008D134C
// vtable 0x00904DF4 (vptr 0x00904DFC), offset_to_top 0, 4 entries
class AudioFxCtr : public ::sead::AudioFx
{
public:
    virtual void vf_0x00(); // 0x00749E14 slot 0x00 | virtual slot, introduced by sead::AudioFx
    virtual void vf_0x04(); // 0x00749DA0 slot 0x04 | virtual slot, introduced by sead::AudioFx
    virtual void vf_0x08(); // 0x00141B98 slot 0x08 | virtual slot, introduced by sead::AudioFx
    virtual void vf_0x0C(); // 0x0053E214 slot 0x0C | nintendogs:bytes
    AudioFxCtr(); // 0x00132CEC | nintendogs:bytes [tier B]
};
} // namespace sead
