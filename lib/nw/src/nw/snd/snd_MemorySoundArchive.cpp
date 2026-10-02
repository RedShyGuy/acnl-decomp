#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/snd_MemorySoundArchive.h"

namespace nw {
namespace snd {
// ctor candidate(s) 0x004C0A74 (unverified)
nw::snd::MemorySoundArchive::MemorySoundArchive()
{
}

// 0x004C0AEC slot 0x00 | slot vf_0x00 of nw::snd::SoundArchive
nw::snd::MemorySoundArchive::~MemorySoundArchive()
{
}

// 0x004C0AC8 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::MemorySoundArchive::vf_0x04()
{
}

// 0x0073EFC0 slot 0x08 | nintendogs:callseq
void nw::snd::MemorySoundArchive::vf_0x08()
{
}

// 0x0073F0FC slot 0x0C | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::MemorySoundArchive::vf_0x0C()
{
}

// 0x004C0A00 slot 0x10 | slot vf_0x10 of nw::snd::SoundArchive
void nw::snd::MemorySoundArchive::OpenStream(void*, int, unsigned int, unsigned int)
{
}

// 0x0073EEB0 slot 0x14 | slot vf_0x14 of nw::snd::SoundArchive
void nw::snd::MemorySoundArchive::OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const
{
}

// 0x004C0988 | nintendogs:callgraph [tier A]
void nw::snd::MemorySoundArchive::Initialize(const void*)
{
}

} // namespace snd
} // namespace nw
