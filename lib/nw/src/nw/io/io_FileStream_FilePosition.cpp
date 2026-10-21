#include "nw/io/io_FileStream_FilePosition.h"

namespace {
// the value within min and max (inline; name is ours)
template <typename T>
inline T Clamp(T value, T min, T max)
{
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }
    return value;
}
} // namespace

// 0x0048B81C | nintendogs:bytes [tier A]
void nw::io::FileStream::FilePosition::Seek(s32 offset, u32 origin)
{
    switch (origin) {
    case SEEK_ORIGIN_BEGIN:
        m_Position = 0;
        break;
    case SEEK_ORIGIN_CURRENT:
        break;
    case SEEK_ORIGIN_END:
        m_Position = m_FileSize;
        break;
    default:
        break;
    }
    Skip(offset);
}

// 0x0048B87C | nintendogs:bytes [tier A]
u32 nw::io::FileStream::FilePosition::Skip(s32 offset)
{
    if (offset != 0) {
        // (the sum is unsigned, the clamp is done in 64 bits)
        s64 position = static_cast<u32>(m_Position + offset);
        m_Position = static_cast<u32>(Clamp<s64>(position, 0, m_FileSize));
    }
    return m_Position;
}
