#include "nn/nex/nex_EncryptionAlgorithm.h"
#include "nn/nex/nex_Buffer.h"
#include <cwchar>

namespace nn {
namespace nex {
namespace {
// (Decrypt without a buffer)
const int ERROR_NO_BUFFER = 3;
} // namespace

// 0x00393188
void nn::nex::EncryptionAlgorithm::KeyHasChanged()
{
    // nothing to do: the algorithms with a key schedule override it
}

// 0x0039318C (name is ours)
bool nn::nex::EncryptionAlgorithm::GetErrorString(int error, wchar_t* pString, u32 size)
{
    if (wcslen(L"Encryption Error") >= size) {
        return false;
    }
    wcscpy(pString, L"Encryption Error");
    return true;
}

// 0x003931E4 | fefates:bytes [tier B]
bool nn::nex::EncryptionAlgorithm::SetKey(const nn::nex::Key& key)
{
    return SetKeyInline(key);
}

// 0x00393230 | fefates:bytes [tier B]
int nn::nex::EncryptionAlgorithm::Decrypt(nn::nex::Buffer* pBuffer)
{
    if (pBuffer == nullptr) {
        return ERROR_NO_BUFFER;
    }
    Buffer output;
    int error = Decrypt(*pBuffer, &output);
    if (error == 0) {
        *pBuffer = output;
    }
    return error;
}

// 0x00393294 | fefates:bytes [tier B]
int nn::nex::EncryptionAlgorithm::Encrypt(nn::nex::Buffer* pBuffer)
{
    Buffer output;
    int error = Encrypt(*pBuffer, &output);
    if (error == 0) {
        *pBuffer = output;
    }
    return error;
}

// 0x00393320
// 0x003932EC (deleting dtor)
nn::nex::EncryptionAlgorithm::~EncryptionAlgorithm()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
