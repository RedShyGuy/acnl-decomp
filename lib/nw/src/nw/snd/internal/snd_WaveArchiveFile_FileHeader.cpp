#include "nw/snd/internal/snd_WaveArchiveFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013FAC4 | nintendogs:bytes [tier A]
const void* nw::snd::internal::WaveArchiveFile::FileHeader::GetFileBlock() const
{
    return GetReferedPointer(this, *GetReferenceBy(TYPE_FILE_BLOCK));
}

// 0x0013FAF8 | nintendogs:bytes [tier A]
const void* nw::snd::internal::WaveArchiveFile::FileHeader::GetInfoBlock() const
{
    return GetReferedPointer(this, *GetReferenceBy(TYPE_INFO_BLOCK));
}

// 0x00141D28 | nintendogs:bytes [tier A]
u32 nw::snd::internal::WaveArchiveFile::FileHeader::GetFileBlockOffset() const
{
    return GetReferenceBy(TYPE_FILE_BLOCK)->offset;
}

// 0x00143A90 | nintendogs:bytes [tier A]
u32 nw::snd::internal::WaveArchiveFile::FileHeader::GetInfoBlockOffset() const
{
    return GetReferenceBy(TYPE_INFO_BLOCK)->offset;
}

} // namespace internal
} // namespace snd
} // namespace nw
