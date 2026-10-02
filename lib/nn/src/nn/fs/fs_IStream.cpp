#include "nn/fs/fs_IOutputStream.h"
#include "nn/fs/fs_IInputStream.h"
#include "nn/fs/fs_IStream.h"

namespace nn {
namespace fs {
// ctor address unknown
nn::fs::IStream::IStream()
{
}

// 0x003498DC slot 0x00 | virtual slot, introduced by nn::fs::IInputStream
void nn::fs::IStream::vf_0x00()
{
}

// 0x003498D8 slot 0x04 | virtual slot, introduced by nn::fs::IInputStream
void nn::fs::IStream::vf_0x04()
{
}

} // namespace fs
} // namespace nn
