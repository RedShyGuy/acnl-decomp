#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal18FsSoundArchiveBaseE @ 0x008D0A08
// vtable 0x00903258 (vptr 0x00903260), offset_to_top 0, 6 entries
class FsSoundArchiveBase : public ::nw::snd::SoundArchive
{
public:
    FsSoundArchiveBase(); // ctor candidate(s) 0x004C8948 (unverified)
    virtual ~FsSoundArchiveBase(); // 0x004C8A60 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004C89D8 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
    virtual void vf_0x08(); // 0x007406E8 slot 0x08 | slot vf_0x08 of nw::snd::SoundArchive
    virtual void vf_0x0C(); // 0x007406F0 slot 0x0C | virtual slot, introduced by nw::snd::SoundArchive
    virtual void OpenStream(void*, int, unsigned int, unsigned int); // 0x004C867C slot 0x10 | fefates:bytes
    virtual void OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const; // 0x007405CC slot 0x14 | fefates:bytes
    void FsErrorCallbackHandler(nn::Result, void*); // 0x004C8794 | fefates:bytes [tier B]
    void Open(const char*); // 0x004C87C4 | fefates:bytes [tier B]
    void Close(); // 0x004C8910 | fefates:bytes-fuzzy [tier B]
    FsSoundArchiveBase(bool); // 0x004C8948 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
