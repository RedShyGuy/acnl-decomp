#include "imgdb/FileWriter.h"
#include "imgdb/util/util_Api.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_FileOutputStream.h"

namespace imgdb {

namespace {

const u32 FS_MODULE = 17;

inline bool IsFsDescription(nn::Result result, int low, int high)
{
    return result.GetModule() == FS_MODULE && static_cast<int>(result.GetDescription()) >= low &&
           static_cast<int>(result.GetDescription()) <= high;
}

} // namespace

// 0x005A46EC | nintendogs:callgraph [tier C]
bool imgdb::FileWriter::Write(StorageType storage, const wchar_t* path, const void* data, u32 size)
{
    nn::fs::FileOutputStream file;
    m_FsResult = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateFile(path, size);
    if (IsFsDescription(m_FsResult, 180, 199))
    {
        // the file exists
        m_Result = util::DeleteFile(storage, path);
        if (m_Result != RESULT_SUCCESS)
        {
            return false;
        }
        m_FsResult = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateFile(path, size);
    }
    if (m_FsResult.IsSuccess())
    {
        m_FsResult = file.TryInitialize(path);
    }

    s32 writtenSize = 0;
    if (m_FsResult.IsSuccess())
    {
        m_FsResult = file.TryWrite(&writtenSize, data, size, true);
        file.Finalize();
        if (m_FsResult.IsSuccess() && size == static_cast<u32>(writtenSize))
        {
            m_Result = RESULT_SUCCESS;
            return true;
        }
    }

    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        util::ThrowFatalErrByNandOperation(m_FsResult);
    }
    if (IsFsDescription(m_FsResult, 260, 279))
    {
        m_Result = RESULT_FS_ACCESS_ERROR;
    }
    else if (IsFsDescription(m_FsResult, 130, 179))
    {
        m_Result = RESULT_FS_NOT_FOUND;
    }
    else if (IsFsDescription(m_FsResult, 280, 334))
    {
        m_Result = RESULT_FS_ARCHIVE_ERROR;
    }
    else if (IsFsDescription(m_FsResult, 200, 219))
    {
        m_Result = RESULT_FS_NOT_ENOUGH_SPACE;
    }
    else
    {
        m_Result = RESULT_FS_ERROR;
    }
    util::DeleteFile(storage, path);
    return false;
}

// 0x005A49C4 | nintendogs:bytes [tier B]
imgdb::FileWriter::FileWriter(Allocator& allocator) : m_pAllocator(&allocator), m_Result(RESULT_NOT_READY)
{
}

// 0x005A49EC
// 0x005A49E8 (deleting dtor)
imgdb::FileWriter::~FileWriter()
{
    // empty (in the original too)
}

} // namespace imgdb
