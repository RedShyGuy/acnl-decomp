#include "nw/snd/internal/snd_CachedFileStream.h"

#include <cstring>

namespace nw {
namespace snd {
namespace internal {
// 0x004C82C0 slot 0x48
bool nw::snd::internal::CachedFileStream::vf_0x48(u32 arg0, u32 arg1)
{
    return m_pStream->vf_0x48(arg0, arg1);
}

// 0x004C82D0 | fefates:bytes [tier B]
void nw::snd::internal::CachedFileStream::SetCacheBuffer(void* pCacheBuffer, u32 cacheSize)
{
    m_pCacheBuffer = pCacheBuffer;
    m_CacheSize = cacheSize;
    m_CachePosition = -1;
    m_Position = -1;
}

// 0x004C82E4 slot 0x24 | fefates:bytes
s32 nw::snd::internal::CachedFileStream::Read(void* pBuffer, u32 size)
{
    if (!IsCacheAvailable()) {
        return m_pStream->Read(pBuffer, size);
    }

    // in the cache
    if (m_CachePosition >= 0 && m_Position >= m_CachePosition &&
        static_cast<u32>(m_CachePosition) + m_CacheSize >= static_cast<u32>(m_Position) + size) {
        std::memcpy(pBuffer, static_cast<u8*>(m_pCacheBuffer) + (m_Position - m_CachePosition), size);
        m_Position += size;
        return size;
    }

    // fill the cache from the position
    m_pStream->Seek(m_Position, SEEK_ORIGIN_BEGIN);
    m_CachePosition = Tell();
    if (size <= m_CacheSize) {
        const s32 result = m_pStream->Read(m_pCacheBuffer, m_CacheSize);
        if (result < 0) {
            m_CachePosition = -1;
            return result;
        }
        std::memcpy(pBuffer, m_pCacheBuffer, size);
    } else {
        const s32 result = m_pStream->Read(pBuffer, size);
        if (result < 0) {
            m_CachePosition = -1;
            return result;
        }
        std::memcpy(m_pCacheBuffer, pBuffer, m_CacheSize);
    }
    m_Position += size;
    return size;
}

// 0x004C8430 slot 0x40 | fefates:bytes
void nw::snd::internal::CachedFileStream::Seek(s32 offset, u32 origin)
{
    if (!IsCacheAvailable()) {
        m_pStream->Seek(offset, origin);
        return;
    }
    switch (origin) {
    case SEEK_ORIGIN_BEGIN:
        m_Position = offset;
        break;
    case SEEK_ORIGIN_CURRENT:
        m_Position += offset;
        break;
    case SEEK_ORIGIN_END:
        m_Position = GetSize() - offset;
        break;
    }
}

// 0x004C84A8 slot 0x38 | slot vf_0x38 of nw::snd::internal::CachedFileStream
void nw::snd::internal::CachedFileStream::Close()
{
    m_pStream->Close();
}

// 0x004C84B8 slot 0x44
void nw::snd::internal::CachedFileStream::vf_0x44()
{
    m_pStream->vf_0x44();
}

// 0x004C84C8 | fefates:bytes [tier B]
nw::snd::internal::CachedFileStream::CachedFileStream(nw::io::FileStream& stream)
    : m_pStream(&stream), m_pCacheBuffer(nullptr), m_CacheSize(0), m_CachePosition(-1), m_Position(-1)
{
    m_IsAvailable = true;
}

// 0x004C850C
// 0x004C8508 (deleting dtor)
nw::snd::internal::CachedFileStream::~CachedFileStream()
{
}

// 0x0073FC70 slot 0x54 | fefates:bytes
u32 nw::snd::internal::CachedFileStream::Tell() const
{
    if (IsCacheAvailable()) {
        return m_Position;
    }
    return m_pStream->Tell();
}

} // namespace internal
} // namespace snd
} // namespace nw
