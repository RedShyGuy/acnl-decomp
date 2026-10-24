#include "nw/snd/internal/snd_FileReader.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/snd_FsFileStream.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C721C
// 0x004C7188 (deleting dtor)
nw::snd::internal::FsFileStream::~FsFileStream()
{
}

// 0x0073F3D4 slot 0x0C | virtual slot, introduced by nw::snd::internal::FsFileStream
bool nw::snd::internal::FsFileStream::IsAvailable() const
{
}

// 0x0073F3F4 slot 0x10 | virtual slot, introduced by nw::snd::internal::FsFileStream
bool nw::snd::internal::FsFileStream::vf_0x10() const
{
}

// 0x0073F3EC slot 0x14 | virtual slot, introduced by nw::snd::internal::FsFileStream
bool nw::snd::internal::FsFileStream::vf_0x14() const
{
}

// 0x004C6C74 slot 0x24 | fefates:bytes
s32 nw::snd::internal::FsFileStream::Read(void*, u32)
{
}

// 0x004C6E14 slot 0x38 | fefates:bytes
void nw::snd::internal::FsFileStream::Close()
{
}

// 0x0073F3E4 slot 0x3C | slot vf_0x3C of nw::snd::internal::FsFileStream
u32 nw::snd::internal::FsFileStream::GetSize() const
{
}

// 0x004C6DB0 slot 0x40 | fefates:bytes
void nw::snd::internal::FsFileStream::Seek(s32, u32)
{
}

// 0x0073F3DC slot 0x4C | virtual slot, introduced by nw::snd::internal::FsFileStream
bool nw::snd::internal::FsFileStream::CanSeek() const
{
}

// 0x0073F3FC slot 0x50 | virtual slot, introduced by nw::snd::internal::FsFileStream
bool nw::snd::internal::FsFileStream::vf_0x50() const
{
}

// 0x0073F3C4 slot 0x54 | virtual slot, introduced by nw::snd::internal::FsFileStream
u32 nw::snd::internal::FsFileStream::Tell() const
{
}

// 0x004C6BF4 | fefates:bytes [tier B]
void nw::snd::internal::FsFileStream::SetPriority(nw::snd::internal::FileReader::Priority)
{
}

// 0x004C6E64 | fefates:bytes [tier B]
nw::snd::internal::FsFileStream::FsFileStream(const char*, unsigned int, unsigned int, bool, void (*)(nn::Result,void*), void*)
{
}

// 0x004C6FEC | fefates:bytes [tier B]
nw::snd::internal::FsFileStream::FsFileStream(nw::snd::internal::FileReader*, unsigned int, unsigned int, void (*)(nn::Result,void*), void*)
{
}

} // namespace internal
} // namespace snd
} // namespace nw
