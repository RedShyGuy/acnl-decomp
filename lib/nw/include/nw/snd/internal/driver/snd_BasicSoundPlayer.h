#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver16BasicSoundPlayerE @ 0x008D0AB8
// vtable 0x009033A8 (vptr 0x009033B0), offset_to_top 0, 7 entries
class BasicSoundPlayer
{
public:
    BasicSoundPlayer(); // ctor candidate(s) 0x004CE550 (unverified)
    virtual ~BasicSoundPlayer(); // 0x004CE5FC slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::BasicSoundPlayer
    // 0x004CE5EC slot 0x04 | slot vf_0x04 of nw::snd::internal::driver::BasicSoundPlayer (deleting dtor)
    virtual void Initialize(); // 0x004CE4C8 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004CE53C slot 0x0C | fefates:callgraph
    virtual void Start(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void Stop(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void Pause(bool); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
