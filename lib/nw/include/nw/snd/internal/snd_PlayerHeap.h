#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundMemoryAllocatable.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal10PlayerHeapE @ 0x008D0984
// vtable 0x00903040 (vptr 0x00903048), offset_to_top 0, 3 entries
class PlayerHeap : public ::nw::snd::SoundMemoryAllocatable
{
public:
    virtual void vf_0x00(); // 0x004C5EC4 slot 0x00 | mk7dlp:callseq
    virtual void vf_0x04(); // 0x004C5EA4 slot 0x04 | virtual slot, introduced by nw::snd::internal::PlayerHeap
    virtual void Alloc(unsigned); // 0x004C5E0C slot 0x08 | nintendogs:bytes
    void Create(void*, unsigned); // 0x004C5E44 | nintendogs:bytes [tier A]
    PlayerHeap(); // 0x004C5E70 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
