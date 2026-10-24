#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"

namespace nw {
namespace snd {
namespace internal {
// A file stream that reads another one through a cache buffer (member names are ours). Without a
// cache buffer everything goes to the other stream.
// RTTI N2nw3snd8internal16CachedFileStreamE @ 0x008D09E4
// vtable 0x00903184 (vptr 0x0090318C), offset_to_top 0, 22 entries
class CachedFileStream : public ::nw::io::FileStream
{
public:
    CachedFileStream(nw::io::FileStream& stream); // 0x004C84C8 | fefates:bytes [tier B]
    virtual ~CachedFileStream(); // 0x004C850C slot 0x04

    virtual bool IsAvailable() const { return m_pStream->IsAvailable(); } // 0x0073FC9C slot 0x0C
    virtual bool vf_0x10() const { return m_pStream->vf_0x10(); } // 0x0073FCDC slot 0x10
    virtual bool vf_0x14() const { return m_pStream->vf_0x14(); } // 0x0073FCCC slot 0x14
    virtual s32 Read(void* pBuffer, u32 size); // 0x004C82E4 slot 0x24 | fefates:bytes
    virtual void Close(); // 0x004C84A8 slot 0x38 | slot vf_0x38 of nw::snd::internal::CachedFileStream
    virtual u32 GetSize() const { return m_pStream->GetSize(); } // 0x0073FCBC slot 0x3C
    virtual void Seek(s32 offset, u32 origin); // 0x004C8430 slot 0x40 | fefates:bytes
    virtual void vf_0x44(); // 0x004C84B8 slot 0x44
    virtual bool vf_0x48(u32 arg0, u32 arg1); // 0x004C82C0 slot 0x48
    virtual bool CanSeek() const { return m_pStream->CanSeek(); } // 0x0073FCAC slot 0x4C
    virtual bool vf_0x50() const { return m_pStream->vf_0x50(); } // 0x0073FCEC slot 0x50
    virtual u32 Tell() const; // 0x0073FC70 slot 0x54 | fefates:bytes

    void SetCacheBuffer(void* pCacheBuffer, u32 cacheSize); // 0x004C82D0 | fefates:bytes [tier B]

    // (inline)
    bool IsCacheAvailable() const { return m_pCacheBuffer != nullptr && m_CacheSize != 0; }

    nw::io::FileStream* m_pStream; // 0x14
    void* m_pCacheBuffer;          // 0x18
    u32 m_CacheSize;               // 0x1C
    s32 m_CachePosition;           // 0x20 the file position of the cache (-1: empty)
    s32 m_Position;                // 0x24 the position when there is a cache
};
ASSERT_SIZE(CachedFileStream, 0x28);
} // namespace internal
} // namespace snd
} // namespace nw
