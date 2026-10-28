#include "nw/snd/internal/snd_SoundArchiveLoader.h"

#include <cstring>

#include "nn/os/os_Api.h"
#include "nn/snd/CTR/CTR_Api.h"
#include "nw/io/io_FileStream.h"
#include "nw/snd/internal/snd_BankFileReader.h"
#include "nw/snd/internal/snd_GroupFileReader.h"
#include "nw/snd/internal/snd_WaveArchiveFile.h"
#include "nw/snd/internal/snd_WaveArchiveFileReader.h"
#include "nw/snd/internal/snd_WaveSoundFileReader.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the alignment of the reads of the file system
const u32 READ_ALIGNMENT = 32;
// the bytes of the header of a wave archive that LoadWaveArchiveTable reads first
const u32 WAVE_ARCHIVE_HEADER_READ_SIZE = 44;
// the priority of the file streams of the loader
const SoundArchive::FileStreamPriority LOAD_STREAM_PRIORITY = static_cast<SoundArchive::FileStreamPriority>(1);

// the header in front of an individually loaded wave (names are ours)
struct IndividualWaveHeader
{
    static const u32 SIGNATURE = 0x56574943; // 'CIWV'

    u32 signature;      // 0x00
    u32 fileId;         // 0x04 of the wave archive
    u32 waveIndex;      // 0x08
    u8 padding[0x14];   // 0x0C (not written)
};

// (inline) whether the memory lies in the device memory
bool IsInDeviceMemory(const void* pMemory, u32 size)
{
    const uptr address = nn::os::GetDeviceMemoryAddress();
    const uptr end = address + nn::os::GetDeviceMemorySize();
    if (address > reinterpret_cast<uptr>(pMemory)) {
        return false;
    }
    return reinterpret_cast<uptr>(pMemory) + size <= end;
}
} // namespace

// 0x00132564 | nintendogs:callgraph [tier A]
DECOMP_NOINLINE bool nw::snd::internal::SoundArchiveLoader::LoadData(u32 itemId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize)
{
    if (m_pSoundArchive == nullptr || !m_pSoundArchive->IsAvailable() || itemId == 0xFFFFFFFF || pAllocator == nullptr) {
        return false;
    }
    ++m_LoadingCount;
    bool result = false;
    if (!m_IsCancelLoading) {
        switch (SoundArchive::GetItemType(itemId)) {
        case SoundArchive::ITEM_TYPE_SOUND:
            switch (m_pSoundArchive->GetSoundType(itemId)) {
            case SoundArchive::SOUND_TYPE_SEQ:
                result = LoadSequenceSound(itemId, pAllocator, loadFlag, loadBlockSize);
                break;
            case SoundArchive::SOUND_TYPE_STRM:
                result = LoadImpl(m_pSoundArchive->GetItemPrefetchFileId(itemId), pAllocator, loadBlockSize, false) != nullptr;
                break;
            case SoundArchive::SOUND_TYPE_WAVE:
                result = LoadWaveSound(itemId, pAllocator, loadFlag, loadBlockSize, 0xFFFFFFFF);
                break;
            default:
                break;
            }
            break;
        case SoundArchive::ITEM_TYPE_SOUND_GROUP:
            result = LoadSoundGroup(itemId, pAllocator, loadFlag, loadBlockSize);
            break;
        case SoundArchive::ITEM_TYPE_BANK:
            result = LoadBank(itemId, pAllocator, loadFlag, loadBlockSize);
            break;
        case SoundArchive::ITEM_TYPE_WAVE_ARCHIVE:
            if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
                result = LoadImpl(m_pSoundArchive->GetItemFileId(itemId), pAllocator, loadBlockSize, true) != nullptr;
            } else {
                result = true;
            }
            break;
        case SoundArchive::ITEM_TYPE_GROUP: {
            const void* pGroupFile = LoadImpl(m_pSoundArchive->GetItemFileId(itemId), pAllocator, loadBlockSize, true);
            if (pGroupFile != nullptr) {
                result = PostProcessForLoadedGroupFile(pGroupFile, pAllocator, loadBlockSize);
            } else {
                result = false;
            }
            break;
        }
        default:
            break;
        }
    }
    if (--m_LoadingCount == 0) {
        m_IsCancelLoading = false;
    }
    return result;
}

// 0x001347B8 | nintendogs:callgraph [tier A]
bool nw::snd::internal::SoundArchiveLoader::IsAvailable() const
{
    if (m_pSoundArchive == nullptr) {
        return false;
    }
    return m_pSoundArchive->IsAvailable();
}

