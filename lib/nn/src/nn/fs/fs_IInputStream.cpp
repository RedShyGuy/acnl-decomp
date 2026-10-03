#include "nn/fs/fs_IInputStream.h"

namespace nn {
namespace fs {

// 0x00346018 slot 0x00
// 0x00346014 slot 0x04 (deleting dtor)
nn::fs::IInputStream::~IInputStream()
{
    // an interface: nothing to destroy
}

} // namespace fs
} // namespace nn
