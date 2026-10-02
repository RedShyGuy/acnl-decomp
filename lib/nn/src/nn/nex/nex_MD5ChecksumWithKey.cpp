#include "nn/nex/nex_KeyedChecksumAlgorithm.h"
#include "nn/nex/nex_MD5ChecksumWithKey.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D32A8 (unverified)
nn::nex::MD5ChecksumWithKey::MD5ChecksumWithKey()
{
}

// 0x0038C5CC slot 0x00 | slot vf_0x00 of nn::nex::KeyedChecksumAlgorithm
nn::nex::MD5ChecksumWithKey::~MD5ChecksumWithKey()
{
}

// 0x0038C598 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
void nn::nex::MD5ChecksumWithKey::vf_0x04()
{
}

// 0x0038C408 slot 0x08 | fefates:callseq
void nn::nex::MD5ChecksumWithKey::vf_0x08()
{
}

// 0x0038C398 slot 0x0C | slot vf_0x0C of nn::nex::KeyedChecksumAlgorithm
void nn::nex::MD5ChecksumWithKey::ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&)
{
}

// 0x0072C1F8 slot 0x10 | slot vf_0x10 of nn::nex::KeyedChecksumAlgorithm
void nn::nex::MD5ChecksumWithKey::IsReady() const
{
}

// 0x0038C53C slot 0x18 | slot vf_0x18 of nn::nex::KeyedChecksumAlgorithm
void nn::nex::MD5ChecksumWithKey::ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int)
{
}

// 0x0038C48C slot 0x1C | slot vf_0x1C of nn::nex::KeyedChecksumAlgorithm
void nn::nex::MD5ChecksumWithKey::GetChecksumLength()
{
}

// 0x0038C494 | fefates:bytes [tier B]
void nn::nex::MD5ChecksumWithKey::ChecksumComputeHelper(const unsigned char**, const unsigned int*, int, nn::nex::MD5&)
{
}

} // namespace nex
} // namespace nn
