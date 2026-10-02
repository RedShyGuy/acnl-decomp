#pragma once

#include "decomp.h"
#include "nn/nex/nex_PluginObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19EncryptionAlgorithmE @ 0x008CE7D8
// vtable 0x008FD6A0 (vptr 0x008FD6A8), offset_to_top 0, 8 entries
class EncryptionAlgorithm : public ::nn::nex::PluginObject
{
public:
    EncryptionAlgorithm(); // ctor address unknown
    virtual ~EncryptionAlgorithm(); // 0x00393320 slot 0x00 | slot vf_0x00 of nn::nex::EncryptionAlgorithm
    // 0x003932EC slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void Encrypt(nn::nex::Buffer*); // 0x00393294 slot 0x0C | fefates:bytes
    virtual void Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void Decrypt(nn::nex::Buffer*); // 0x00393230 slot 0x14 | fefates:bytes
    virtual void vf_0x18(); // 0x0039318C slot 0x18 | virtual slot, introduced by nn::nex::EncryptionAlgorithm
    virtual void KeyHasChanged(); // 0x00393188 slot 0x1C | slot vf_0x1C of nn::nex::EncryptionAlgorithm
    void SetKey(const nn::nex::Key&); // 0x003931E4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
