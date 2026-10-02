#pragma once

#include "decomp.h"

namespace imgdb {
class ImageCollectionTableRecord
{
public:
    struct State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void CreateImagePath(wchar_t*, int, imgdb::StorageType, bool) const; // 0x00755ED4 | nintendogs:bytes-fuzzy [tier B]
    void CreateImagePathWithChangeExt(wchar_t*, int, imgdb::StorageType, imgdb::ImageKind, bool) const; // 0x0075601C | nintendogs:bytes-fuzzy [tier B]
};
} // namespace imgdb
