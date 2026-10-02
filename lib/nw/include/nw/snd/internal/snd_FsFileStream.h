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
    FsFileStream(); // ctor candidate(s) 0x004C6E64, 0x004C6FEC (unverified)
    virtual void vf_0x00(); // 0x007375A0 slot 0x00 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x04(); // 0x004C721C slot 0x04 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x08(); // 0x004C7188 slot 0x08 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x0C(); // 0x0073F3D4 slot 0x0C | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x10(); // 0x0073F3F4 slot 0x10 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x14(); // 0x0073F3EC slot 0x14 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x18(); // 0x007375BC slot 0x18 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x1C(); // 0x007375AC slot 0x1C | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x20(); // 0x007375B4 slot 0x20 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void Read(void*, unsigned int); // 0x004C6C74 slot 0x24 | fefates:bytes
    virtual void vf_0x28(); // 0x0048B8D4 slot 0x28 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x2C(); // 0x0048B8CC slot 0x2C | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x30(); // 0x0048B8C4 slot 0x30 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void vf_0x34(); // 0x007375C4 slot 0x34 | virtual slot, introduced by nw::snd::internal::CachedFileStream
    virtual void Close(); // 0x004C6E14 slot 0x38 | fefates:bytes
    virtual void GetSize() const; // 0x0073F3E4 slot 0x3C | slot vf_0x3C of nw::snd::internal::FsFileStream
    virtual void Seek(int, unsigned int); // 0x004C6DB0 slot 0x40 | fefates:bytes
    virtual void vf_0x44(); // 0x0048B8C0 slot 0x44 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x48(); // 0x0048B814 slot 0x48 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x4C(); // 0x0073F3DC slot 0x4C | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x50(); // 0x0073F3FC slot 0x50 | virtual slot, introduced by nw::snd::internal::FsFileStream
    virtual void vf_0x54(); // 0x0073F3C4 slot 0x54 | virtual slot, introduced by nw::snd::internal::FsFileStream
    void SetPriority(nw::snd::internal::FileReader::Priority); // 0x004C6BF4 | fefates:bytes [tier B]
    FsFileStream(const char*, unsigned int, unsigned int, bool, void (*)(nn::Result,void*), void*); // 0x004C6E64 | fefates:bytes [tier B]
    FsFileStream(nw::snd::internal::FileReader*, unsigned int, unsigned int, void (*)(nn::Result,void*), void*); // 0x004C6FEC | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
