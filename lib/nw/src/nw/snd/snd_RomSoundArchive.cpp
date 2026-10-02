#include "nw/snd/internal/snd_FsSoundArchiveBase.h"
#include "nw/snd/snd_RomSoundArchive.h"

namespace nw {
namespace snd {
// 0x004C05A8 slot 0x00 | slot vf_0x00 of nw::snd::SoundArchive
nw::snd::RomSoundArchive::~RomSoundArchive()
{
}

// 0x004C0598 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::RomSoundArchive::vf_0x04()
{
}

// 0x004C057C | fefates:bytes [tier B]
nw::snd::RomSoundArchive::RomSoundArchive()
{
}

// 0x004C85D4 | mk7dlp:callseq [tier A]
void nw::snd::RomSoundArchive::LoadHeader(void*, unsigned long)
{
}

// 0x004C86E4 | mk7dlp:callseq [tier A]
void nw::snd::RomSoundArchive::LoadLabelStringData(void*, unsigned long)
{
}

} // namespace snd
} // namespace nw
