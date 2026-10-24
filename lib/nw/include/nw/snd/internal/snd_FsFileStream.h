#pragma once

#include "decomp.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/snd_FileReader.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal12FsFileStreamE @ 0x008D099C
// vtable 0x00903090 (vptr 0x00903098), offset_to_top 0, 22 entries
class FsFileStream : public ::nw::io::FileStream
{
public:
    virtual ~FsFileStream(); // 0x004C721C slot 0x04
    virtual bool IsAvailable() const; // 0x0073F3D4 slot 0x0C
    virtual bool vf_0x10() const; // 0x0073F3F4 slot 0x10
    virtual bool vf_0x14() const; // 0x0073F3EC slot 0x14
    virtual s32 Read(void* pBuffer, u32 size); // 0x004C6C74 slot 0x24
    virtual void Close(); // 0x004C6E14 slot 0x38
    virtual u32 GetSize() const; // 0x0073F3E4 slot 0x3C
    virtual void Seek(s32 offset, u32 origin); // 0x004C6DB0 slot 0x40
    virtual bool CanSeek() const; // 0x0073F3DC slot 0x4C
    virtual bool vf_0x50() const; // 0x0073F3FC slot 0x50
    virtual u32 Tell() const; // 0x0073F3C4 slot 0x54
    void SetPriority(nw::snd::internal::FileReader::Priority); // 0x004C6BF4 | fefates:bytes [tier B]
    FsFileStream(const char*, unsigned int, unsigned int, bool, void (*)(nn::Result,void*), void*); // 0x004C6E64 | fefates:bytes [tier B]
    FsFileStream(nw::snd::internal::FileReader*, unsigned int, unsigned int, void (*)(nn::Result,void*), void*); // 0x004C6FEC | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
