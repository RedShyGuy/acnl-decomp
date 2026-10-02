#include "nn/nex/nex_EncryptionAlgorithm.h"
#include "nn/nex/nex_RC4Encryption.h"

namespace nn {
namespace nex {
// 0x0036AE8C slot 0x00 | mk7dlp:callseq-callee
nn::nex::RC4Encryption::~RC4Encryption()
{
}

// 0x0036AD28 slot 0x08 | mk7dlp:bytes
void nn::nex::RC4Encryption::Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0036AD08 slot 0x0C | slot vf_0x0C of nn::nex::EncryptionAlgorithm
void nn::nex::RC4Encryption::Encrypt(nn::nex::Buffer*)
{
}

// 0x0036ACD8 slot 0x10 | mk7dlp:bytes
void nn::nex::RC4Encryption::Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0036ACB8 slot 0x14 | slot vf_0x14 of nn::nex::EncryptionAlgorithm
void nn::nex::RC4Encryption::Decrypt(nn::nex::Buffer*)
{
}

// 0x0036AB0C slot 0x1C | fefates:bytes-fuzzy
void nn::nex::RC4Encryption::KeyHasChanged()
{
}

// 0x0036AD58 | mk7dlp:callseq-callee [tier A]
nn::nex::RC4Encryption::RC4Encryption()
{
}

} // namespace nex
} // namespace nn
