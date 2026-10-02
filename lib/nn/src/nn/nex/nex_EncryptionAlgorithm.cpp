#include "nn/nex/nex_PluginObject.h"
#include "nn/nex/nex_EncryptionAlgorithm.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::EncryptionAlgorithm::EncryptionAlgorithm()
{
}

// 0x00393320 slot 0x00 | slot vf_0x00 of nn::nex::EncryptionAlgorithm
nn::nex::EncryptionAlgorithm::~EncryptionAlgorithm()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EncryptionAlgorithm::Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x00393294 slot 0x0C | fefates:bytes
void nn::nex::EncryptionAlgorithm::Encrypt(nn::nex::Buffer*)
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nn::nex::EncryptionAlgorithm::Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x00393230 slot 0x14 | fefates:bytes
void nn::nex::EncryptionAlgorithm::Decrypt(nn::nex::Buffer*)
{
}

// 0x0039318C slot 0x18 | virtual slot, introduced by nn::nex::EncryptionAlgorithm
void nn::nex::EncryptionAlgorithm::vf_0x18()
{
}

// 0x00393188 slot 0x1C | slot vf_0x1C of nn::nex::EncryptionAlgorithm
void nn::nex::EncryptionAlgorithm::KeyHasChanged()
{
}

// 0x003931E4 | fefates:bytes [tier B]
void nn::nex::EncryptionAlgorithm::SetKey(const nn::nex::Key&)
{
}

} // namespace nex
} // namespace nn
