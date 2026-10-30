#pragma once

// helpers of util_Api.cpp without reference symbols (names are ours)

#include "imgdb/util/util_Api.h"

namespace imgdb {
namespace util {

// deletes the file of an image (and the JPEG of an MPO on the SD card); empty directories afterwards on request
Result DeleteImageFile(StorageType storage, const ImageInfo& info, bool isLegacy, bool isDeleteEmptyDirectory);
Result DeleteDirectory(StorageType storage, const wchar_t* path);

inline bool IsFsDescription(nn::Result result, int low, int high)
{
    const u32 FS_MODULE = 17;
    return result.GetModule() == FS_MODULE && static_cast<int>(result.GetDescription()) >= low &&
           static_cast<int>(result.GetDescription()) <= high;
}

// a Result for a failed file system call that changes a file or directory
inline Result ConvertFsResult(nn::Result result)
{
    if (IsFsDescription(result, 260, 279))
    {
        return RESULT_FS_ACCESS_ERROR;
    }
    if (IsFsDescription(result, 130, 179))
    {
        return RESULT_FS_NOT_FOUND;
    }
    if (IsFsDescription(result, 280, 334))
    {
        return RESULT_FS_ARCHIVE_ERROR;
    }
    if (IsFsDescription(result, 200, 219))
    {
        return RESULT_FS_NOT_ENOUGH_SPACE;
    }
    return RESULT_FS_ERROR;
}

} // namespace util
} // namespace imgdb
