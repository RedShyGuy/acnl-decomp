#include "nw/snd/internal/snd_BankFile_FileHeader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D658 | nintendogs:bytes [tier A]
const void* nw::snd::internal::BankFile::FileHeader::GetInfoBlock() const
{
    return GetBlockByType(this, dataBlocks, blocks, TYPE_INFO_BLOCK);
}

} // namespace internal
} // namespace snd
} // namespace nw
