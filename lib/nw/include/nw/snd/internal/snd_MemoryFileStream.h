#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"

namespace nw {
namespace snd {
namespace internal {
// A file stream over a block of memory (member names are ours).
// RTTI N2nw3snd8internal16MemoryFileStreamE @ 0x008D09F0
// vtable 0x009031E4 (vptr 0x009031EC), offset_to_top 0, 22 entries
class MemoryFileStream : public ::nw::io::FileStream
{
public:
    MemoryFileStream(const void* pData, u32 size); // 0x004C859C | fefates:bytes [tier B]
    virtual ~MemoryFileStream(); // 0x004C85C8 slot 0x04

    virtual bool IsAvailable() const { return true; } // 0x0073FD04 slot 0x0C
    virtual bool vf_0x10() const { return false; } // 0x0073FD24 slot 0x10
    virtual bool vf_0x14() const { return false; } // 0x0073FD1C slot 0x14
    virtual s32 Read(void* pBuffer, u32 size); // 0x004C8510 slot 0x24 (name after the other streams)
    virtual void Close(); // 0x004C8588 slot 0x38 | nintendogs:bytes
    virtual u32 GetSize() const { return m_Size; } // 0x0073FD14 slot 0x3C
    virtual void Seek(s32 offset, u32 origin); // 0x004C8558 slot 0x40 | nintendogs:bytes
    virtual bool CanSeek() const { return true; } // 0x0073FD0C slot 0x4C (name after the other streams)
    virtual bool vf_0x50() const { return true; } // 0x0073FD2C slot 0x50
    virtual u32 Tell() const { return m_Position; } // 0x0073FCFC slot 0x54 (name after the other streams)

    const void* m_pData; // 0x14
    u32 m_Size;          // 0x18
    u32 m_Position;      // 0x1C
};
ASSERT_SIZE(MemoryFileStream, 0x20);
} // namespace internal
} // namespace snd
} // namespace nw
