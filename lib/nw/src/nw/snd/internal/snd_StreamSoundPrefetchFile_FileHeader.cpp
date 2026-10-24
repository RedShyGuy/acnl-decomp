#include "nw/snd/internal/snd_StreamSoundPrefetchFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0074127C | fefates:bytes [tier B]
const void* nw::snd::internal::StreamSoundPrefetchFile::FileHeader::GetInfoBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_INFO_BLOCK);
}

// 0x007412E8 (name after GetInfoBlock)
const void* nw::snd::internal::StreamSoundPrefetchFile::FileHeader::GetPrefetchDataBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_PREFETCH_DATA_BLOCK);
}

} // namespace internal
} // namespace snd
} // namespace nw
