#include "nw/snd/internal/snd_FsSoundArchiveBase.h"

#include <cstring>
#include <new>

#include "nw/snd/internal/snd_CachedFileStream.h"
#include "nw/snd/internal/snd_FsFileStream.h"
#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C867C slot 0x10 | fefates:bytes
nw::io::FileStream* nw::snd::internal::FsSoundArchiveBase::OpenStream(void* pBuffer, int bufferSize, u32 begin, u32 length)
{
    if (!m_IsOpened || bufferSize < static_cast<int>(sizeof(FsFileStream))) {
        return nullptr;
    }
    FsFileStream* pStream =
        pBuffer != nullptr ? new (pBuffer) FsFileStream(&m_FileReader, begin, length, FsErrorCallbackHandler, this) : nullptr;
    if (!pStream->m_IsOpened) {
        return nullptr;
    }
    return pStream;
}

// 0x004C8794 | fefates:bytes [tier B]
void nw::snd::internal::FsSoundArchiveBase::FsErrorCallbackHandler(nn::Result result, void* pArg)
{
    if (pArg == nullptr) {
        return;
    }
    FsSoundArchiveBase* pArchive = static_cast<FsSoundArchiveBase*>(pArg);
    nn::os::CriticalSection::ScopedLock lock(pArchive->m_ResultLock);
    pArchive->m_LastFsResult = result;
}

// 0x004C87C4 | fefates:bytes [tier B]
bool nw::snd::internal::FsSoundArchiveBase::Open(const char* pPath)
{
    if (!m_FileReader.Initialize(pPath, m_IsTry)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    m_IsOpened = true;

    // the header
    u8 headerBuffer[HEADER_SIZE + 32];
    void* pHeader = ut::RoundUp(headerBuffer, 32);
    int readSize = 0;
    if (!m_FileReader.Read(pHeader, HEADER_SIZE, &readSize)) {
        m_LastFsResult = m_FileReader.m_Result;
        return false;
    }
    if (readSize != HEADER_SIZE) {
        return false;
    }
    m_ArchiveFileReader.Initialize(pHeader);
    SoundArchive::Initialize(&m_ArchiveFileReader);

    // the external files are in the directory of the archive
    int index = std::strlen(pPath) - 1;
    for (; index >= 0; index--) {
        if (pPath[index] == '/' || pPath[index] == '\\') {
            break;
        }
    }
    if (index >= 0) {
        if (index >= FILENAME_MAX) {
            return false;
        }
        char extFileRoot[FILENAME_MAX];
        const int copyCount = index < FILENAME_MAX - 1 ? index : FILENAME_MAX - 1;
        char* pDest = extFileRoot;
        const char* pSrc = pPath;
        for (int i = 0; i < copyCount;) {
            *pDest++ = *pSrc;
            i++;
            if (*++pSrc == '\0') {
                break;
            }
        }
        *pDest = '\0';
        extFileRoot[index] = '\0';
        SetExternalFileRoot(extFileRoot);
    }
    return true;
}

// 0x004C8910 | fefates:bytes-fuzzy [tier B]
void nw::snd::internal::FsSoundArchiveBase::Close()
{
    if (m_IsOpened) {
        m_FileReader.Finalize();
        m_ArchiveFileReader.Finalize();
        m_IsOpened = false;
    }
    Finalize();
}

// 0x004C8948 | fefates:bytes [tier B]
nw::snd::internal::FsSoundArchiveBase::FsSoundArchiveBase(bool isTry)
    : m_LastFsResult(), m_IsOpened(false), m_IsTry(isTry)
{
    m_ResultLock.Initialize();
}

// 0x004C8A60 slot 0x00 | fefates:bytes
// 0x004C89D8 (deleting dtor)
nw::snd::internal::FsSoundArchiveBase::~FsSoundArchiveBase()
{
    Close();
    m_ResultLock.Finalize();
}

// 0x007405CC slot 0x14 | fefates:bytes
nw::io::FileStream* nw::snd::internal::FsSoundArchiveBase::OpenExtStream(void* pBuffer, int bufferSize, const char* pExtFilePath,
                                                                       void* pCacheBuffer, u32 cacheSize,
                                                                       FileStreamPriority priority) const
{
    if (!m_IsOpened || bufferSize < static_cast<int>(sizeof(FsFileStream) + sizeof(CachedFileStream))) {
        return nullptr;
    }
    FsFileStream* pFsStream =
        pBuffer != nullptr ? new (pBuffer) FsFileStream(pExtFilePath, 0, 0, m_IsTry, FsErrorCallbackHandler,
                                                        const_cast<FsSoundArchiveBase*>(this))
                           : nullptr;
    if (!pFsStream->m_IsOpened) {
        return nullptr;
    }
    bool result;
    switch (priority) {
    case FILE_STREAM_PRIORITY_HIGH:
        result = pFsStream->SetPriority(FileReader::PRIORITY_HIGH);
        break;
    case FILE_STREAM_PRIORITY_NORMAL:
        result = pFsStream->SetPriority(FileReader::PRIORITY_NORMAL);
        break;
    case FILE_STREAM_PRIORITY_LOW:
        result = pFsStream->SetPriority(FileReader::PRIORITY_LOW);
        break;
    default:
        return nullptr;
    }
    if (!result) {
        return nullptr;
    }
    void* pCachedStreamBuffer = static_cast<u8*>(pBuffer) + sizeof(FsFileStream);
    CachedFileStream* pStream = pCachedStreamBuffer != nullptr ? new (pCachedStreamBuffer) CachedFileStream(*pFsStream) : nullptr;
    if (pStream != nullptr && pCacheBuffer != nullptr && cacheSize != 0) {
        pStream->SetCacheBuffer(pCacheBuffer, cacheSize);
    }
    return pStream;
}

// 0x007406E8 slot 0x08
const void* nw::snd::internal::FsSoundArchiveBase::detail_GetFileAddress(u32 /*fileId*/) const
{
    return nullptr;
}

// 0x007406F0 slot 0x0C
size_t nw::snd::internal::FsSoundArchiveBase::detail_GetRequiredStreamBufferSize() const
{
    return sizeof(FsFileStream) + sizeof(CachedFileStream);
}

} // namespace internal
} // namespace snd
} // namespace nw
