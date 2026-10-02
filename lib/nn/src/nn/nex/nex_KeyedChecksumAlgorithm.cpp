#include "nn/nex/nex_ChecksumAlgorithm.h"
#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::KeyedChecksumAlgorithm::KeyedChecksumAlgorithm()
{
}

// 0x0039DAF4 slot 0x00 | slot vf_0x00 of nn::nex::KeyedChecksumAlgorithm
nn::nex::KeyedChecksumAlgorithm::~KeyedChecksumAlgorithm()
{
}

// 0x0039DAC0 slot 0x04 | virtual slot, introduced by nn::nex::KeyedChecksumAlgorithm
void nn::nex::KeyedChecksumAlgorithm::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::KeyedChecksumAlgorithm::vf_0x08()
{
}

// 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
void nn::nex::KeyedChecksumAlgorithm::ComputeChecksum(const unsigned char**, const unsigned int*, int, nn::nex::SignatureBytes&)
{
}

// 0x0072D088 slot 0x10 | fefates:bytes
void nn::nex::KeyedChecksumAlgorithm::IsReady() const
{
}

// 0x00385EB0 slot 0x14 | fefates:bytes
void nn::nex::KeyedChecksumAlgorithm::ComputeChecksumForTransport(const unsigned char*, unsigned int)
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void nn::nex::KeyedChecksumAlgorithm::ComputeChecksumForTransportArray(const unsigned char**, const unsigned int*, int)
{
}

// 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
void nn::nex::KeyedChecksumAlgorithm::GetChecksumLength()
{
}

// 0x0039DA70 slot 0x20 | slot vf_0x20 of nn::nex::KeyedChecksumAlgorithm
void nn::nex::KeyedChecksumAlgorithm::KeyHasChanged()
{
}

// 0x0039DA74 | fefates:bytes [tier B]
void nn::nex::KeyedChecksumAlgorithm::SetKey(const nn::nex::Key&)
{
}

} // namespace nex
} // namespace nn
