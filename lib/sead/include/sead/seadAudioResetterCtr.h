#pragma once

#include "decomp.h"
#include "sead/seadAudioResetter.h"

namespace sead {
// RTTI N4sead16AudioResetterCtrE @ 0x008D1AFC
// vtable 0x00905E14 (vptr 0x00905E1C), offset_to_top 0, 11 entries
class AudioResetterCtr : public ::sead::AudioResetter
{
public:
    virtual void vf_0x00(); // 0x005468E4 slot 0x00 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x04(); // 0x005468E0 slot 0x04 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x08(); // 0x0054177C slot 0x08 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x0C(); // 0x005466E4 slot 0x0C | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x10(); // 0x00546804 slot 0x10 | virtual slot, introduced by sead::AudioResetter
    virtual void isResetting() const; // 0x0074C7AC slot 0x14 | nintendogs:bytes
    virtual void vf_0x18(); // 0x0074C780 slot 0x18 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x1C(); // 0x0054662C slot 0x1C | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x20(); // 0x0054689C slot 0x20 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x24(); // 0x0074C7FC slot 0x24 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x28(); // 0x0074C7D0 slot 0x28 | virtual slot, introduced by sead::AudioResetter
    AudioResetterCtr(); // 0x0012CAA4 | nintendogs:bytes [tier A]
};
} // namespace sead
