#include "nn/fs/CTR/MPCore/detail/fs_ArchiveTableEntry.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x00347F54 | fefates:bytes [tier B]
nn::fs::CTR::MPCore::detail::ArchiveTableEntry::ArchiveTableEntry() : key(0), archive(0)
{
    // the flags are not initialized
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
