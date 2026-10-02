#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd14FsSoundArchiveE @ 0x008D08E8
// vtable 0x00902EFC (vptr 0x00902F04), offset_to_top 0, 6 entries
class FsSoundArchive : public ::nw::snd::internal::FsSoundArchiveBase
{
public:
    FsSoundArchive(); // ctor candidate(s) 0x004C0358 (unverified)
    virtual ~FsSoundArchive(); // 0x004C8A5C slot 0x00 | slot vf_0x00 of nw::snd::SoundArchive
    virtual void vf_0x04(); // 0x004C0374 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
};
} // namespace snd
} // namespace nw
