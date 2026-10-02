#pragma once

#include "decomp.h"

namespace imgdb {
class ImageInfo
{
public:
    void Initialize(); // 0x005A95D8 | nintendogs:bytes [tier B]
    void ResetBodyId(imgdb::BodyIdType); // 0x005B0C08 | nintendogs:bytes [tier B]
    void SetFileName(const imgdb::IndexInfo&); // 0x005B0C4C | nintendogs:bytes-fuzzy [tier B]
    void CopyStrToBuf(char*, int, const char*); // 0x005B0CA8 | nintendogs:bytes [tier B]
    void SetShootingType(imgdb::ShootingType); // 0x005B0F20 | nintendogs:bytes [tier B]
    void SetValidityFlag(bool); // 0x005B0F38 | nintendogs:bytes [tier B]
    void SetDirectoryName(const imgdb::IndexInfo&, bool); // 0x005B0F60 | nintendogs:bytes-fuzzy [tier B]
    void SetHandleTypeBit(imgdb::HandleTypeBit); // 0x005B0FB8 | nintendogs:bytes [tier B]
    void SetSaveProcessType(imgdb::SaveProcessType); // 0x005B0FD0 | nintendogs:bytes [tier B]
    void SetDistinctionTypeBit(imgdb::DistinctionTypeBit); // 0x005B0FEC | nintendogs:bytes [tier B]
    void SetExt(imgdb::ImageKind); // 0x005B1070 | nintendogs:bytes [tier B]
    void SetBodyId(imgdb::BodyIdType, unsigned); // 0x005B10A8 | nintendogs:bytes [tier B]
    void GetFileName(wchar_t*, int) const; // 0x0075634C | nintendogs:bytes [tier B]
    void ValidateExt() const; // 0x007563A0 | nintendogs:bytes [tier B]
    void GetImageKind() const; // 0x007563F4 | nintendogs:bytes-fuzzy [tier B]
    void GetIndexInfo() const; // 0x007564A0 | nintendogs:bytes [tier B]
    void IsValidBodyId(imgdb::BodyIdType) const; // 0x007564D8 | nintendogs:bytes [tier B]
    void GetDirectoryName(wchar_t*, int) const; // 0x00756520 | nintendogs:bytes [tier B]
    void ValidateWithoutPath() const; // 0x00756594 | nintendogs:bytes [tier B]
    void GetExt(wchar_t*, int) const; // 0x007565E8 | nintendogs:bytes [tier B]
    void Validate() const; // 0x0075663C | nintendogs:bytes [tier B]
};
} // namespace imgdb
