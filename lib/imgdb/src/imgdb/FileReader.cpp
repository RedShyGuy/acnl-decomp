#include "imgdb/FileReader.h"
#include "imgdb/util/util_Api.h"
#include "nn/fs/fs_FileInputStream.h"

namespace imgdb {

namespace {

const u32 FS_MODULE = 17;

inline bool IsFsDescription(nn::Result result, int low, int high)
{
    return result.GetModule() == FS_MODULE && static_cast<int>(result.GetDescription()) >= low &&
           static_cast<int>(result.GetDescription()) <= high;
}

} // namespace

// 0x005A4280 | nintendogs:bytes [tier B]
bool imgdb::FileReader::Read(StorageType storage, const wchar_t* path)
{
    return Read(storage, path, 0, 0);
}

// size 0: the whole file (from offset)
// 0x005A42AC | nintendogs:callgraph [tier C]
bool imgdb::FileReader::Read(StorageType storage, const wchar_t* path, s64 offset, u32 size)
{
    nn::fs::FileInputStream file;
    ReleaseData();
    m_Result = RESULT_SUCCESS;

    m_FsResult = file.TryInitialize(path);
    if (m_FsResult.IsSuccess())
    {
        m_FsResult = file.TryGetSize(&m_FileSize);
        if (m_FsResult.IsSuccess())
        {
            s64 readSize = size != 0 ? size : m_FileSize;
            if (readSize > m_FileSize - offset)
            {
                readSize = m_FileSize - offset;
            }
            if (readSize > m_LimitSize)
            {
                if (m_IsTruncate)
                {
                    readSize = m_LimitSize;
                }
                else
                {
                    m_Result = RESULT_FILE_TOO_LARGE;
                }
            }

            if (m_Result == RESULT_SUCCESS &&
                (offset == 0 || (m_FsResult = file.TrySeek(offset, nn::fs::POSITION_BASE_BEGIN)).IsSuccess()))
            {
                ReleaseData();
                m_DataSize = readSize;
                m_pData = m_pBuffer != NULL ? m_pBuffer : m_pAllocator->Allocate(readSize, 32);
                s32 readNum;
                m_FsResult = file.TryRead(&readNum, m_pData, m_DataSize);
                if (m_FsResult.IsFailure() || m_DataSize != static_cast<u32>(readNum))
                {
                    ReleaseData();
                }
            }
        }
        file.Finalize();
    }

    if (m_FsResult.IsFailure())
    {
        if (storage == STORAGE_TYPE_CTR_NAND)
        {
            util::ThrowFatalErrByNandOperation(m_FsResult);
        }
        if (IsFsDescription(m_FsResult, 130, 179))
        {
            m_Result = RESULT_FS_NOT_FOUND;
        }
        else if (IsFsDescription(m_FsResult, 280, 334))
        {
            m_Result = RESULT_FS_ARCHIVE_ERROR;
        }
        else
        {
            m_Result = RESULT_FS_ERROR;
        }
    }
    return m_Result == RESULT_SUCCESS;
}

// 0x005A45B8 | nintendogs:bytes [tier B]
imgdb::FileReader::FileReader(void* buffer, u32 limitSize, bool isTruncate)
    : m_pAllocator(NULL),
      m_pBuffer(buffer),
      m_LimitSize(limitSize),
      m_IsTruncate(isTruncate),
      m_pData(buffer),
      m_DataSize(0),
      m_FileSize(0),
      m_Result(RESULT_NOT_READY)
{
}

// 0x005A45F8 | nintendogs:bytes [tier B]
imgdb::FileReader::FileReader(Allocator& allocator, u32 limitSize, bool isTruncate)
    : m_pAllocator(&allocator),
      m_pBuffer(NULL),
      m_LimitSize(limitSize),
      m_IsTruncate(isTruncate),
      m_pData(NULL),
      m_DataSize(0),
      m_FileSize(0),
      m_Result(RESULT_NOT_READY)
{
}

// 0x005A4698
// 0x005A4640 (deleting dtor)
imgdb::FileReader::~FileReader()
{
    ReleaseData();
}

// 0x00754CD8 | nintendogs:callgraph [tier C]
s64 imgdb::FileReader::GetFileSize() const
{
    return m_FileSize;
}

} // namespace imgdb
