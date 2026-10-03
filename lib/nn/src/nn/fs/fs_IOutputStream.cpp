#include "nn/fs/fs_IOutputStream.h"

namespace nn {
namespace fs {

// 0x003460C8 slot 0x00
// 0x003460C4 slot 0x04 (deleting dtor)
nn::fs::IOutputStream::~IOutputStream()
{
    // an interface: nothing to destroy
}

} // namespace fs
} // namespace nn
