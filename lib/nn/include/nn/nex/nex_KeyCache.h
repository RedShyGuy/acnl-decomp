#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_qMap.h"

namespace std {
// (inline with the insert body in KeyCache::AddKey)
template <>
inline nn::nex::Buffer& map<u32, nn::nex::Buffer, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::Buffer> > >::operator[](const u32& key)
{
    return IndexInline(key);
}
template <>
inline nn::nex::Key& map<u32, nn::nex::Key, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::Key> > >::operator[](const u32& key)
{
    return IndexInline(key);
}
} // namespace std

namespace nn {
namespace nex {
// The keys derived from passwords, per id (the password must match). The member names are ours.
class KeyCache
{
public:
    // false if the cache has no key of the id for the password
    bool RetrieveKey(unsigned int id, const char* pPassword, nn::nex::Key* pKey); // 0x003D5194 | fefates:bytes [tier B]
    bool RetrieveKey(unsigned int id, const nn::nex::Buffer& password, nn::nex::Key* pKey); // 0x003D51FC | fefates:bytes [tier B]
    void AddKey(unsigned id, const char* pPassword, const nn::nex::Key& key); // 0x003D5338 | mk7dlp:callseq [tier A]
    void AddKey(unsigned id, const nn::nex::Buffer& password, const nn::nex::Key& key); // 0x003D5398 | mk7dlp:callseq [tier A]

    u32 m_Unknown0x00;            // 0x00
    qMap<u32, Buffer> m_Passwords; // 0x04
    qMap<u32, Key> m_Keys;         // 0x20
    CriticalSection m_Lock;        // 0x3C
};
ASSERT_OFFSET(KeyCache, m_Keys, 0x20);
ASSERT_OFFSET(KeyCache, m_Lock, 0x3C);
} // namespace nex
} // namespace nn
