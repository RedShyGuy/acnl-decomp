#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class KerberosEncryption
{
public:
    void InitializeKey(nn::nex::CallContext*, const char*, nn::nex::Key*); // 0x0038BFCC | fefates:bytes [tier B]
    void Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*); // 0x0038C014 | mk7dlp:callseq [tier A]
    void Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*, const nn::nex::Key&); // 0x0038C084 | mk7dlp:callseq [tier A]
    void Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*, const nn::nex::Key&); // 0x0038C16C | mk7dlp:callseq [tier A]
    void CreateKey(nn::nex::CallContext*, unsigned int, const char*, nn::nex::Key*); // 0x0038C220 | fefates:bytes [tier B]
    void CreateKey(unsigned int, const char*); // 0x0038C268 | fefates:bytes [tier B]
    KerberosEncryption(); // 0x0038C350 | mk7dlp:callseq [tier A]
    ~KerberosEncryption(); // 0x0038C374 | mk7dlp:callseq [tier A]
    void SetKey(const nn::nex::Key&); // 0x003CCAE4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
