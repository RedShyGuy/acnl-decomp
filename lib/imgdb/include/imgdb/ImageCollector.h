#pragma once

#include "decomp.h"

namespace imgdb {
class ImageCollector
{
public:
    struct ProcessType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void ChangeProcess(imgdb::ImageCollector::ProcessType); // 0x005A7744 | nintendogs:bytes [tier B]
    void UpdateProcessCleanup(); // 0x005A77DC | nintendogs:bytes [tier B]
    void UpdateProcessCollectOfficial(); // 0x005A7B20 | nintendogs:bytes [tier B]
    void UpdateProcessCollectUnofficial(); // 0x005A7DA4 | nintendogs:bytes [tier B]
    void FindSameInfoInTable(const imgdb::ImageInfo&) const; // 0x00755568 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace imgdb
