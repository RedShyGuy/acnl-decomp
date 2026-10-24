#include "nw/snd/internal/snd_WaveFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x007430CC | nintendogs:bytes [tier C]
const void* nw::snd::internal::WaveFile::FileHeader::GetDataBlock() const
{
    return GetBlock(TYPE_DATA_BLOCK);
}

// 0x00743134 | nintendogs:bytes [tier A]
const void* nw::snd::internal::WaveFile::FileHeader::GetInfoBlock() const
{
    return GetBlock(TYPE_INFO_BLOCK);
}

} // namespace internal
} // namespace snd
} // namespace nw