// 0x001347D8 (name is ours)
bool nw::snd::internal::SoundArchiveLoader::IsBankDataLoaded(u32 bankId, u32 loadFlag) const
{
    const void* pBankFile = GetFileAddressImpl(m_pSoundArchive->GetItemFileId(bankId));
    if ((loadFlag & (LOAD_FLAG_BANK | LOAD_FLAG_WAVE_ARCHIVE)) && pBankFile == nullptr) {
        return false;
    }
    if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
        BankFileReader reader(pBankFile);
        const BinaryTable<WaveId>* pTable = reader.GetWaveIdTable();
        if (pTable == nullptr) {
            return false;
        }
        for (u32 i = 0; i < pTable->count; ++i) {
            const WaveId* pWaveId = i < pTable->count ? &pTable->items[i] : nullptr;
            if (pWaveId == nullptr) {
                return false;
            }
            if (!IsWaveArchiveDataLoaded(pWaveId->waveArchiveId, pWaveId->waveIndex)) {
                return false;
            }
        }
    }
    return true;
}

// 0x001348A8 | nintendogs:bytes [tier B]
bool nw::snd::internal::SoundArchiveLoader::IsWaveSoundDataLoaded(u32 soundId, u32 loadFlag) const
{
    const void* pWaveSoundFile = GetFileAddressImpl(m_pSoundArchive->GetItemFileId(soundId));
    if ((loadFlag & (LOAD_FLAG_WAVE_SOUND | LOAD_FLAG_WAVE_ARCHIVE)) && pWaveSoundFile == nullptr) {
        return false;
    }
    if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
        SoundArchive::WaveSoundInfo info;
        if (!m_pSoundArchive->detail_ReadWaveSoundInfo(soundId, &info)) {
            return false;
        }
        WaveSoundFileReader reader(pWaveSoundFile);
        WaveSoundNoteInfo noteInfo;
        if (!reader.ReadNoteInfo(&noteInfo, info.index, 0)) {
            return false;
        }
        if (!IsWaveArchiveDataLoaded(noteInfo.waveArchiveId, noteInfo.waveIndex)) {
            return false;
        }
    }
    return true;
}

// 0x00134990 (name is ours)
bool nw::snd::internal::SoundArchiveLoader::IsDataLoaded(u32 itemId, u32 loadFlag) const
{
    if (m_pSoundArchive == nullptr || !m_pSoundArchive->IsAvailable() || itemId == 0xFFFFFFFF) {
        return false;
    }
    switch (SoundArchive::GetItemType(itemId)) {
    case SoundArchive::ITEM_TYPE_SOUND:
        switch (m_pSoundArchive->GetSoundType(itemId)) {
        case SoundArchive::SOUND_TYPE_SEQ:
            return IsSequenceSoundDataLoaded(itemId, loadFlag);
        case SoundArchive::SOUND_TYPE_WAVE:
            return IsWaveSoundDataLoaded(itemId, loadFlag);
        default:
            return false;
        }
    case SoundArchive::ITEM_TYPE_SOUND_GROUP:
        return IsSoundGroupDataLoaded(itemId, loadFlag);
    case SoundArchive::ITEM_TYPE_BANK:
        return IsBankDataLoaded(itemId, loadFlag);
    case SoundArchive::ITEM_TYPE_WAVE_ARCHIVE:
        return IsWaveArchiveDataLoaded(itemId, 0xFFFFFFFF);
    case SoundArchive::ITEM_TYPE_GROUP:
        return GetFileAddressImpl(m_pSoundArchive->GetItemFileId(itemId)) != nullptr;
    default:
        return false;
    }
}

// 0x00134AA0 | nintendogs:bytes [tier B]
bool nw::snd::internal::SoundArchiveLoader::IsSoundGroupDataLoaded(u32 soundGroupId, u32 loadFlag) const
{
    SoundArchive::SoundGroupInfo info;
    if (!m_pSoundArchive->detail_ReadSoundGroupInfo(soundGroupId, &info)) {
        return false;
    }
    switch (m_pSoundArchive->GetSoundType(info.startId)) {
    case SoundArchive::SOUND_TYPE_SEQ:
        for (u32 id = info.startId; id <= info.endId; ++id) {
            if (!IsSequenceSoundDataLoaded(id, loadFlag)) {
                return false;
            }
        }
        break;
    case SoundArchive::SOUND_TYPE_WAVE:
        for (u32 id = info.startId; id <= info.endId; ++id) {
            if (!IsWaveSoundDataLoaded(id, loadFlag)) {
                return false;
            }
        }
        break;
    default:
        break;
    }
    return true;
}

