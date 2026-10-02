#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class Bank
{
public:
    void CalcChannelVelocityVolume(unsigned char); // 0x004D3C58 | fefates:bytes [tier B]
    void NoteOn(const void*, const nw::snd::internal::driver::NoteOnInfo&, const nw::snd::SoundArchive&, const nw::snd::SoundArchivePlayer&, const nw::snd::internal::PlayerHeapDataManager*) const; // 0x0074168C | nintendogs:callseq [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
