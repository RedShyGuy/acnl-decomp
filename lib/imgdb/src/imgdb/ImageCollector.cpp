#include <cwchar>
#include <new>

#include "imgdb/ImageCollector.h"
#include "imgdb/ImageDbImageValidatorBasic.h"
#include "imgdb/ImageSearcher.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

const int PATH_SIZE = 44;
// the names with the terminator
const int NAME_SIZE = 9;
const int EXT_SIZE = 5;

} // namespace

// (ARMCC copies the tables from .rodata 0x008C2474 at startup)
// 0x0079DC00 (static initializer)

// 0x00AF631C
imgdb::ImageCollector::ProcessFunc imgdb::ImageCollector::s_EnterProcess[PROCESS_TYPE_NUM] = {
    &ImageCollector::EnterProcessIdle,
    &ImageCollector::EnterProcessCleanup,
    &ImageCollector::EnterProcessCollectOfficial,
    &ImageCollector::EnterProcessCollectUnofficial,
};
// 0x00AF633C
imgdb::ImageCollector::ProcessFunc imgdb::ImageCollector::s_ExitProcess[PROCESS_TYPE_NUM] = {
    &ImageCollector::ExitProcessIdle,
    &ImageCollector::ExitProcessCleanup,
    &ImageCollector::ExitProcessCollectOfficial,
    &ImageCollector::ExitProcessCollectUnofficial,
};
// 0x00AF635C
imgdb::ImageCollector::ProcessFunc imgdb::ImageCollector::s_UpdateProcess[PROCESS_TYPE_NUM] = {
    &ImageCollector::UpdateProcessIdle,
    &ImageCollector::UpdateProcessCleanup,
    &ImageCollector::UpdateProcessCollectOfficial,
    &ImageCollector::UpdateProcessCollectUnofficial,
};

// 0x005A7744 | nintendogs:bytes [tier B]
void imgdb::ImageCollector::ChangeProcess(ProcessType type)
{
    (this->*s_ExitProcess[m_Process])();
    m_Process = type;
    (this->*s_EnterProcess[type])();
}

// 0x005A77B8 (name is ours)
void imgdb::ImageCollector::ExitProcessIdle()
{
    // empty (in the original too)
}

// 0x005A77BC (name is ours)
void imgdb::ImageCollector::EnterProcessIdle()
{
    // empty (in the original too)
}

// 0x005A77C0 (name is ours)
void imgdb::ImageCollector::ExitProcessCleanup()
{
    m_Index = -1;
}

// 0x005A77CC (name is ours)
void imgdb::ImageCollector::UpdateProcessIdle()
{
    // empty (in the original too)
}

// 0x005A77D0 (name is ours)
void imgdb::ImageCollector::EnterProcessCleanup()
{
    m_Index = -1;
}

// (a known record whose file is gone or has changed gets a new state)
// 0x005A77DC | nintendogs:bytes [tier B]
void imgdb::ImageCollector::UpdateProcessCleanup()
{
    m_Index = m_Table.FindRecord(ImageCollectionTableRecord::STATE_KNOWN, m_Index);
    if (m_Index < 0)
    {
        ChangeProcess(PROCESS_TYPE_IDLE);
        return;
    }
    ImageCollectionTableRecord& record = m_Table.m_pRecords[m_Index];
    wchar_t path[PATH_SIZE];
    record.CreateImagePath(path, PATH_SIZE, m_Storage, false);
    bool isExist;
    s64 size = util::GetFileSize(m_Storage, path, &isExist);
    if (isExist)
    {
        if (record.m_FileSize == size)
        {
            return;
        }
        if (record.GetImageKind() == IMAGE_KIND_JPEG)
        {
            wchar_t mpPath[PATH_SIZE];
            record.CreateImagePathWithChangeExt(mpPath, PATH_SIZE, m_Storage, IMAGE_KIND_MP, false);
            bool isMpExist;
            s64 mpSize = util::GetFileSize(m_Storage, mpPath, &isMpExist);
            if (isMpExist)
            {
                record.m_State = ImageCollectionTableRecord::STATE_REPLACED_BY_MP;
                record.m_FileSize = mpSize;
                m_IsChanged = true;
                return;
            }
        }
        record.m_State = ImageCollectionTableRecord::STATE_CHANGED;
        record.m_FileSize = size;
    }
    else
    {
        if (record.GetImageKind() == IMAGE_KIND_MP)
        {
            wchar_t jpegPath[PATH_SIZE];
            record.CreateImagePathWithChangeExt(jpegPath, PATH_SIZE, m_Storage, IMAGE_KIND_JPEG, false);
            bool isJpegExist;
            s64 jpegSize = util::GetFileSize(m_Storage, jpegPath, &isJpegExist);
            if (isJpegExist)
            {
                record.m_State = ImageCollectionTableRecord::STATE_REPLACED_BY_JPEG;
                record.m_FileSize = jpegSize;
                m_IsChanged = true;
                return;
            }
        }
        record.m_State = ImageCollectionTableRecord::STATE_DELETED;
    }
    m_IsChanged = true;
}

