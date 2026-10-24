#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal18FsSoundArchiveBaseE @ 0x008D0A08
// vtable 0x00903258 (vptr 0x00903260), offset_to_top 0, 6 entries
class FsSoundArchiveBase : public ::nw::snd::SoundArchive
{
public:
    FsSoundArchiveBase(); // ctor candidate(s) 0x004C8948 (unverified)
    virtual ~FsSoundArchiveBase(); // 0x004C8A60 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004C89D8 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchive
    virtual const void* detail_GetFileAddress(u32 fileId) const; // 0x007406E8 slot 0x08 | slot vf_0x08 of nw::snd::SoundArchive
    virtual size_t detail_GetRequiredStreamBufferSize() const; // 0x007406F0 slot 0x0C | virtual slot, introduced by nw::snd::SoundArchive
    virtual io::FileStream* OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length); // 0x004C867C slot 0x10 | fefates:bytes
    virtual io::FileStream* OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer, u32 cacheSize, FileStreamPriority priority) const; // 0x007405CC slot 0x14 | fefates:bytes
    void FsErrorCallbackHandler(nn::Result, void*); // 0x004C8794 | fefates:bytes [tier B]
    void Open(const char*); // 0x004C87C4 | fefates:bytes [tier B]
    void Close(); // 0x004C8910 | fefates:bytes-fuzzy [tier B]
    FsSoundArchiveBase(bool); // 0x004C8948 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
