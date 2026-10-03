#include "nn/fs/fs_IStream.h"

namespace nn {
namespace fs {

// 0x003498DC slot 0x00
// 0x003498D8 slot 0x04 (deleting dtor)
nn::fs::IStream::~IStream()
{
    // an interface: nothing to destroy
}

} // namespace fs
} // namespace nn
