#include "nn/nex/nex_KerberosEncryption.h"

namespace nn {
namespace nex {
// 0x0038BFCC | fefates:bytes [tier B]
void nn::nex::KerberosEncryption::InitializeKey(nn::nex::CallContext*, const char*, nn::nex::Key*)
{
}

// 0x0038C014 | mk7dlp:callseq [tier A]
void nn::nex::KerberosEncryption::Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*)
{
}

// 0x0038C084 | mk7dlp:callseq [tier A]
void nn::nex::KerberosEncryption::Decrypt(const nn::nex::Buffer&, nn::nex::Buffer*, const nn::nex::Key&)
{
}

// 0x0038C16C | mk7dlp:callseq [tier A]
void nn::nex::KerberosEncryption::Encrypt(const nn::nex::Buffer&, nn::nex::Buffer*, const nn::nex::Key&)
{
}

// 0x0038C220 | fefates:bytes [tier B]
void nn::nex::KerberosEncryption::CreateKey(nn::nex::CallContext*, unsigned int, const char*, nn::nex::Key*)
{
}

// 0x0038C268 | fefates:bytes [tier B]
void nn::nex::KerberosEncryption::CreateKey(unsigned int, const char*)
{
}

// 0x0038C350 | mk7dlp:callseq [tier A]
nn::nex::KerberosEncryption::KerberosEncryption()
{
}

// 0x0038C374 | mk7dlp:callseq [tier A]
nn::nex::KerberosEncryption::~KerberosEncryption()
{
}

// 0x003CCAE4 | fefates:bytes [tier B]
void nn::nex::KerberosEncryption::SetKey(const nn::nex::Key&)
{
}

} // namespace nex
} // namespace nn
