#include "nw/snd/snd_SoundMemoryAllocatable.h"
#include "nw/snd/snd_SoundHeap.h"

namespace nw {
namespace snd {
// 0x004D50D4 slot 0x00 | nintendogs:bytes
nw::snd::SoundHeap::~SoundHeap()
{
}

// 0x004D502C slot 0x04 | virtual slot, introduced by nw::snd::SoundHeap
void nw::snd::SoundHeap::vf_0x04()
{
}

// 0x004D4F2C slot 0x08 | nintendogs:bytes
void nw::snd::SoundHeap::Alloc(unsigned)
{
}

// 0x0012BF60 | nintendogs:bytes [tier B]
void nw::snd::SoundHeap::SaveState()
{
}

// 0x001327F4 | nintendogs:bytes [tier A]
void nw::snd::SoundHeap::Create(void*, unsigned)
{
}

// 0x00132834 | nintendogs:bytes [tier A]
nw::snd::SoundHeap::SoundHeap()
{
}

// 0x0013C46C | nintendogs:bytes [tier B]
void nw::snd::SoundHeap::LoadState(int)
{
}

// 0x0013F564 | nintendogs:bytes [tier A]
void nw::snd::SoundHeap::Destroy()
{
}

// 0x004D4EE8 | fefates:bytes [tier B]
void nw::snd::SoundHeap::DisposeCallbackFunc(void*, unsigned long, void*)
{
}

// 0x004D4F90 | nintendogs:bytes [tier B]
void nw::snd::SoundHeap::Alloc(unsigned, void(*)(void*, unsigned long, void*), void*)
{
}

} // namespace snd
} // namespace nw
