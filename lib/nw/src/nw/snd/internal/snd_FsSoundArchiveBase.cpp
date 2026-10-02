#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

namespace nw {
namespace snd {
namespace internal {
// ctor candidate(s) 0x004C8948 (unverified)
nw::snd::internal::FsSoundArchiveBase::FsSoundArchiveBase()
{
}

// 0x004C8A60 slot 0x00 | fefates:bytes
nw::snd::internal::FsSoundArchiveBase::~FsSoundArchiveBase()
{
}

// 0x004C89D8 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::internal::FsSoundArchiveBase::vf_0x04()
{
}

// 0x007406E8 slot 0x08 | slot vf_0x08 of nw::snd::SoundArchive
void nw::snd::internal::FsSoundArchiveBase::vf_0x08()
{
}

// 0x007406F0 slot 0x0C | virtual slot, introduced by nw::snd::SoundArchive
void nw::snd::internal::FsSoundArchiveBase::vf_0x0C()
{
}

// 0x004C867C slot 0x10 | fefates:bytes
void nw::snd::internal::FsSoundArchiveBase::OpenStream(void*, int, unsigned int, unsigned int)
{
}

// 0x007405CC slot 0x14 | fefates:bytes
void nw::snd::internal::FsSoundArchiveBase::OpenExtStream(void*, int, const char*, void*, unsigned int, nw::snd::SoundArchive::FileStreamPriority) const
{
}

// 0x004C8794 | fefates:bytes [tier B]
void nw::snd::internal::FsSoundArchiveBase::FsErrorCallbackHandler(nn::Result, void*)
{
}

// 0x004C87C4 | fefates:bytes [tier B]
void nw::snd::internal::FsSoundArchiveBase::Open(const char*)
{
}

// 0x004C8910 | fefates:bytes-fuzzy [tier B]
void nw::snd::internal::FsSoundArchiveBase::Close()
{
}

// 0x004C8948 | fefates:bytes [tier B]
nw::snd::internal::FsSoundArchiveBase::FsSoundArchiveBase(bool)
{
}

} // namespace internal
} // namespace snd
} // namespace nw