// 0x005A7A00 (name is ours)
void imgdb::ImageCollector::ExitProcessCollectOfficial()
{
    m_IsDirectoryOpen = false;
    if (m_pSearcher != NULL)
    {
        Allocator* allocator = m_pAllocator;
        m_pSearcher->~ImageSearcher();
        allocator->Free(m_pSearcher);
    }
    m_pSearcher = NULL;
}

// 0x005A7A44 (name is ours)
void imgdb::ImageCollector::EnterProcessCollectOfficial()
{
    void* buffer = m_pAllocator->Allocate(sizeof(ImageSearcher), 4);
    m_pSearcher = buffer != NULL ? new (buffer) ImageSearcher() : NULL;
    m_pSearcher->Initialize(*m_pAllocator, m_Storage);
    m_IsDirectoryOpen = false;
}

// 0x005A7A90 (name is ours)
void imgdb::ImageCollector::ExitProcessCollectUnofficial()
{
    m_IsDirectoryOpen = false;
    if (m_pSearcher != NULL)
    {
        Allocator* allocator = m_pAllocator;
        m_pSearcher->~ImageSearcher();
        allocator->Free(m_pSearcher);
    }
    m_pSearcher = NULL;
}

// 0x005A7AD4 (name is ours)
void imgdb::ImageCollector::EnterProcessCollectUnofficial()
{
    void* buffer = m_pAllocator->Allocate(sizeof(ImageSearcher), 4);
    m_pSearcher = buffer != NULL ? new (buffer) ImageSearcher() : NULL;
    m_pSearcher->Initialize(*m_pAllocator, m_Storage);
    m_IsDirectoryOpen = false;
}

DECOMP_ALWAYS_INLINE void imgdb::ImageCollector::CollectImage(bool isOfficial)
{
    const ImageInfo& info = m_pSearcher->m_ImageInfo;
    ImageDbImageValidatorBasic validator;
    if (!validator.ValidateImage(m_Storage, info, false))
    {
        return;
    }
    ImageCollectionTableRecord* record;
    int index = FindSameInfoInTable(info);
    if (index >= 0)
    {
        // an MPO of a new JPEG
        record = &m_Table.m_pRecords[index];
        if (record->m_State != ImageCollectionTableRecord::STATE_NEW)
        {
            return;
        }
        wchar_t recordExt[EXT_SIZE];
        detail::CopyBufToStr(recordExt, record->m_Ext, sizeof(record->m_Ext));
        ImageInfo recordInfo;
        recordInfo.Initialize();
        recordInfo.SetExt(recordExt);
        if (recordInfo.GetImageKind() != IMAGE_KIND_JPEG)
        {
            return;
        }
        wchar_t ext[EXT_SIZE];
        info.GetExt(ext, EXT_SIZE);
        ImageCollectionTableRecord::CopyStrToBuf(record->m_Ext, sizeof(record->m_Ext), ext);
    }
    else
    {
        index = m_Table.FindRecord(ImageCollectionTableRecord::STATE_EMPTY, -1);
        if (index < 0)
        {
            return;
        }
        record = &m_Table.m_pRecords[index];
        wchar_t directoryName[NAME_SIZE];
        info.GetDirectoryName(directoryName, NAME_SIZE);
        ImageCollectionTableRecord::CopyStrToBuf(record->m_DirectoryName, sizeof(record->m_DirectoryName),
                                                 directoryName);
        wchar_t fileName[NAME_SIZE];
        info.GetFileName(fileName, NAME_SIZE);
        ImageCollectionTableRecord::CopyStrToBuf(record->m_FileName, sizeof(record->m_FileName), fileName);
        wchar_t ext[EXT_SIZE];
        info.GetExt(ext, EXT_SIZE);
        ImageCollectionTableRecord::CopyStrToBuf(record->m_Ext, sizeof(record->m_Ext), ext);
        record->m_State = ImageCollectionTableRecord::STATE_NEW;
        record->m_IsOfficial = isOfficial;
    }
    record->m_DateTime = validator.m_DateTime;
    record->m_FileSize = validator.m_FileSize;
    m_IsChanged = true;
}

