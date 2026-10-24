#include "nw/snd/internal/snd_SoundArchiveFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073FD34 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::FileHeader::GetInfoBlockSize() const
{
    return GetReferenceBy(TYPE_INFO_BLOCK)->size;
}

// 0x0073FD74 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::FileHeader::GetFileBlockOffset() const
{
    return GetReferenceBy(TYPE_FILE_BLOCK)->offset;
}

// 0x0073FDB4 | nintendogs:bytes [tier C]
u32 nw::snd::internal::SoundArchiveFile::FileHeader::GetInfoBlockOffset() const
{
    return GetReferenceBy(TYPE_INFO_BLOCK)->offset;
}

// 0x0073FDF4 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::FileHeader::GetStringBlockSize() const
{
    return GetReferenceBy(TYPE_STRING_BLOCK)->size;
}

// 0x0073FE2C | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::FileHeader::GetStringBlockOffset() const
{
    return GetReferenceBy(TYPE_STRING_BLOCK)->offset;
}

} // namespace internal
} // namespace snd
} // namespace nw
