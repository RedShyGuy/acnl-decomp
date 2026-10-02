#include "nn/nex/nex_PluginObject.h"
#include "nn/nex/nex_CompressionAlgorithm.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::CompressionAlgorithm::CompressionAlgorithm()
{
}

// 0x00396270 slot 0x00 | virtual slot, introduced by nn::nex::CompressionAlgorithm
void nn::nex::CompressionAlgorithm::vf_0x00()
{
}

// 0x00396218 slot 0x04 | fefates:callseq
void nn::nex::CompressionAlgorithm::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::CompressionAlgorithm::CompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::CompressionAlgorithm::DecompressImpl(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

} // namespace nex
} // namespace nn
