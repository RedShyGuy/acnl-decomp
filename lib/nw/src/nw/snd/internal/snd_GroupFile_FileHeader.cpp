#include "nw/snd/internal/snd_GroupFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013FFF8 | nintendogs:callgraph [tier A]
const void* nw::snd::internal::GroupFile::FileHeader::GetFileBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_FILE_BLOCK);
}

// 0x00140060 | nintendogs:bytes [tier A]
const void* nw::snd::internal::GroupFile::FileHeader::GetInfoBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_INFO_BLOCK);
}

// 0x001400CC | nintendogs:callgraph [tier A]
const void* nw::snd::internal::GroupFile::FileHeader::GetInfoExBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_INFO_EX_BLOCK);
}

} // namespace internal
} // namespace snd
} // namespace nw
