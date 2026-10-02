#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd18MemorySoundArchiveE @ 0x008D0928
// vtable 0x00902F78 (vptr 0x00902F80), offset_to_top 0, 6 entries
class MemorySoundArchive : public ::nw::snd::SoundArchive
{
public:
    MemorySoundArchive(); // ctor candidate(s) 0x004C0A74 (unverified)
    virtual ~MemorySoundArchive(); // 0x004C0AEC slot 0x00 | slot vf_0x00 of nw::snd::SoundArchive
    virtual void vf_0x04(); // 0x004C0AC8 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
    virtual void vf_0x08(); // 0x0073EFC0 slot 0x08 | nintendogs:callseq
    virtual void vf_0x0C(); // 0x0073F0FC slot 0x0C | virtual slot, introduced by nw::snd::SoundArchive
    virtual void OpenStream(void*, int, unsigned int, unsigned int); // 0x004C0A00 slot 0x10 | slot vf_0x10 of nw::snd::SoundArchive
    virtual void OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const; // 0x0073EEB0 slot 0x14 | slot vf_0x14 of nw::snd::SoundArchive
    void Initialize(const void*); // 0x004C0988 | nintendogs:callgraph [tier A]
};
} // namespace snd
} // namespace nw
