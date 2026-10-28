#include "nw/io/io_FileStream.h"

namespace nw {
namespace io {
// 0x009828A8
const TypeInfo nw::io::FileStream::s_TypeInfo(&IOStream::s_TypeInfo);

// sets s_TypeInfo
// 0x00797B08 (static initializer)

// 0x007375A0
const nw::io::TypeInfo* nw::io::FileStream::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0048B8C0
void nw::io::FileStream::vf_0x44()
{
    // empty (in the original too)
}

// 0x0048B814
bool nw::io::FileStream::vf_0x48(u32, u32)
{
    return true;
}

} // namespace io
} // namespace nw
