#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto10CipherModeE @ 0x008D0474
//
// Base of the cipher modes (AuthenticatedEncryptor, AuthenticatedDecryptor). The slot names are
// ours; slots 0x0C to 0x14 return constants (CCM: 13, 1, 16) whose meaning is unknown (13 is the
// largest CCM nonce, 16 the largest CCM MAC).
class CipherMode
{
public:
    virtual ~CipherMode() {}          // slots 0x00, 0x04
    // clears the state
    virtual void Finalize() = 0;      // slot 0x08 (name is ours)
    virtual size_t vf_0x0C() const = 0; // slot 0x0C
    virtual size_t vf_0x10() const = 0; // slot 0x10
    virtual size_t vf_0x14() const = 0; // slot 0x14
};
} // namespace crypto
} // namespace nn
