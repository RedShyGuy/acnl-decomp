#include "nw/snd/snd_SoundDataManager.h"

#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_WaveArchiveFileReader.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace {
// (inline) registers or unregisters the manager in the sound thread
void PushDisposeCallbackCommand(internal::DriverCommand::Id id, internal::driver::DisposeCallback* pCallback)
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandDisposeCallback* pCommand = manager.AllocCommand<internal::DriverCommandDisposeCallback>();
    pCommand->id = id;
    pCommand->pCallback = pCallback;
    manager.PushCommand(pCommand);
}
} // namespace

// 0x00137C74 | nintendogs:bytes [tier A]
nw::snd::SoundDataManager::SoundDataManager() : m_pFileTable(nullptr), m_pFileManager(nullptr)
{
}

// 0x004C05AC | nintendogs:bytes-fuzzy [tier A]
bool nw::snd::SoundDataManager::Initialize(const SoundArchive* pSoundArchive, void* pBuffer, u32 bufferSize)
{
    u8* p = static_cast<u8*>(pBuffer);
    u8* pEnd = p + bufferSize;
    if (ut::RoundUp(p + sizeof(u32) + pSoundArchive->detail_GetFileCount() * sizeof(const void*), 4) - pEnd > 0) {
        return false;
    }
    m_pFileTable = static_cast<internal::BinaryTable<const void*>*>(pBuffer);
    m_pFileTable->count = pSoundArchive->detail_GetFileCount();
    for (u32 i = 0; i < m_pFileTable->count; ++i) {
        m_pFileTable->items[i] = nullptr;
    }
    SetSoundArchive(pSoundArchive);
    PushDisposeCallbackCommand(internal::DriverCommand::ID_REGISTER_DISPOSE_CALLBACK, this);
    return true;
}

// 0x004C06B8 slot 0x08 | nintendogs:callseq
void nw::snd::SoundDataManager::InvalidateData(const void* pStart, const void* pEnd)
{
    if (m_pFileTable != nullptr) {
        for (u32 i = 0; i < m_pFileTable->count; ++i) {
            const void* pAddress = m_pFileTable->items[i];
            if (pStart <= pAddress && pAddress <= pEnd) {
                m_pFileTable->items[i] = nullptr;
            }
        }
    }
    // the waves of the individually loaded wave archives
    const SoundArchive* pSoundArchive = m_pSoundArchive;
    if (pSoundArchive == nullptr) {
        return;
    }
    const u32 waveArchiveCount = pSoundArchive->GetWaveArchiveCount();
    for (u32 i = 0; i < waveArchiveCount; ++i) {
        SoundArchive::WaveArchiveInfo info;
        if (!pSoundArchive->ReadWaveArchiveInfo(SoundArchive::MakeItemId(SoundArchive::ITEM_TYPE_WAVE_ARCHIVE, i), &info) || !info.isLoadIndividual) {
            continue;
        }
        const void* pTableFile = GetFileAddressFromTable(info.fileId);
        if (pTableFile == nullptr) {
            continue;
        }
        internal::WaveArchiveFileReader reader(pTableFile, true);
        for (u32 j = 0; j < info.waveCount; ++j) {
            const void* pWaveFile = reader.GetWaveFile(j);
            if (pWaveFile != nullptr && pStart <= pWaveFile && pWaveFile <= pEnd) {
                reader.SetWaveFile(j, nullptr);
            }
        }
    }
}

// 0x004C0800 slot 0x0C | mk7dlp:bytes
// 0x00774334 (thunk)
const void* nw::snd::SoundDataManager::SetFileAddressToTable(u32 fileId, const void* pAddress)
{
    if (m_pFileTable == nullptr) {
        return nullptr;
    }
    const void* pOld = m_pFileTable->items[fileId];
    m_pFileTable->items[fileId] = pAddress;
    return pOld;
}

// 0x004C0824 | nintendogs:callseq [tier A]
void nw::snd::SoundDataManager::Finalize()
{
    PushDisposeCallbackCommand(internal::DriverCommand::ID_UNREGISTER_DISPOSE_CALLBACK, this);
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    manager.WaitCommandReply(manager.FlushCommand(true));
    m_pFileManager = nullptr;
    m_pFileTable = nullptr;
}

// 0x004C089C slot 0x00 | fefates:bytes
// 0x004C0884 (deleting dtor)
// 0x0077436C (thunk 12)
// 0x00774358 (deleting thunk 12)
nw::snd::SoundDataManager::~SoundDataManager()
{
    // (only the bases are destroyed)
}

// 0x0073EDF4 slot 0x14 | fefates:bytes
// 0x00774594 (thunk)
const void* nw::snd::SoundDataManager::GetFileAddressImpl(u32 fileId) const
{
    if (m_pFileManager != nullptr) {
        const void* pAddress = m_pFileManager->GetFileAddress(fileId);
        if (pAddress != nullptr) {
            return pAddress;
        }
    }
    const void* pAddress = GetFileAddressFromSoundArchive(fileId);
    if (pAddress != nullptr) {
        return pAddress;
    }
    return GetFileAddressFromTable(fileId);
}

// 0x0073EE58 | nintendogs:bytes [tier A]
u32 nw::snd::SoundDataManager::GetRequiredMemSize(const SoundArchive* pSoundArchive) const
{
    return ut::RoundUp(sizeof(u32) + pSoundArchive->detail_GetFileCount() * sizeof(const void*), 4);
}

// 0x0073EE78 | nintendogs:callseq
const void* nw::snd::SoundDataManager::detail_GetFileAddress(u32 fileId) const
{
    return GetFileAddressImpl(fileId);
}

// 0x0073EE84 slot 0x10 | nintendogs:bytes
// 0x007745F8 (thunk)
const void* nw::snd::SoundDataManager::GetFileAddressFromTable(u32 fileId) const
{
    if (m_pFileTable == nullptr) {
        return nullptr;
    }
    if (fileId >= m_pFileTable->count) {
        return nullptr;
    }
    return m_pFileTable->items[fileId];
}

} // namespace snd
} // namespace nw