// (the directories of Nintendo consoles)
// 0x005A7B20 | nintendogs:bytes [tier B]
void imgdb::ImageCollector::UpdateProcessCollectOfficial()
{
    if (m_IsDirectoryOpen)
    {
        ImageSearcher::SearchResult result = m_pSearcher->SearchFile();
        if (result == ImageSearcher::SEARCH_RESULT_END)
        {
            if (m_Table.FindRecord(ImageCollectionTableRecord::STATE_EMPTY, -1) >= 0)
            {
                m_IsDirectoryOpen = false;
                return;
            }
            ChangeProcess(PROCESS_TYPE_IDLE);
            return;
        }
        if (result == ImageSearcher::SEARCH_RESULT_FOUND)
        {
            CollectImage(true);
        }
        return;
    }
    ImageSearcher::SearchResult result = m_pSearcher->SearchDirectory();
    if (result == ImageSearcher::SEARCH_RESULT_END)
    {
        ChangeProcess(PROCESS_TYPE_IDLE);
        return;
    }
    if (result != ImageSearcher::SEARCH_RESULT_FOUND || !m_pSearcher->IsNintendoDirectoryName(NULL))
    {
        return;
    }
    if (m_Table.FindRecord(ImageCollectionTableRecord::STATE_EMPTY, -1) >= 0)
    {
        m_IsDirectoryOpen = true;
        return;
    }
    ChangeProcess(PROCESS_TYPE_IDLE);
}

// (the directories of other cameras)
// 0x005A7DA4 | nintendogs:bytes [tier B]
void imgdb::ImageCollector::UpdateProcessCollectUnofficial()
{
    if (m_IsDirectoryOpen)
    {
        ImageSearcher::SearchResult result = m_pSearcher->SearchFile();
        if (result == ImageSearcher::SEARCH_RESULT_END)
        {
            if (m_Table.FindRecord(ImageCollectionTableRecord::STATE_EMPTY, -1) >= 0)
            {
                m_IsDirectoryOpen = false;
                return;
            }
            ChangeProcess(PROCESS_TYPE_IDLE);
            return;
        }
        if (result == ImageSearcher::SEARCH_RESULT_FOUND)
        {
            CollectImage(false);
        }
        return;
    }
    ImageSearcher::SearchResult result = m_pSearcher->SearchDirectory();
    if (result == ImageSearcher::SEARCH_RESULT_END)
    {
        ChangeProcess(PROCESS_TYPE_IDLE);
        return;
    }
    if (result != ImageSearcher::SEARCH_RESULT_FOUND || m_pSearcher->IsNintendoDirectoryName(NULL))
    {
        return;
    }
    if (m_Table.FindRecord(ImageCollectionTableRecord::STATE_EMPTY, -1) >= 0)
    {
        m_IsDirectoryOpen = true;
        return;
    }
    ChangeProcess(PROCESS_TYPE_IDLE);
}

// 0x00755568 | nintendogs:bytes-fuzzy [tier B]
int imgdb::ImageCollector::FindSameInfoInTable(const ImageInfo& info) const
{
    wchar_t directoryName[NAME_SIZE];
    info.GetDirectoryName(directoryName, NAME_SIZE);
    wchar_t fileName[NAME_SIZE];
    info.GetFileName(fileName, NAME_SIZE);
    const u8 stateBits =
        (1 << ImageCollectionTableRecord::STATE_KNOWN) | (1 << ImageCollectionTableRecord::STATE_NEW);
    int index = -1;
    while (true)
    {
        index = m_Table.FindRecordByStateBits(stateBits, index);
        if (index < 0)
        {
            return -1;
        }
        const ImageCollectionTableRecord& record = m_Table.m_pRecords[index];
        wchar_t recordDirectoryName[NAME_SIZE];
        detail::CopyBufToStr(recordDirectoryName, record.m_DirectoryName, sizeof(record.m_DirectoryName));
        wchar_t recordFileName[NAME_SIZE];
        detail::CopyBufToStr(recordFileName, record.m_FileName, sizeof(record.m_FileName));
        if (std::wcscmp(directoryName, recordDirectoryName) == 0 && std::wcscmp(fileName, recordFileName) == 0)
        {
            return index;
        }
    }
}

} // namespace imgdb