// 0x00137D94 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadSoundGroup(u32 soundGroupId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize)
{
    SoundArchive::SoundGroupInfo info;
    if (!m_pSoundArchive->detail_ReadSoundGroupInfo(soundGroupId, &info)) {
        return false;
    }
    if (info.startId == 0xFFFFFFFF) {
        return true;
    }
    switch (m_pSoundArchive->GetSoundType(info.startId)) {
    case SoundArchive::SOUND_TYPE_SEQ:
        for (u32 id = info.startId; id <= info.endId; ++id) {
            if (!LoadSequenceSound(id, pAllocator, loadFlag, loadBlockSize)) {
                return false;
            }
        }
        break;
    case SoundArchive::SOUND_TYPE_WAVE:
        for (u32 id = info.startId; id <= info.endId; ++id) {
            if (!LoadWaveSound(id, pAllocator, loadFlag, loadBlockSize, soundGroupId)) {
                return false;
            }
        }
        break;
    default:
        break;
    }
    return true;
}

// 0x00137F44 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadSequenceSound(u32 soundId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize)
{
    if (loadFlag & LOAD_FLAG_SEQUENCE) {
        if (LoadImpl(m_pSoundArchive->GetItemFileId(soundId), pAllocator, loadBlockSize, false) == nullptr) {
            return false;
        }
    }
    if (loadFlag & (LOAD_FLAG_BANK | LOAD_FLAG_WAVE_ARCHIVE)) {
        SoundArchive::SequenceSoundInfo info;
        if (!m_pSoundArchive->ReadSequenceSoundInfo(soundId, &info)) {
            return false;
        }
        for (int i = 0; i < 4; ++i) {
            const u32 bankId = info.bankIds[i];
            if (bankId != 0xFFFFFFFF && !LoadBank(bankId, pAllocator, loadFlag, loadBlockSize)) {
                return false;
            }
        }
    }
    return true;
}

namespace {
// (inline) the table of the individually loaded wave archive of the first wave of a loaded bank
DECOMP_ALWAYS_INLINE void SetWaveArchiveTableOfBank(SoundArchiveLoader* pLoader, u32 bankId, SoundMemoryAllocatable* pAllocator)
{
    SoundArchive::BankInfo bankInfo;
    if (!pLoader->m_pSoundArchive->ReadBankInfo(bankId, &bankInfo)) {
        return;
    }
    const void* pBankFile = pLoader->GetFileAddressImpl(bankInfo.fileId);
    if (pBankFile == nullptr) {
        return;
    }
    BankFileReader reader(pBankFile);
    const BinaryTable<WaveId>* pTable = reader.GetWaveIdTable();
    if (pTable == nullptr || pTable->count == 0) {
        return;
    }
    const WaveId* pWaveId = &pTable->items[0];
    if (pWaveId == nullptr) {
        return;
    }
    pLoader->SetWaveArchiveTableInEmbeddedGroupImpl(pWaveId->waveArchiveId, pAllocator);
}

// (inline) the tables of the banks of a sequence sound
DECOMP_ALWAYS_INLINE void SetWaveArchiveTableOfSequenceSound(SoundArchiveLoader* pLoader, u32 soundId, SoundMemoryAllocatable* pAllocator)
{
    SoundArchive::SequenceSoundInfo info;
    if (!pLoader->m_pSoundArchive->ReadSequenceSoundInfo(soundId, &info)) {
        return;
    }
    for (int i = 0; i < 4; ++i) {
        if (info.bankIds[i] != 0xFFFFFFFF) {
            SetWaveArchiveTableOfBank(pLoader, info.bankIds[i], pAllocator);
        }
    }
}
} // namespace

