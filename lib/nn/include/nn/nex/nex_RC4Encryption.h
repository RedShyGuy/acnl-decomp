#pragma once

#include "decomp.h"
#include "nn/nex/nex_EncryptionAlgorithm.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13RC4EncryptionE @ 0x008CE26C
// vtable 0x008FC900 (vptr 0x008FC908), offset_to_top 0, 8 entries
class RC4Encryption : public ::nn::nex::EncryptionAlgorithm
{
public:
    virtual ~RC4Encryption(); // 0x0036AE8C slot 0x00 | mk7dlp:callseq-callee
    // 0x0036AE28 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0036AD28 slot 0x08 | mk7dlp:bytes
    virtual void Encrypt(nn::nex::Buffer*); // 0x0036AD08 slot 0x0C | slot vf_0x0C of nn::nex::EncryptionAlgorithm
    virtual void Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0036ACD8 slot 0x10 | mk7dlp:bytes
    virtual void Decrypt(nn::nex::Buffer*); // 0x0036ACB8 slot 0x14 | slot vf_0x14 of nn::nex::EncryptionAlgorithm
    virtual void KeyHasChanged(); // 0x0036AB0C slot 0x1C | fefates:bytes-fuzzy
    RC4Encryption(); // 0x0036AD58 | mk7dlp:callseq-callee [tier A]
};
} // namespace nex
} // namespace nn
