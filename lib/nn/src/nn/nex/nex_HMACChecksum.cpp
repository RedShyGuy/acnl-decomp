#include "nn/nex/nex_KeyedChecksumAlgorithm.h"
#include "nn/nex/nex_HMACChecksum.h"

namespace nn {
namespace nex {
// 0x0035CE48 slot 0x00 | mk7dlp:callseq-callee
nn::nex::HMACChecksum::~HMACChecksum()
{
}

// 0x0035CE14 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
void nn::nex::HMACChecksum::vf_0x04()
{
}

// 0x0035CBCC slot 0x08 | fefates:callseq
void nn::nex::HMACChecksum::vf_0x08()
{
}

// 0x0035CB5C slot 0x0C | slot vf_0x0C of nn::nex::KeyedChecksumAlgorithm
void nn::nex::HMACChecksum::ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&)
{
}

// 0x0035CD5C slot 0x18 | slot vf_0x18 of nn::nex::KeyedChecksumAlgorithm
void nn::nex::HMACChecksum::ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int)
{
}

// 0x0035CC50 slot 0x1C | slot vf_0x1C of nn::nex::KeyedChecksumAlgorithm
void nn::nex::HMACChecksum::GetChecksumLength()
{
}

// 0x0035CA68 slot 0x20 | fefates:bytes
void nn::nex::HMACChecksum::KeyHasChanged()
{
}

// 0x0035CC58 | fefates:bytes [tier B]
void nn::nex::HMACChecksum::ChecksumComputeHelper(const unsigned char**, const unsigned int*, int, nn::nex::MD5&)
{
}

// 0x0035CDB8 | mk7dlp:callseq-callee [tier A]
nn::nex::HMACChecksum::HMACChecksum()
{
}

} // namespace nex
} // namespace nn
