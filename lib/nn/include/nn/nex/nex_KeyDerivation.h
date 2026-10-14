#pragma once

#include "decomp.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class CallContext;
class KeyCache;

// The derivation of keys from passwords (directly or in a job). The slot names without a reference
// name are ours (after the reference names of the other slots).
// RTTI N2nn3nex13KeyDerivationE @ 0x008CE224
class KeyDerivation : public ::nn::nex::RootObject
{
public:
    // (inline)
    KeyDerivation() {}
    virtual ~KeyDerivation() {}
    // the key of the id
    virtual nn::nex::Key CreateKey(unsigned id, const char* pPassword) = 0; // slot 0x08
    virtual nn::nex::Key InitializeKey(const char* pPassword) = 0; // slot 0x0C
    // the key of the id from another key
    virtual nn::nex::Key CreateKey(unsigned id, const nn::nex::Key& key) = 0; // slot 0x10
    virtual bool CreateKey(nn::nex::CallContext* pContext, unsigned int id, const char* pPassword, nn::nex::Key* pKey, nn::nex::KeyCache* pKeyCache) = 0; // slot 0x14
    virtual bool InitializeKey(nn::nex::CallContext* pContext, const char* pPassword, nn::nex::Key* pKey) = 0; // slot 0x18
    virtual bool CreateKey(nn::nex::CallContext* pContext, unsigned int id, const nn::nex::Key& key, nn::nex::Key* pKey) = 0; // slot 0x1C
};
} // namespace nex
} // namespace nn