// 0x00138028 | nintendogs:callseq [tier A]
bool nw::snd::internal::SoundArchiveLoader::PostProcessForLoadedGroupFile(const void* pGroupFile, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize)
{
    GroupFileReader reader(pGroupFile);
    const u32 itemCount = reader.m_pInfoBlockBody->count;
    bool isEmbedded = true;
    for (u32 i = 0; i < itemCount; ++i) {
        GroupItemLocationInfo locationInfo;
        if (!reader.ReadGroupItemLocationInfo(&locationInfo, i)) {
            return false;
        }
        if (locationInfo.address != nullptr) {
            SetFileAddressToTable(locationInfo.fileId, locationInfo.address);
        } else if (i == 0) {
            isEmbedded = false;
            break;
        }
    }

    if (!isEmbedded) {
        // the files are not in the group: load the items
        const u32 itemExCount = reader.GetGroupItemExCount();
        for (u32 i = 0; i < itemExCount; ++i) {
            GroupFile::GroupItemInfoEx infoEx;
            if (reader.ReadGroupItemInfoEx(&infoEx, i)) {
                if (!LoadData(infoEx.itemId, pAllocator, infoEx.loadFlag, loadBlockSize)) {
                    return false;
                }
            }
        }
        return true;
    }

    // embedded: the individually loaded wave archives need tables of their waves
    const u32 itemExCount = reader.GetGroupItemExCount();
    for (u32 i = 0; i < itemExCount; ++i) {
        GroupFile::GroupItemInfoEx infoEx;
        if (!reader.ReadGroupItemInfoEx(&infoEx, i) || !(infoEx.loadFlag & LOAD_FLAG_WAVE_ARCHIVE)) {
            continue;
        }
        switch (SoundArchive::GetItemType(infoEx.itemId)) {
        case SoundArchive::ITEM_TYPE_SOUND:
            if (m_pSoundArchive->GetSoundType(infoEx.itemId) == SoundArchive::SOUND_TYPE_SEQ) {
                SetWaveArchiveTableOfSequenceSound(this, infoEx.itemId, pAllocator);
            }
            break;
        case SoundArchive::ITEM_TYPE_SOUND_GROUP: {
            SoundArchive::SoundGroupInfo soundGroupInfo;
            if (!m_pSoundArchive->detail_ReadSoundGroupInfo(infoEx.itemId, &soundGroupInfo) || soundGroupInfo.startId == 0xFFFFFFFF) {
                break;
            }
            switch (m_pSoundArchive->GetSoundType(soundGroupInfo.startId)) {
            case SoundArchive::SOUND_TYPE_SEQ:
                for (u32 id = soundGroupInfo.startId; id <= soundGroupInfo.endId; ++id) {
                    SetWaveArchiveTableOfSequenceSound(this, id, pAllocator);
                }
                break;
            case SoundArchive::SOUND_TYPE_WAVE: {
                // the wave archive of the first wave sound
                const u32 soundId = soundGroupInfo.startId;
                if (soundId == 0xFFFFFFFF) {
                    break;
                }
                SoundArchive::SoundInfo soundInfo;
                if (!m_pSoundArchive->ReadSoundInfo(soundId, &soundInfo)) {
                    break;
                }
                SoundArchive::WaveSoundInfo waveSoundInfo;
                if (!m_pSoundArchive->detail_ReadWaveSoundInfo(soundId, &waveSoundInfo)) {
                    break;
                }
                const void* pWaveSoundFile = GetFileAddressImpl(soundInfo.fileId);
                if (pWaveSoundFile == nullptr) {
                    break;
                }
                WaveSoundFileReader waveSoundReader(pWaveSoundFile);
                WaveSoundNoteInfo noteInfo;
                if (!waveSoundReader.ReadNoteInfo(&noteInfo, waveSoundInfo.index, 0)) {
                    break;
                }
                SetWaveArchiveTableInEmbeddedGroupImpl(noteInfo.waveArchiveId, pAllocator);
                break;
            }
            default:
                break;
            }
            break;
        }
        case SoundArchive::ITEM_TYPE_BANK:
            if (infoEx.itemId != 0xFFFFFFFF) {
                SetWaveArchiveTableOfBank(this, infoEx.itemId, pAllocator);
            }
            break;
        default:
            break;
        }
    }
    return true;
}

// 0x00139ADC (name is ours)
bool nw::snd::internal::SoundArchiveLoader::IsWaveArchiveDataLoaded(u32 waveArchiveId, u32 waveIndex) const
{
    const void* pWaveArchiveFile = GetFileAddressImpl(m_pSoundArchive->GetItemFileId(waveArchiveId));
    if (pWaveArchiveFile == nullptr) {
        return false;
    }
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(waveArchiveId, &info)) {
        return false;
    }
    if (info.isLoadIndividual != true) {
        return true;
    }
    WaveArchiveFileReader reader(pWaveArchiveFile, true);
    if (waveIndex != 0xFFFFFFFF) {
        return reader.m_IsInitialized && reader.GetWaveFile(waveIndex) != nullptr;
    }
    const u32 waveCount = reader.GetWaveFileCount();
    for (u32 i = 0; i < waveCount; ++i) {
        if (!reader.m_IsInitialized || reader.GetWaveFile(i) == nullptr) {
            return false;
        }
    }
    return true;
}

