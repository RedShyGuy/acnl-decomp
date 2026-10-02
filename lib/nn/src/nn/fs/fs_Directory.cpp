#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "nn/fs/detail/fs_DirectoryBase.h"
#include "nn/fs/fs_Directory.h"

namespace nn {
namespace fs {
// ctor candidate(s) 0x005AC08C, 0x005B0AC0, 0x005B0B54 (unverified)
nn::fs::Directory::Directory()
{
}

// 0x00349908 slot 0x00 | virtual slot, introduced by nn::fs::Directory
void nn::fs::Directory::vf_0x00()
{
}

// 0x003498E0 slot 0x04 | fefates:bytes
nn::fs::Directory::~Directory()
{
}

} // namespace fs
} // namespace nn
