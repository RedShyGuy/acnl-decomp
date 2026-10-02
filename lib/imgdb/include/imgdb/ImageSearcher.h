#pragma once

#include "decomp.h"

namespace imgdb {
class ImageSearcher
{
public:
    void Initialize(imgdb::Allocator&, imgdb::StorageType); // 0x005A7354 | nintendogs:bytes [tier B]
    void SearchFile(); // 0x005A73D0 | nintendogs:bytes-fuzzy [tier B]
    void SearchDirectory(); // 0x005A7524 | nintendogs:bytes-fuzzy [tier B]
    ImageSearcher(); // 0x005A7674 | nintendogs:bytes [tier B]
    ~ImageSearcher(); // 0x005A76A4 | nintendogs:bytes [tier B]
    void IsNintendoDirectoryName(int*) const; // 0x007554C0 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace imgdb