// 0x00139BE0 | nintendogs:bytes [tier B]
bool nw::snd::internal::SoundArchiveLoader::IsSequenceSoundDataLoaded(u32 soundId, u32 loadFlag) const
{
    if (loadFlag & LOAD_FLAG_SEQUENCE) {
        if (GetFileAddressImpl(m_pSoundArchive->GetItemFileId(soundId)) == nullptr) {
            return false;
        }
    }
    if (loadFlag & (LOAD_FLAG_BANK | LOAD_FLAG_WAVE_ARCHIVE)) {
        SoundArchive::SequenceSoundInfo info;
        if (!m_pSoundArchive->ReadSequenceSoundInfo(soundId, &info)) {
            return false;
        }
        for (int i = 0; i < 4; ++i) {
            const u32 bankId = info.bankIds[i];
            if (bankId != 0xFFFFFFFF && !IsBankDataLoaded(bankId, loadFlag)) {
                return false;
            }
        }
    }
    return true;
}

// 0x0013BC2C | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadWaveSound(u32 soundId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize, u32 waveSoundSetId)
{
    const u32 fileId = m_pSoundArchive->GetItemFileId(soundId);
    if (loadFlag & LOAD_FLAG_WAVE_SOUND) {
        if (LoadImpl(fileId, pAllocator, loadBlockSize, false) == nullptr) {
            return false;
        }
    }
    if (!(loadFlag & LOAD_FLAG_WAVE_ARCHIVE)) {
        return true;
    }
    const void* pWaveSoundFile = GetFileAddressImpl(fileId);
    if (pWaveSoundFile != nullptr) {
        // the wave archive of the wave
        SoundArchive::WaveSoundInfo info;
        if (!m_pSoundArchive->detail_ReadWaveSoundInfo(soundId, &info)) {
            return false;
        }
        WaveSoundFileReader reader(pWaveSoundFile);
        WaveSoundNoteInfo noteInfo;
        if (!reader.ReadNoteInfo(&noteInfo, info.index, 0)) {
            return false;
        }
        return LoadWaveArchiveImpl(noteInfo.waveArchiveId, noteInfo.waveIndex, pAllocator, loadFlag, loadBlockSize);
    }
    // all wave archives of the wave sound set
    const BinaryTable<u32>* pTable = m_pSoundArchive->detail_GetWaveArchiveIdTable(waveSoundSetId != 0xFFFFFFFF ? waveSoundSetId : soundId);
    for (u32 i = 0; i < pTable->count; ++i) {
        const u32 waveArchiveId = pTable->items[i];
        if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
            if (LoadImpl(m_pSoundArchive->GetItemFileId(waveArchiveId), pAllocator, loadBlockSize, true) == nullptr) {
                return false;
            }
        }
    }
    return true;
}

// 0x0013BDC0 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadWaveArchiveImpl(u32 waveArchiveId, u32 waveIndex, SoundMemoryAllocatable* pAllocator, u32 loadFlag,
                                                                u32 loadBlockSize)
{
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(waveArchiveId, &info)) {
        return false;
    }
    if (info.isLoadIndividual) {
        return LoadIndividualWave(waveArchiveId, waveIndex, pAllocator, loadBlockSize);
    }
    if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
        if (LoadImpl(m_pSoundArchive->GetItemFileId(waveArchiveId), pAllocator, loadBlockSize, true) == nullptr) {
            return false;
        }
    }
    return true;
}

// 0x0013BE80 | nintendogs:callseq [tier A]
void nw::snd::internal::SoundArchiveLoader::SetWaveArchiveTableInEmbeddedGroupImpl(u32 waveArchiveId, SoundMemoryAllocatable* pAllocator)
{
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(waveArchiveId, &info) || !info.isLoadIndividual) {
        return;
    }
    const void* pWaveArchiveFile = GetFileAddressImpl(info.fileId);
    WaveArchiveFileReader reader(pWaveArchiveFile, false);
    if (reader.HasIndividualLoadTable()) {
        return;
    }
    const u32 fileBlockOffset = static_cast<const WaveArchiveFile::FileHeader*>(pWaveArchiveFile)->GetFileBlockOffset();
    u8* pTableFile = static_cast<u8*>(pAllocator->Alloc(fileBlockOffset + info.waveCount * sizeof(u32) + sizeof(u32)));
    if (pTableFile == nullptr) {
        return;
    }
    // the header and the table of the waves in the group
    std::memcpy(pTableFile, pWaveArchiveFile, fileBlockOffset);
    *reinterpret_cast<u32*>(pTableFile + fileBlockOffset) = WaveArchiveFile::SIGNATURE_INDIVIDUAL_LOAD_TABLE;
    WaveArchiveFileReader tableReader(pTableFile, true);
    tableReader.InitializeFileTable();
    for (u32 i = 0; i < info.waveCount; ++i) {
        tableReader.SetWaveFile(i, reader.GetWaveFile(i));
    }
    SetFileAddressToTable(info.fileId, pTableFile);
}

