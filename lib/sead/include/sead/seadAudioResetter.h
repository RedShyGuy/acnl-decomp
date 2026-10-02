#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead13AudioResetterE @ 0x008D15EC
// vtable 0x00905318 (vptr 0x00905320), offset_to_top 0, 11 entries
class AudioResetter
{
public:
    AudioResetter(); // ctor candidate(s) 0x00132F80 (unverified)
    virtual void vf_0x00(); // 0x005418BC slot 0x00 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x04(); // 0x005418B8 slot 0x04 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x08(); // 0x00541780 slot 0x08 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x005417E8 slot 0x10 | virtual slot, introduced by sead::AudioResetter
    virtual void isResetting() const; // 0x0074B164 slot 0x14 | nintendogs:callgraph
    virtual void vf_0x18(); // 0x0074B0F8 slot 0x18 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x1C(); // 0x00541788 slot 0x1C | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x20(); // 0x00541850 slot 0x20 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x24(); // 0x0074B244 slot 0x24 | virtual slot, introduced by sead::AudioResetter
    virtual void vf_0x28(); // 0x0074B1D8 slot 0x28 | virtual slot, introduced by sead::AudioResetter
};
} // namespace sead
