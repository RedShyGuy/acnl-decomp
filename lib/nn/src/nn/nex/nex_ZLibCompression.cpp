#include "nn/nex/nex_CompressionAlgorithm.h"
#include "nn/nex/nex_ZLibCompression.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x0037EB10, 0x0037EC3C (unverified)
nn::nex::ZLibCompression::ZLibCompression()
{
}

// 0x0037EE08 slot 0x00 | virtual slot, introduced by nn::nex::CompressionAlgorithm
void nn::nex::ZLibCompression::vf_0x00()
{
}

// 0x0037ED54 slot 0x04 | fefates:callseq
void nn::nex::ZLibCompression::vf_0x04()
{
}

// 0x0037E804 slot 0x08 | slot vf_0x08 of nn::nex::CompressionAlgorithm
void nn::nex::ZLibCompression::CompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0037E974 slot 0x0C | fefates:bytes-fuzzy
void nn::nex::ZLibCompression::DecompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0037EB10 | fefates:bytes-fuzzy [tier B]
nn::nex::ZLibCompression::ZLibCompression(int, int)
{
}

} // namespace nex
} // namespace nn