// 0x0013BFB8 | nintendogs:callgraph [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadBank(u32 bankId, SoundMemoryAllocatable* pAllocator, u32 loadFlag, u32 loadBlockSize)
{
    const u32 fileId = m_pSoundArchive->GetItemFileId(bankId);
    if (loadFlag & LOAD_FLAG_BANK) {
        if (LoadImpl(fileId, pAllocator, loadBlockSize, false) == nullptr) {
            return false;
        }
    }
    if (!(loadFlag & LOAD_FLAG_WAVE_ARCHIVE)) {
        return true;
    }
    const void* pBankFile = GetFileAddressImpl(fileId);
    if (pBankFile != nullptr) {
        // the wave archives of the waves of the bank
        BankFileReader reader(pBankFile);
        const BinaryTable<WaveId>* pTable = reader.GetWaveIdTable();
        if (pTable == nullptr) {
            return false;
        }
        for (u32 i = 0; i < pTable->count; ++i) {
            const WaveId* pWaveId = i < pTable->count ? &pTable->items[i] : nullptr;
            if (pWaveId == nullptr) {
                return false;
            }
            if (!LoadWaveArchiveImpl(pWaveId->waveArchiveId, pWaveId->waveIndex, pAllocator, loadFlag, loadBlockSize)) {
                return false;
            }
        }
        return true;
    }
    // all wave archives of the bank
    const BinaryTable<u32>* pTable = m_pSoundArchive->detail_GetWaveArchiveIdTable(bankId);
    for (u32 i = 0; i < pTable->count; ++i) {
        const u32 waveArchiveId = pTable->items[i];
        if (loadFlag & LOAD_FLAG_WAVE_ARCHIVE) {
            if (LoadImpl(m_pSoundArchive->GetItemFileId(waveArchiveId), pAllocator, loadBlockSize, true) == nullptr) {
                return false;
            }
        }
    }
    return true;
}

// 0x0013C1C4 | nintendogs:bytes [tier A]
const void* nw::snd::internal::SoundArchiveLoader::LoadImpl(u32 fileId, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize, bool isDeviceMemory)
{
    const void* pFile = GetFileAddressImpl(fileId);
    if (pFile != nullptr) {
        return pFile;
    }
    SoundArchive::FileInfo fileInfo;
    if (!m_pSoundArchive->detail_ReadFileInfo(fileId, &fileInfo)) {
        return nullptr;
    }
    const u32 fileSize = fileInfo.fileSize;
    if (fileSize == 0 || fileSize == 0xFFFFFFFF) {
        return nullptr;
    }
    void* pBuffer = pAllocator->Alloc(fileSize);
    if (pBuffer == nullptr) {
        return nullptr;
    }
    if (isDeviceMemory && !IsInDeviceMemory(pBuffer, fileSize)) {
        pBuffer = nullptr;
    } else {
        if (ReadFile(fileId, pBuffer, fileSize, 0, loadBlockSize) != static_cast<s32>(fileSize)) {
            return nullptr;
        }
        nn::snd::CTR::FlushDataCache(reinterpret_cast<uptr>(pBuffer), fileSize);
    }
    if (pBuffer == nullptr) {
        return nullptr;
    }
    SetFileAddressToTable(fileId, pBuffer);
    return pBuffer;
}

// 0x0013C2EC | nintendogs:bytes [tier A]
nw::snd::internal::SoundArchiveLoader::SoundArchiveLoader() : m_pSoundArchive(nullptr), m_LoadingCount(0), m_IsCancelLoading(false)
{
}

// 0x0013F19C | nintendogs:callgraph [tier A]
bool nw::snd::internal::SoundArchiveLoader::LoadIndividualWave(u32 waveArchiveId, u32 waveIndex, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize)
{
    const u32 fileId = m_pSoundArchive->GetItemFileId(waveArchiveId);
    const void* pTableFile = GetFileAddressImpl(fileId);
    if (pTableFile == nullptr) {
        pTableFile = LoadWaveArchiveTable(waveArchiveId, pAllocator, loadBlockSize);
        if (pTableFile == nullptr) {
            return false;
        }
    }
    WaveArchiveFileReader reader(pTableFile, true);
    if (reader.m_IsInitialized && reader.GetWaveFile(waveIndex) != nullptr) {
        return true;
    }
    const u32 waveFileSize = reader.GetWaveFileSize(waveIndex);
    const u32 allocSize = waveFileSize + sizeof(IndividualWaveHeader);
    u8* pBuffer = static_cast<u8*>(pAllocator->Alloc(allocSize));
    if (pBuffer == nullptr) {
        return false;
    }
    if (!IsInDeviceMemory(pBuffer, allocSize)) {
        return false;
    }
    IndividualWaveHeader header;
    header.signature = IndividualWaveHeader::SIGNATURE;
    header.fileId = fileId;
    header.waveIndex = waveIndex;
    std::memcpy(pBuffer, &header, sizeof(header));
    u8* pWaveFile = pBuffer + sizeof(IndividualWaveHeader);
    if (ReadFile(fileId, pWaveFile, waveFileSize, reader.GetWaveFileOffsetFromFileHead(waveIndex), loadBlockSize) != static_cast<s32>(waveFileSize)) {
        return false;
    }
    reader.SetWaveFile(waveIndex, pWaveFile);
    nn::snd::CTR::FlushDataCache(reinterpret_cast<uptr>(pWaveFile), waveFileSize);
    return true;
}

