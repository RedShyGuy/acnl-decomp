#include "imgdb/ImageFileWriter.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x005A9AB8 | nintendogs:bytes [tier B]
imgdb::Result imgdb::ImageFileWriter::WriteImage()
{
    wchar_t directory[30];
    util::CreateImageDirectoryPath(directory, 30, static_cast<StorageType>(m_Storage), *m_pInfo, m_IsLegacy);
    wchar_t path[44];
    util::CreateImagePath(path, 44, static_cast<StorageType>(m_Storage), *m_pInfo, m_IsLegacy);
    Result result = util::CreateDirectory(static_cast<StorageType>(m_Storage), directory, NULL);
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    FileWriter::Write(static_cast<StorageType>(m_Storage), path, m_pData, m_Size);
    return GetResult();
}

// 0x005A9B3C | nintendogs:bytes [tier B]
imgdb::ImageFileWriter::ImageFileWriter(Allocator& allocator, StorageType storage, const ImageInfo& info,
                                        bool isLegacy, const void* data, u32 size)
    : FileWriter(allocator), m_Storage(storage), m_pInfo(&info), m_IsLegacy(isLegacy), m_pData(data), m_Size(size)
{
}

// 0x005A9B94
// 0x005A9B90 (deleting dtor)
imgdb::ImageFileWriter::~ImageFileWriter()
{
    // empty (in the original too)
}

} // namespace imgdb
