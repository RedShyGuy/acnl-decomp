#include "imgdb/PhotoImageInfoReader.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

// 0x005AB3FC | nintendogs:bytes [tier B]
imgdb::Result imgdb::PhotoImageInfoReader::ReadImage()
{
    wchar_t path[44];
    util::CreateImagePath(path, 44, static_cast<StorageType>(m_Storage), *m_pInfo, m_IsLegacy);
    Read(static_cast<StorageType>(m_Storage), path, 0, 0);
    return GetResult();
}

// 0x005AB458 | nintendogs:bytes [tier B]
imgdb::PhotoImageInfoReader::PhotoImageInfoReader(Allocator& allocator, StorageType storage, const ImageInfo& info,
                                                  bool isLegacy)
    : PhotoFileReader(allocator), m_Storage(storage), m_pInfo(&info), m_IsLegacy(isLegacy)
{
}

// 0x005AB504
// 0x005AB4AC (deleting dtor)
imgdb::PhotoImageInfoReader::~PhotoImageInfoReader()
{
    // (only the base is destroyed)
}

} // namespace imgdb
