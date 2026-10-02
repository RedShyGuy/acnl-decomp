#pragma once

#include "decomp.h"
#include "imgdb/ImageCollectionTableRecord.h"

namespace imgdb {
class ImageCollectionTable
{
public:
    void FindRecord(imgdb::ImageCollectionTableRecord::State, int) const; // 0x00755D14 | nintendogs:bytes [tier B]
};
} // namespace imgdb
