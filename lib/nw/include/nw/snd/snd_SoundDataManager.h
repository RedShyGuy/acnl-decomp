#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/snd_SoundArchiveLoader.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd16SoundDataManagerE @ 0x008D0908
// vtable 0x00902F3C (vptr 0x00902F44), offset_to_top 0, 6 entries
// vtable 0x00902F5C (vptr 0x00902F64), offset_to_top -12, 5 entries
class SoundDataManager : public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::SoundArchiveLoader
{
public:
    virtual ~SoundDataManager(); // 0x004C089C slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004C0884 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
    virtual void InvalidateData(const void*, const void*); // 0x004C06B8 slot 0x08 | nintendogs:callseq
    virtual void SetFileAddressToTable(unsigned, const void*); // 0x004C0800 slot 0x0C | mk7dlp:bytes
    virtual void GetFileAddressFromTable(unsigned) const; // 0x0073EE84 slot 0x10 | nintendogs:bytes
    virtual void GetFileAddressImpl(unsigned int) const; // 0x0073EDF4 slot 0x14 | fefates:bytes
    SoundDataManager(); // 0x00137C74 | nintendogs:bytes [tier A]
    void Initialize(const nw::snd::SoundArchive*, void*, unsigned); // 0x004C05AC | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004C0824 | nintendogs:callseq [tier A]
    void GetRequiredMemSize(const nw::snd::SoundArchive*) const; // 0x0073EE58 | nintendogs:bytes [tier A]
};
} // namespace snd
} // namespace nw
