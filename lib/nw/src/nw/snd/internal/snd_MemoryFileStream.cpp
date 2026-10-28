#include "nw/snd/internal/snd_MemoryFileStream.h"

#include <cstring>

namespace nw {
namespace snd {
namespace internal {
// 0x004C859C | fefates:bytes [tier B]
nw::snd::internal::MemoryFileStream::MemoryFileStream(const void* pData, u32 size) : m_pData(pData), m_Size(size), m_Position(0)
{
}

// 0x004C85C8
// 0x004C85C4 (deleting dtor)
nw::snd::internal::MemoryFileStream::~MemoryFileStream()
{
    // empty (in the original too)
}

// 0x004C8510 slot 0x24 (name after the other streams)
s32 nw::snd::internal::MemoryFileStream::Read(void* pBuffer, u32 size)
{
    u32 readSize = m_Size - m_Position;
    if (readSize >= size) {
        readSize = size;
    }
    std::memcpy(pBuffer, static_cast<const u8*>(m_pData) + m_Position, readSize);
    m_Position += readSize;
    return readSize;
}

// 0x004C8558 | nintendogs:bytes [tier A]
void nw::snd::internal::MemoryFileStream::Seek(s32 offset, u32 origin)
{
    switch (origin) {
    case SEEK_ORIGIN_BEGIN:
        m_Position = offset;
        break;
    case SEEK_ORIGIN_CURRENT:
        m_Position += offset;
        break;
    case SEEK_ORIGIN_END:
        m_Position = m_Size - offset;
        break;
    }
}

// 0x004C8588 | nintendogs:bytes [tier A]
void nw::snd::internal::MemoryFileStream::Close()
{
    m_pData = nullptr;
    m_Size = 0;
    m_Position = 0;
}

} // namespace internal
} // namespace snd
} // namespace nw
