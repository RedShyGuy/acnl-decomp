#include "nw/snd/snd_SoundMemoryAllocatable.h"
#include "nw/snd/internal/snd_PlayerHeap.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C5EC4 slot 0x00 | mk7dlp:callseq
void nw::snd::internal::PlayerHeap::vf_0x00()
{
}

// 0x004C5EA4 slot 0x04 | virtual slot, introduced by nw::snd::internal::PlayerHeap
void nw::snd::internal::PlayerHeap::vf_0x04()
{
}

// 0x004C5E0C slot 0x08 | nintendogs:bytes
void nw::snd::internal::PlayerHeap::Alloc(unsigned)
{
}

// 0x004C5E44 | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeap::Create(void*, unsigned)
{
}

// 0x004C5E70 | fefates:bytes [tier B]
nw::snd::internal::PlayerHeap::PlayerHeap()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
