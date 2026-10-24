#pragma once

#include "decomp.h"
#include "nw/io/io_IOStream.h"

namespace nw {
namespace io {
// A stream with a size and a position (the class name and GetSize/Seek/CanSeek/Tell are from the
// reference symbols).
// RTTI N2nw2io10FileStreamE @ 0x008D0514
class FileStream : public ::nw::io::IOStream
{
public:
    class FilePosition;

    // the origins of Seek (names are ours)
    enum SeekOrigin
    {
        SEEK_ORIGIN_BEGIN,
        SEEK_ORIGIN_CURRENT,
        SEEK_ORIGIN_END
    };

    virtual const TypeInfo* GetRuntimeTypeInfo() const; // 0x007375A0 slot 0x00 (name after nw::gfx)
    virtual u32 GetSize() const = 0; // slot 0x3C
    virtual void Seek(s32 offset, u32 origin) = 0; // slot 0x40
    virtual void vf_0x44(); // 0x0048B8C0 slot 0x44
    // two arguments of unknown types
    virtual bool vf_0x48(u32 arg0, u32 arg1); // 0x0048B814 slot 0x48
    virtual bool CanSeek() const = 0; // slot 0x4C
    virtual bool vf_0x50() const = 0; // slot 0x50
    virtual u32 Tell() const = 0; // slot 0x54

    static const TypeInfo s_TypeInfo;
};
ASSERT_SIZE(FileStream, 0x14);
} // namespace io
} // namespace nw
