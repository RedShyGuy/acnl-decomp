#include "nn/nex/nex_ChecksumAlgorithm.h"
#include "nn/nex/nex_MD5Checksum.h"

namespace nn {
namespace nex {
// 0x00358968 slot 0x00 | virtual slot, introduced by nn::nex::MD5Checksum
void nn::nex::MD5Checksum::vf_0x00()
{
}

// 0x00358950 slot 0x04 | slot vf_0x04 of nn::nex::MD5Checksum
nn::nex::MD5Checksum::~MD5Checksum()
{
}

// 0x00358804 slot 0x08 | fefates:bytes
void nn::nex::MD5Checksum::ComputeChecksum(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x00358750 slot 0x0C | fefates:bytes
void nn::nex::MD5Checksum::ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&)
{
}

// 0x0072C1D4 slot 0x10 | slot vf_0x10 of nn::nex::MD5Checksum
void nn::nex::MD5Checksum::IsReady() const
{
}

// 0x00385EB0 slot 0x14 | fefates:bytes
void nn::nex::MD5Checksum::ComputeChecksumForTransport(const unsigned char*, unsigned int)
{
}

// 0x00358898 slot 0x18 | fefates:bytes
void nn::nex::MD5Checksum::ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int)
{
}

// 0x00358890 slot 0x1C | slot vf_0x1C of nn::nex::MD5Checksum
void nn::nex::MD5Checksum::GetChecksumLength()
{
}

// 0x00358934 | fefates:bytes [tier B]
nn::nex::MD5Checksum::MD5Checksum()
{
}

} // namespace nex
} // namespace nn
