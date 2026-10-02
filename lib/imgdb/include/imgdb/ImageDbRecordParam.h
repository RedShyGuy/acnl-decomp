#pragma once

#include "decomp.h"

namespace imgdb {
class ImageDbRecordParam
{
public:
    void Initialize(imgdb::ImageKind); // 0x005AB260 | nintendogs:bytes [tier B]
    void Initialize(); // 0x005AB2A8 | nintendogs:bytes [tier B]
    void ResetBodyId(imgdb::BodyIdType); // 0x005AB2D4 | nintendogs:bytes [tier B]
    void SetBodyId(imgdb::BodyIdType, unsigned); // 0x005AB2FC | nintendogs:bytes [tier B]
    void IsValidBodyId(imgdb::BodyIdType) const; // 0x00755CDC | nintendogs:bytes [tier B]
};
} // namespace imgdb
