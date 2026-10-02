#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd15RomSoundArchiveE @ 0x008D08FC
// vtable 0x00902F1C (vptr 0x00902F24), offset_to_top 0, 6 entries
class RomSoundArchive : public ::nw::snd::internal::FsSoundArchiveBase
{
public:
    virtual ~RomSoundArchive(); // 0x004C05A8 slot 0x00 | slot vf_0x00 of nw::snd::SoundArchive
    virtual void vf_0x04(); // 0x004C0598 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
    RomSoundArchive(); // 0x004C057C | fefates:bytes [tier B]
    void LoadHeader(void*, unsigned long); // 0x004C85D4 | mk7dlp:callseq [tier A]
    void LoadLabelStringData(void*, unsigned long); // 0x004C86E4 | mk7dlp:callseq [tier A]
};
} // namespace snd
} // namespace nw
