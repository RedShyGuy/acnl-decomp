#include "nw/snd/snd_MemorySoundArchive.h"

#include <new>

#include "nw/snd/internal/snd_CachedFileStream.h"
#include "nw/snd/internal/snd_FileReader.h"
#include "nw/snd/internal/snd_FsFileStream.h"
#include "nw/snd/internal/snd_GroupFileReader.h"
#include "nw/snd/internal/snd_MemoryFileStream.h"

namespace nw {
namespace snd {
// 0x004C0988 | nintendogs:callgraph [tier A]
bool nw::snd::MemorySoundArchive::Initialize(const void* pSoundArchiveFile)
{
    m_ArchiveFileReader.Initialize(pSoundArchiveFile);
    SoundArchive::Initialize(&m_ArchiveFileReader);
    const u32 infoBlockOffset = m_ArchiveFileReader.GetInfoBlockOffset();
    // (the size is read but not used)
    m_ArchiveFileReader.GetInfoBlockSize();
    m_ArchiveFileReader.SetInfoBlock(static_cast<const u8*>(pSoundArchiveFile) + infoBlockOffset);
    const u32 stringBlockOffset = m_ArchiveFileReader.GetStringBlockOffset();
    const u32 stringBlockSize = m_ArchiveFileReader.GetStringBlockSize();
    if (stringBlockOffset != 0xFFFFFFFF && stringBlockSize != 0xFFFFFFFF) {
        m_ArchiveFileReader.SetStringBlock(static_cast<const u8*>(pSoundArchiveFile) + stringBlockOffset);
    }
    m_pData = pSoundArchiveFile;
    return true;
}

// 0x004C0A00 slot 0x10
nw::io::FileStream* nw::snd::MemorySoundArchive::OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length)
{
    if (m_pData == nullptr || bufferSize < static_cast<int>(sizeof(internal::MemoryFileStream))) {
        return nullptr;
    }
    if (pBuffer == nullptr) {
        return nullptr;
    }
    return new (pBuffer) internal::MemoryFileStream(static_cast<const u8*>(m_pData) + begin, length);
}

// 0x004C02C8 (name is ours)
void nw::snd::MemorySoundArchive::Finalize()
{
    m_pData = nullptr;
    m_ArchiveFileReader.Finalize();
    SoundArchive::Finalize();
}

// 0x004C0A44
void nw::snd::MemorySoundArchive::FsErrorCallbackHandler(nn::Result result, void* pArg)
{
    if (pArg == nullptr) {
        return;
    }
    MemorySoundArchive* pArchive = static_cast<MemorySoundArchive*>(pArg);
    nn::os::CriticalSection::ScopedLock lock(pArchive->m_ResultLock);
    pArchive->m_LastFsResult = result;
}

// 0x004C0A74 (symbols.json: [ctor?] without the parameter)
nw::snd::MemorySoundArchive::MemorySoundArchive(bool isTry)
    : m_pData(nullptr), m_LastFsResult(internal::FileReader::RESULT_NOT_INITIALIZED), m_IsTry(isTry)
{
    m_ResultLock.Initialize();
}

// 0x004C0AEC slot 0x00
// 0x004C0AC8 (deleting dtor)
nw::snd::MemorySoundArchive::~MemorySoundArchive()
{
    m_ResultLock.Finalize();
}

// 0x0073EEB0 slot 0x14
nw::io::FileStream* nw::snd::MemorySoundArchive::OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath,
                                                             void* pCacheBuffer, u32 cacheSize, FileStreamPriority priority) const
{
    if (bufferSize < static_cast<int>(sizeof(internal::FsFileStream) + sizeof(internal::CachedFileStream))) {
        return nullptr;
    }
    internal::FsFileStream* pFsStream =
        pBuffer != nullptr ? new (pBuffer) internal::FsFileStream(pExtFilePath, 0, 0, m_IsTry, FsErrorCallbackHandler,
                                                                  const_cast<MemorySoundArchive*>(this))
                           : nullptr;
    if (!pFsStream->m_IsOpened) {
        return nullptr;
    }
    bool result;
    switch (priority) {
    case FILE_STREAM_PRIORITY_HIGH:
        result = pFsStream->SetPriority(internal::FileReader::PRIORITY_HIGH);
        break;
    case FILE_STREAM_PRIORITY_NORMAL:
        result = pFsStream->SetPriority(internal::FileReader::PRIORITY_NORMAL);
        break;
    case FILE_STREAM_PRIORITY_LOW:
        result = pFsStream->SetPriority(internal::FileReader::PRIORITY_LOW);
        break;
    default:
        return nullptr;
    }
    if (!result) {
        return nullptr;
    }
    void* pCachedStreamBuffer = static_cast<u8*>(pBuffer) + sizeof(internal::FsFileStream);
    internal::CachedFileStream* pStream =
        pCachedStreamBuffer != nullptr ? new (pCachedStreamBuffer) internal::CachedFileStream(*pFsStream) : nullptr;
    if (pStream != nullptr && pCacheBuffer != nullptr && cacheSize != 0) {
        pStream->SetCacheBuffer(pCacheBuffer, cacheSize);
    }
    return pStream;
}

// 0x0073EFC0 slot 0x08 | nintendogs:callseq
const void* nw::snd::MemorySoundArchive::detail_GetFileAddress(u32 fileId) const
{
    // a file of the file block
    FileInfo fileInfo;
    if (detail_ReadFileInfo(fileId, &fileInfo) && fileInfo.offsetFromFileBlockHead != 0xFFFFFFFF) {
        return static_cast<const u8*>(m_pData) + (m_ArchiveFileReader.GetFileBlockOffset() + fileInfo.offsetFromFileBlockHead);
    }

    // a file embedded in a group
    const u32 groupCount = GetGroupCount();
    for (u32 i = 0; i < groupCount; i++) {
        GroupInfo groupInfo;
        if (!detail_ReadGroupInfo(MakeItemId(ITEM_TYPE_GROUP, i), &groupInfo)) {
            continue;
        }
        FileInfo groupFileInfo;
        if (!detail_ReadFileInfo(groupInfo.fileId, &groupFileInfo)) {
            continue;
        }
        internal::GroupFileReader reader(static_cast<const u8*>(m_pData) +
                                         (m_ArchiveFileReader.GetFileBlockOffset() + groupFileInfo.offsetFromFileBlockHead));
        const u32 itemCount = reader.m_pInfoBlockBody->count;
        for (u32 j = 0; j < itemCount; j++) {
            internal::GroupItemLocationInfo locationInfo;
            if (!reader.ReadGroupItemLocationInfo(&locationInfo, j)) {
                continue;
            }
            if (locationInfo.fileId == fileId && locationInfo.address != nullptr) {
                return locationInfo.address;
            }
        }
    }
    return nullptr;
}

// 0x0073F0FC slot 0x0C
size_t nw::snd::MemorySoundArchive::detail_GetRequiredStreamBufferSize() const
{
    return sizeof(internal::MemoryFileStream);
}

} // namespace snd
} // namespace nw
