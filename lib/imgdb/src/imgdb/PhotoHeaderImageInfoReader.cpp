#include "imgdb/PhotoHeaderImageInfoReader.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// (reads the file without the size check of PhotoHeaderFileReader)
// 0x005ABE1C | nintendogs:bytes [tier B]
imgdb::Result imgdb::PhotoHeaderImageInfoReader::ReadImage()
{
    wchar_t path[44];
    util::CreateImagePath(path, 44, static_cast<StorageType>(m_Storage), *m_pInfo, m_IsLegacy);
    FileReader::Read(static_cast<StorageType>(m_Storage), path, 0, 0);
    return GetResult();
}

// 0x005ABE70 | nintendogs:bytes [tier B]
imgdb::PhotoHeaderImageInfoReader::PhotoHeaderImageInfoReader(Allocator& allocator, StorageType storage,
                                                              const ImageInfo& info, bool isLegacy)
    : PhotoHeaderFileReader(allocator), m_Storage(storage), m_pInfo(&info), m_IsLegacy(isLegacy)
{
}

// 0x005ABF1C
// 0x005ABEC4 (deleting dtor)
imgdb::PhotoHeaderImageInfoReader::~PhotoHeaderImageInfoReader()
{
    // (only the base is destroyed)
}

} // namespace imgdb
