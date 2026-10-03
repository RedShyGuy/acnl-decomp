#include "nn/fs/fs_Directory.h"

namespace nn {
namespace fs {

// 0x00349908 slot 0x00
// 0x003498E0 slot 0x04 (deleting dtor) | fefates:bytes
nn::fs::Directory::~Directory()
{
    Finalize();
}

} // namespace fs
} // namespace nn
