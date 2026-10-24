#include "nw/snd/internal/snd_SequenceSoundFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00740404 | nintendogs:bytes [tier A]
const void* nw::snd::internal::SequenceSoundFile::FileHeader::GetDataBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_DATA_BLOCK);
}

// 0x00740470 | nintendogs:callgraph [tier C]
const void* nw::snd::internal::SequenceSoundFile::FileHeader::GetLabelBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_LABEL_BLOCK);
}

} // namespace internal
} // namespace snd
} // namespace nw
