#include "nn/nex/nex_KeyCache.h"
#include <cstring>

namespace nn {
namespace nex {
// 0x003D5194 | fefates:bytes [tier B]
bool nn::nex::KeyCache::RetrieveKey(unsigned int id, const char* pPassword, nn::nex::Key* pKey)
{
    Buffer password;
    password.AppendData(pPassword, strlen(pPassword), static_cast<unsigned>(-1));
    return RetrieveKey(id, password, pKey);
}

// 0x003D51FC | fefates:bytes [tier B]
bool nn::nex::KeyCache::RetrieveKey(unsigned int id, const nn::nex::Buffer& password, nn::nex::Key* pKey)
{
    ScopedCS lock(m_Lock);
    qMap<u32, Buffer>::iterator it = m_Passwords.find(id);
    if (it == m_Passwords.end()) {
        return false;
    }
    const Buffer& stored = it->second;
    if (stored.m_ContentSize != password.m_ContentSize
        || memcmp(password.m_pData + password.m_HeaderSize, stored.m_pData + stored.m_HeaderSize, stored.m_ContentSize) != 0) {
        return false;
    }
    *pKey = m_Keys.find(id)->second;
    return true;
}

// 0x003D5338 | mk7dlp:callseq [tier A]
void nn::nex::KeyCache::AddKey(unsigned id, const char* pPassword, const nn::nex::Key& key)
{
    Buffer password;
    password.AppendData(pPassword, strlen(pPassword), static_cast<unsigned>(-1));
    AddKey(id, password, key);
}

// 0x003D5398 | mk7dlp:callseq [tier A]
void nn::nex::KeyCache::AddKey(unsigned id, const nn::nex::Buffer& password, const nn::nex::Key& key)
{
    ScopedCS lock(m_Lock);
    m_Passwords[id] = password;
    m_Keys[id] = key;
}

} // namespace nex
} // namespace nn
