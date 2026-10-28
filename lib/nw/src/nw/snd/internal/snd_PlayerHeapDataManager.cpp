#include "nw/snd/internal/snd_PlayerHeapDataManager.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C8ADC | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeapDataManager::Initialize(const SoundArchive* pSoundArchive)
{
    if (m_IsInitialized) {
        return;
    }
    m_IsInitialized = true;
    m_IsFinalized = false;
    for (int i = 0; i < FILE_ADDRESS_COUNT; ++i) {
        m_FileAddressTable[i].fileId = 0xFFFFFFFF;
        m_FileAddressTable[i].address = nullptr;
    }
    SetSoundArchive(pSoundArchive);
}

// 0x004C9378 slot 0x08 | nintendogs:bytes
void nw::snd::internal::PlayerHeapDataManager::InvalidateData(const void*, const void*)
{
    for (int i = 0; i < FILE_ADDRESS_COUNT; ++i) {
        m_FileAddressTable[i].fileId = 0xFFFFFFFF;
        m_FileAddressTable[i].address = nullptr;
    }
}

// 0x004C93AC slot 0x0C
// 0x0077437C (thunk)
const void* nw::snd::internal::PlayerHeapDataManager::SetFileAddressToTable(u32 fileId, const void* pAddress)
{
    for (int i = 0; i < FILE_ADDRESS_COUNT; ++i) {
        if (m_FileAddressTable[i].fileId == fileId) {
            const void* pOld = m_FileAddressTable[i].address;
            m_FileAddressTable[i].address = pAddress;
            return pOld;
        }
    }
    for (int i = 0; i < FILE_ADDRESS_COUNT; ++i) {
        if (m_FileAddressTable[i].fileId == 0xFFFFFFFF) {
            m_FileAddressTable[i].fileId = fileId;
            m_FileAddressTable[i].address = pAddress;
            return nullptr;
        }
    }
    return nullptr;
}

// 0x004C9418 | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeapDataManager::Finalize()
{
    if (m_IsFinalized) {
        return;
    }
    m_IsInitialized = false;
    m_IsFinalized = true;
    SetSoundArchive(nullptr);
}

// 0x004C9444 | nintendogs:bytes [tier A]
nw::snd::internal::PlayerHeapDataManager::PlayerHeapDataManager() : m_IsInitialized(false), m_IsFinalized(true)
{
}

// 0x004C94E0 slot 0x00 | fefates:bytes
// 0x004C9488 (deleting dtor)
// 0x00774450 (thunk)
// 0x007743F8 (deleting thunk)
nw::snd::internal::PlayerHeapDataManager::~PlayerHeapDataManager()
{
    Finalize();
}

// 0x00740980 slot 0x14
// 0x00774624 (thunk)
const void* nw::snd::internal::PlayerHeapDataManager::GetFileAddressImpl(u32 fileId) const
{
    return GetFileAddressFromTable(fileId);
}

// 0x0074098C slot 0x10
// 0x00774630 (thunk)
const void* nw::snd::internal::PlayerHeapDataManager::GetFileAddressFromTable(u32 fileId) const
{
    for (int i = 0; i < FILE_ADDRESS_COUNT; ++i) {
        if (m_FileAddressTable[i].fileId == fileId) {
            return m_FileAddressTable[i].address;
        }
    }
    return nullptr;
}

} // namespace internal
} // namespace snd
} // namespace nw
