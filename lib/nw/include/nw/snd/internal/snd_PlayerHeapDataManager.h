#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/snd_SoundArchiveLoader.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal21PlayerHeapDataManagerE @ 0x008D0A34
// vtable 0x009032BC (vptr 0x009032C4), offset_to_top 0, 6 entries
// vtable 0x009032DC (vptr 0x009032E4), offset_to_top -12, 5 entries
class PlayerHeapDataManager : public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::SoundArchiveLoader
{
public:
    virtual ~PlayerHeapDataManager(); // 0x004C94E0 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004C9488 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
    virtual void InvalidateData(const void*, const void*); // 0x004C9378 slot 0x08 | nintendogs:bytes
    virtual void vf_0x0C(); // 0x004C93AC slot 0x0C | virtual slot, introduced by nw::snd::internal::PlayerHeapDataManager
    virtual void GetFileAddressFromTable(unsigned int) const; // 0x0074098C slot 0x10 | slot vf_0x10 of nw::snd::internal::PlayerHeapDataManager
    virtual void vf_0x14(); // 0x00740980 slot 0x14 | virtual slot, introduced by nw::snd::internal::PlayerHeapDataManager
    void Initialize(const nw::snd::SoundArchive*); // 0x004C8ADC | nintendogs:bytes [tier A]
    void Finalize(); // 0x004C9418 | nintendogs:bytes [tier A]
    PlayerHeapDataManager(); // 0x004C9444 | nintendogs:bytes [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
