#include "nw/snd/internal/snd_SoundArchiveFile_FileInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00143AF0 | nintendogs:bytes [tier A]
const char* nw::snd::internal::SoundArchiveFile::FileInfo::GetExternalFileInfo() const
{
    if (fileLocation.typeId == 0 || fileLocation.typeId != TYPE_EXTERNAL_FILE_INFO) {
        return nullptr;
    }
    return static_cast<const char*>(GetReferedPointer(this, fileLocation));
}

// 0x00143B1C | nintendogs:bytes [tier A]
nw::snd::internal::SoundArchiveFile::FileInfo::FileLocationType nw::snd::internal::SoundArchiveFile::FileInfo::GetFileLocationType() const
{
    switch (fileLocation.typeId) {
    case TYPE_INTERNAL_FILE_INFO:
        return FILE_LOCATION_TYPE_INTERNAL;
    case TYPE_EXTERNAL_FILE_INFO:
        return FILE_LOCATION_TYPE_EXTERNAL;
    default:
        return FILE_LOCATION_TYPE_NONE;
    }
}

// 0x00143B40 | nintendogs:bytes [tier A]
const nw::snd::internal::BinaryReferenceWithSize* nw::snd::internal::SoundArchiveFile::FileInfo::GetInternalFileInfo() const
{
    if (fileLocation.typeId == 0 || fileLocation.typeId != TYPE_INTERNAL_FILE_INFO) {
        return nullptr;
    }
    return static_cast<const BinaryReferenceWithSize*>(GetReferedPointer(this, fileLocation));
}

} // namespace internal
} // namespace snd
} // namespace nw
