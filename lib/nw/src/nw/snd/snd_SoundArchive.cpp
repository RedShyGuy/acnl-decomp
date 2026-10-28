#include "nw/snd/snd_SoundArchive.h"

#include <cstring>

#include "nw/snd/internal/snd_SoundArchiveFileReader.h"

namespace nw {
namespace snd {
namespace {
// (inline) at most dstCount - 1 characters of the first srcCount ones of the string copied, with
// the end of the string (name is ours)
inline void CopyString(char* pDst, u32 dstCount, const char* pSrc, u32 srcCount)
{
    u32 length = 0;
    while (length < dstCount - 1 && length < srcCount && pSrc[length] != '\0') {
        pDst[length] = pSrc[length];
        ++length;
    }
    pDst[length] = '\0';
}

// the separator of the directories of a path
const char PATH_SEPARATOR = '/';
} // namespace

// 0x00139ACC | nintendogs:callseq-callee [tier A]
bool nw::snd::SoundArchive::IsAvailable() const
{
    return m_pFileReader != nullptr;
}

// 0x00139D4C | nintendogs:callgraph [tier A]
nw::snd::SoundArchive::SoundType nw::snd::SoundArchive::GetSoundType(u32 soundId) const
{
    return m_pFileReader->GetSoundType(soundId);
}

// 0x0013CF84 | nintendogs:callseq-callee [tier A]
bool nw::snd::SoundArchive::ReadBankInfo(u32 bankId, BankInfo* pInfo) const
{
    bool result = m_pFileReader->ReadBankInfo(bankId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadBankInfo(bankId, pInfo) : false;
    }
    return result;
}

// 0x0013CFF0 | nintendogs:callseq-callee [tier A]
bool nw::snd::SoundArchive::ReadSoundInfo(u32 soundId, SoundInfo* pInfo) const
{
    bool result = m_pFileReader->ReadSoundInfo(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSoundInfo(soundId, pInfo) : false;
    }
    return result;
}

// 0x0013D048 | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::ReadSequenceSoundInfo(u32 soundId, SequenceSoundInfo* pInfo) const
{
    bool result = m_pFileReader->ReadSequenceSoundInfo(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSequenceSoundInfo(soundId, pInfo) : false;
    }
    return result;
}

// 0x0013D0A0 | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::detail_ReadWaveSoundInfo(u32 soundId, WaveSoundInfo* pInfo) const
{
    bool result = m_pFileReader->ReadWaveSoundInfo(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadWaveSoundInfo(soundId, pInfo) : false;
    }
    return result;
}

// 0x0013D0F8 | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::detail_ReadSoundGroupInfo(u32 soundGroupId, SoundGroupInfo* pInfo) const
{
    bool result = m_pFileReader->ReadSoundGroupInfo(soundGroupId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSoundGroupInfo(soundGroupId, pInfo) : false;
    }
    return result;
}

// 0x0013D520 | nintendogs:callgraph [tier A]
u32 nw::snd::SoundArchive::GetItemFileId(u32 id) const
{
    return m_pFileReader->GetItemFileId(id);
}

// 0x0013D5D8 (name after the InfoBlockBody)
u32 nw::snd::SoundArchive::GetItemPrefetchFileId(u32 id) const
{
    return m_pFileReader->GetItemPrefetchFileId(id);
}

// 0x0013FA10 | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::ReadWaveArchiveInfo(u32 waveArchiveId, WaveArchiveInfo* pInfo) const
{
    bool result = m_pFileReader->ReadWaveArchiveInfo(waveArchiveId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadWaveArchiveInfo(waveArchiveId, pInfo) : false;
    }
    return result;
}

// 0x0013FA68 | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::detail_ReadFileInfo(u32 fileId, FileInfo* pInfo) const
{
    if (!m_pFileReader->ReadFileInfo(fileId, pInfo, 0)) {
        return false;
    }
    if (m_pInfoHook != nullptr && m_pInfoHook->IsEnabled()) {
        m_pInfoHook->ReadFileInfo(fileId, pInfo, 0);
    }
    return true;
}

// 0x001410F4 | fefates:bytes [tier B]
nw::io::FileStream* nw::snd::SoundArchive::detail_OpenFileStream(u32 fileId, void* pBuffer, int bufferSize, void* pCacheBuffer,
                                                                 u32 cacheSize, const char* pExtFilePath, FileStreamPriority priority)
{
    FileInfo fileInfo;
    if (!m_pFileReader->ReadFileInfo(fileId, &fileInfo, 0)) {
        return nullptr;
    }
    if (m_pInfoHook != nullptr && m_pInfoHook->IsEnabled()) {
        m_pInfoHook->ReadFileInfo(fileId, &fileInfo, 0);
    }

    // an external file: the given path wins
    if (fileInfo.externalFilePath != nullptr) {
        if (pExtFilePath != nullptr) {
            return OpenExtStreamImpl(pBuffer, bufferSize, pExtFilePath, pCacheBuffer, cacheSize, priority);
        }
        return OpenExtStreamImpl(pBuffer, bufferSize, fileInfo.externalFilePath, pCacheBuffer, cacheSize, priority);
    }
    if (fileInfo.offsetFromFileBlockHead == SoundArchive::INVALID_ID || fileInfo.fileSize == SoundArchive::INVALID_ID) {
        return nullptr;
    }
    return OpenStream(pBuffer, bufferSize, m_FileBlockOffset + fileInfo.offsetFromFileBlockHead, fileInfo.fileSize);
}

// 0x0014200C | nintendogs:callgraph [tier A]
const nw::snd::internal::BinaryTable<u32>* nw::snd::SoundArchive::detail_GetWaveArchiveIdTable(u32 id) const
{
    return m_pFileReader->GetWaveArchiveIdTable(id);
}

// 0x00143980 | nintendogs:bytes-fuzzy [tier A]
nw::io::FileStream* nw::snd::SoundArchive::OpenExtStreamImpl(void* pBuffer, int bufferSize, const char* pExtFilePath, void* pCacheBuffer,
                                                             u32 cacheSize, FileStreamPriority priority) const
{
    if (pExtFilePath[0] == PATH_SEPARATOR) {
        return OpenExtStream(pBuffer, bufferSize, pExtFilePath, pCacheBuffer, cacheSize, priority);
    }
    // a relative path: from the external file root
    const u32 pathLength = std::strlen(pExtFilePath);
    const u32 rootLength = std::strlen(m_ExtFileRoot);
    if (rootLength + pathLength >= FILENAME_MAX) {
        return nullptr;
    }
    char fullPath[FILENAME_MAX];
    CopyString(fullPath, FILENAME_MAX, m_ExtFileRoot, rootLength + 1);
    u32 count = pathLength + 1;
    const u32 length = std::strlen(fullPath);
    if (length + count >= FILENAME_MAX) {
        count = FILENAME_MAX - 1 - length;
    }
    std::strncat(fullPath, pExtFilePath, count);
    fullPath[FILENAME_MAX - 1] = '\0';
    return OpenExtStream(pBuffer, bufferSize, fullPath, pCacheBuffer, cacheSize, priority);
}

// 0x004C01B4 | fefates:bytes [tier B]
void nw::snd::SoundArchive::Initialize(internal::SoundArchiveFileReader* pFileReader)
{
    m_pFileReader = pFileReader;
    m_FileBlockOffset = pFileReader->m_Header.GetFileBlockOffset();
    m_FileOpenLock.Initialize();
}

// 0x004C022C | nintendogs:bytes-fuzzy [tier A]
void nw::snd::SoundArchive::SetExternalFileRoot(const char* pExtFileRoot)
{
    u32 length = std::strlen(pExtFileRoot);
    CopyString(m_ExtFileRoot, FILENAME_MAX, pExtFileRoot, length);
    if (pExtFileRoot[length - 1] != PATH_SEPARATOR) {
        m_ExtFileRoot[length] = PATH_SEPARATOR;
        ++length;
    }
    m_ExtFileRoot[length] = '\0';
}

// 0x004C02EC | fefates:bytes [tier B]
void nw::snd::SoundArchive::Finalize()
{
    m_pFileReader = nullptr;
    m_ExtFileRoot[0] = PATH_SEPARATOR;
    m_ExtFileRoot[1] = '\0';
}

// 0x004C0304 | fefates:bytes [tier B]
nw::snd::SoundArchive::SoundArchive() : m_pFileReader(nullptr), m_pInfoHook(nullptr)
{
    m_ExtFileRoot[0] = PATH_SEPARATOR;
    m_ExtFileRoot[1] = '\0';
}

// 0x004C0348 slot 0x00 | fefates:callgraph
// 0x004C0338 (deleting dtor)
nw::snd::SoundArchive::~SoundArchive()
{
    // empty (in the original too)
}

// 0x0073EAF4 | fefates:bytes [tier B]
const char* nw::snd::SoundArchive::GetItemLabel(u32 id) const
{
    if (m_pInfoHook != nullptr && m_pInfoHook->IsEnabled()) {
        const char* pLabel = m_pInfoHook->GetItemLabel(id);
        if (pLabel != nullptr) {
            return pLabel;
        }
    }
    return m_pFileReader->GetItemLabel(id);
}

// 0x0073EB3C (name after the other Read functions)
bool nw::snd::SoundArchive::detail_ReadGroupInfo(u32 groupId, GroupInfo* pInfo) const
{
    bool result = m_pFileReader->ReadGroupInfo(groupId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadGroupInfo(groupId, pInfo) : false;
    }
    return result;
}

// 0x0073EB94 | fefates:bytes [tier B]
bool nw::snd::SoundArchive::ReadPlayerInfo(u32 playerId, PlayerInfo* pInfo) const
{
    bool result = m_pFileReader->ReadPlayerInfo(playerId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadPlayerInfo(playerId, pInfo) : false;
    }
    return result;
}

// 0x0073EBEC | fefates:bytes [tier B]
bool nw::snd::SoundArchive::ReadSound3DInfo(u32 soundId, Sound3DInfo* pInfo) const
{
    bool result = m_pFileReader->ReadSound3DInfo(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSound3DInfo(soundId, pInfo) : false;
    }
    return result;
}

// 0x0073EC44 | fefates:bytes [tier B]
bool nw::snd::SoundArchive::ReadSoundUserParam(u32 soundId, int index, u32& value) const
{
    bool result = m_pFileReader->ReadSoundUserParam(soundId, index, value);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSoundUserParam(soundId, index, value) : false;
    }
    return result;
}

// 0x0073ECA4 | fefates:bytes [tier B]
bool nw::snd::SoundArchive::ReadStreamSoundInfo(u32 soundId, StreamSoundInfo* pInfo) const
{
    bool result = m_pFileReader->ReadStreamSoundInfo(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadStreamSoundInfo(soundId, pInfo) : false;
    }
    return result;
}

// 0x0073ECFC | nintendogs:callgraph [tier A]
bool nw::snd::SoundArchive::ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const
{
    bool result = m_pFileReader->ReadSoundArchivePlayerInfo(pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadSoundArchivePlayerInfo(pInfo) : false;
    }
    return result;
}

// 0x0073ED4C | fefates:bytes [tier B]
bool nw::snd::SoundArchive::detail_ReadStreamSoundInfo2(u32 soundId, StreamSoundInfo2* pInfo) const
{
    bool result = m_pFileReader->ReadStreamSoundInfo2(soundId, pInfo);
    if (m_pInfoHook != nullptr) {
        result |= m_pInfoHook->IsEnabled() ? m_pInfoHook->ReadStreamSoundInfo2(soundId, pInfo) : false;
    }
    return result;
}

// 0x0073EDAC | fefates:bytes [tier B]
u32 nw::snd::SoundArchive::GetItemId(const char* pLabel) const
{
    if (m_pInfoHook != nullptr && m_pInfoHook->IsEnabled()) {
        const u32 id = m_pInfoHook->GetItemId(pLabel);
        if (id != INVALID_ID) {
            return id;
        }
    }
    return m_pFileReader->GetItemId(pLabel);
}

// 0x00740C94 | nintendogs:callgraph [tier A]
u32 nw::snd::SoundArchive::detail_GetFileCount() const
{
    return m_pFileReader->GetFileCount();
}

// 0x00740CEC | nintendogs:callgraph [tier C]
u32 nw::snd::SoundArchive::GetGroupCount() const
{
    return m_pFileReader->GetGroupCount();
}

// 0x00740D08 (name is ours)
u32 nw::snd::SoundArchive::GetSoundCount() const
{
    return m_pFileReader->GetSoundCount();
}

// 0x00740DB0 (name after GetGroupCount)
u32 nw::snd::SoundArchive::GetPlayerCount() const
{
    return m_pFileReader->GetPlayerCount();
}

// 0x00740E78 | nintendogs:callseq-callee [tier A]
u32 nw::snd::SoundArchive::GetWaveArchiveCount() const
{
    return m_pFileReader->GetWaveArchiveCount();
}

} // namespace snd
} // namespace nw
