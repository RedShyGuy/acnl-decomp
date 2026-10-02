#include "nw/snd/internal/snd_FrameHeap.h"

namespace nw {
namespace snd {
namespace internal {
// 0x001327AC | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::SaveState()
{
}

// 0x00138540 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::Create(void*, unsigned)
{
}

// 0x001385C4 | nintendogs:bytes [tier A]
nw::snd::internal::FrameHeap::FrameHeap()
{
}

// 0x0013F3FC | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::FrameHeap::LoadState(int)
{
}

// 0x0013F4D8 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::NewSection()
{
}

// 0x00141964 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::ClearSection()
{
}

// 0x001419FC | nintendogs:callgraph [tier A]
void nw::snd::internal::FrameHeap::ProcessCallback(int)
{
}

// 0x00141AFC | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::FrameHeap::Clear()
{
}

// 0x00141B20 | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::Destroy()
{
}

// 0x004D4B3C | nintendogs:bytes [tier A]
void nw::snd::internal::FrameHeap::Alloc(unsigned, void(*)(void*, unsigned long, void*), void*)
{
}

// 0x004D4BAC | nintendogs:bytes [tier A]
nw::snd::internal::FrameHeap::~FrameHeap()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
