#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

namespace nw {
namespace snd {
// A sound archive in the ROM file system (errors are fatal); the game loads the info and string blocks into
// its memory (the class and function names are from the reference symbols).
// RTTI N2nw3snd15RomSoundArchiveE @ 0x008D08FC
// vtable 0x00902F1C (vptr 0x00902F24), offset_to_top 0, 6 entries
class RomSoundArchive : public ::nw::snd::internal::FsSoundArchiveBase
{
public:
    RomSoundArchive(); // 0x004C057C | fefates:bytes [tier B]
    // 0x004C0598 (deleting dtor)
    virtual ~RomSoundArchive(); // 0x004C05A8 slot 0x00

    bool LoadHeader(void* pBuffer, u32 size); // 0x004C85D4 | mk7dlp:callseq [tier A]
    bool LoadLabelStringData(void* pBuffer, u32 size); // 0x004C86E4 | mk7dlp:callseq [tier A]
};
} // namespace snd
} // namespace nw