// 0x0014123C | nintendogs:callseq [tier A]
const void* nw::snd::internal::SoundArchiveLoader::LoadWaveArchiveTable(u32 waveArchiveId, SoundMemoryAllocatable* pAllocator, u32 loadBlockSize)
{
    const u32 fileId = m_pSoundArchive->GetItemFileId(waveArchiveId);
    const void* pTableFile = GetFileAddressImpl(fileId);
    if (pTableFile != nullptr) {
        return pTableFile;
    }
    SoundArchive::WaveArchiveInfo info;
    if (!m_pSoundArchive->ReadWaveArchiveInfo(waveArchiveId, &info) || info.waveCount == 0) {
        return nullptr;
    }
    const u32 waveCount = info.waveCount;

    // the header, for the offset of the file block
    u8 headerBuffer[WAVE_ARCHIVE_HEADER_READ_SIZE + READ_ALIGNMENT];
    const WaveArchiveFile::FileHeader* pHeader = reinterpret_cast<const WaveArchiveFile::FileHeader*>(ut::RoundUp(headerBuffer, READ_ALIGNMENT));
    if (ReadFile(fileId, const_cast<WaveArchiveFile::FileHeader*>(pHeader), WAVE_ARCHIVE_HEADER_READ_SIZE, 0, loadBlockSize) !=
        static_cast<s32>(WAVE_ARCHIVE_HEADER_READ_SIZE)) {
        return nullptr;
    }
    const u32 fileBlockOffset = pHeader->GetFileBlockOffset();
    if (pHeader->GetInfoBlockOffset() > fileBlockOffset) {
        return nullptr;
    }

    // up to the file block and a table of the waves behind it
    const u32 size = fileBlockOffset + waveCount * sizeof(u32) + sizeof(u32);
    u8* pBuffer = static_cast<u8*>(pAllocator->Alloc(size));
    if (!IsInDeviceMemory(pBuffer, size)) {
        return nullptr;
    }
    if (ReadFile(fileId, pBuffer, fileBlockOffset, 0, loadBlockSize) != static_cast<s32>(fileBlockOffset)) {
        return nullptr;
    }
    *reinterpret_cast<u32*>(pBuffer + fileBlockOffset) = WaveArchiveFile::SIGNATURE_INDIVIDUAL_LOAD_TABLE;
    WaveArchiveFileReader reader(pBuffer, true);
    reader.InitializeFileTable();
    SetFileAddressToTable(fileId, pBuffer);
    return pBuffer;
}

// 0x001413C4 | nintendogs:callgraph [tier A]
s32 nw::snd::internal::SoundArchiveLoader::ReadFile(u32 fileId, void* pBuffer, s32 size, s32 offset, u32 loadBlockSize)
{
    // closes the stream at every return, after the lock is left (name is ours)
    struct StreamCloser
    {
        ~StreamCloser()
        {
            if (pStream != nullptr) {
                pStream->Close();
            }
        }
        io::FileStream* pStream;
    };
    StreamCloser stream = {nullptr};
    SoundArchive* pSoundArchive = const_cast<SoundArchive*>(m_pSoundArchive);
    nn::os::CriticalSection::ScopedLock lock(pSoundArchive->m_FileOpenLock);
    stream.pStream = pSoundArchive->detail_OpenFileStream(fileId, m_StreamArea, STREAM_AREA_SIZE, nullptr, 0, nullptr, LOAD_STREAM_PRIORITY);
    if (stream.pStream == nullptr) {
        return -1;
    }
    if (!stream.pStream->CanSeek() || !stream.pStream->IsAvailable()) {
        return -1;
    }
    stream.pStream->Seek(offset, io::FileStream::SEEK_ORIGIN_BEGIN);
    if (loadBlockSize == 0) {
        if (stream.pStream->Read(pBuffer, ut::RoundUp(static_cast<u32>(size), READ_ALIGNMENT)) < 0) {
            return -1;
        }
    } else {
        // in blocks, so that the loading can be canceled
        u8* p = static_cast<u8*>(pBuffer);
        for (s32 rest = size; rest > 0;) {
            if (m_IsCancelLoading) {
                return -1;
            }
            const s32 readSize = static_cast<s32>(loadBlockSize) > rest ? rest : static_cast<s32>(loadBlockSize);
            const s32 readResult = stream.pStream->Read(p, ut::RoundUp(static_cast<u32>(readSize), READ_ALIGNMENT));
            if (readResult < 0) {
                return -1;
            }
            if (rest <= readResult) {
                break;
            }
            rest -= readResult;
            p += readResult;
        }
    }
    return size;
}

// 0x004C8B2C | nintendogs:callgraph [tier A]
void nw::snd::internal::SoundArchiveLoader::SetSoundArchive(const SoundArchive* pSoundArchive)
{
    m_pSoundArchive = pSoundArchive;
}

// 0x004C8B34 | nintendogs:callseq [tier A]
bool nw::snd::internal::SoundArchiveLoader::detail_LoadWaveArchiveByBankFile(const void* pBankFile, SoundMemoryAllocatable* pAllocator)
{
    if (pBankFile == nullptr) {
        return false;
    }
    BankFileReader reader(pBankFile);
    const BinaryTable<WaveId>* pTable = reader.GetWaveIdTable();
    if (pTable == nullptr) {
        return false;
    }
    for (u32 i = 0; i < pTable->count; ++i) {
        const WaveId* pWaveId = i < pTable->count ? &pTable->items[i] : nullptr;
        if (pWaveId == nullptr) {
            return false;
        }
        if (!LoadWaveArchiveImpl(pWaveId->waveArchiveId, pWaveId->waveIndex, pAllocator, LOAD_FLAG_WAVE_ARCHIVE, 0)) {
            return false;
        }
    }
    return true;
}

// 0x004C8C54 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveLoader::detail_LoadWaveArchiveByWaveSoundFile(const void* pWaveSoundFile, int index, SoundMemoryAllocatable* pAllocator)
{
    if (pWaveSoundFile == nullptr) {
        return false;
    }
    WaveSoundFileReader reader(pWaveSoundFile);
    WaveSoundNoteInfo noteInfo;
    if (!reader.ReadNoteInfo(&noteInfo, index, 0)) {
        return false;
    }
    return LoadWaveArchiveImpl(noteInfo.waveArchiveId, noteInfo.waveIndex, pAllocator, LOAD_FLAG_WAVE_ARCHIVE, 0);
}

// 0x004C8CF4 slot 0x00 | fefates:callgraph
// 0x004C8CDC (deleting dtor)
nw::snd::internal::SoundArchiveLoader::~SoundArchiveLoader()
{
    m_pSoundArchive = nullptr;
}

// 0x007406F8 | nintendogs:callseq-callee [tier A]
const void* nw::snd::internal::SoundArchiveLoader::detail_GetFileAddressByItemId(u32 itemId) const
{
    if (m_pSoundArchive == nullptr) {
        return nullptr;
    }
    u32 fileId;
    switch (SoundArchive::GetItemType(itemId)) {
    case SoundArchive::ITEM_TYPE_SOUND: {
        SoundArchive::SoundInfo info;
        m_pSoundArchive->ReadSoundInfo(itemId, &info);
        fileId = info.fileId;
        break;
    }
    case SoundArchive::ITEM_TYPE_BANK: {
        SoundArchive::BankInfo info;
        m_pSoundArchive->ReadBankInfo(itemId, &info);
        fileId = info.fileId;
        break;
    }
    case SoundArchive::ITEM_TYPE_WAVE_ARCHIVE: {
        SoundArchive::WaveArchiveInfo info;
        m_pSoundArchive->ReadWaveArchiveInfo(itemId, &info);
        fileId = info.fileId;
        break;
    }
    case SoundArchive::ITEM_TYPE_GROUP: {
        SoundArchive::GroupInfo info;
        m_pSoundArchive->detail_ReadGroupInfo(itemId, &info);
        fileId = info.fileId;
        break;
    }
    default:
        return nullptr;
    }
    if (fileId == 0xFFFFFFFF) {
        return nullptr;
    }
    return GetFileAddressImpl(fileId);
}

// 0x007407DC | nintendogs:bytes [tier A]
const void* nw::snd::internal::SoundArchiveLoader::GetFileAddressFromSoundArchive(u32 fileId) const
{
    if (m_pSoundArchive == nullptr) {
        return nullptr;
    }
    return m_pSoundArchive->detail_GetFileAddress(fileId);
}

} // namespace internal
} // namespace snd
} // namespace nw
