#pragma once

#include "decomp.h"

namespace imgdb {
class Directory
{
public:
    void Initialize(imgdb::Allocator&, imgdb::StorageType, const wchar_t*, int); // 0x005B09A0 | nintendogs:bytes [tier B]
    void Read(); // 0x005B0A0C | nintendogs:bytes [tier B]
    Directory(imgdb::Allocator&, imgdb::StorageType, const wchar_t*, int); // 0x005B0AC0 | nintendogs:bytes [tier B]
    Directory(); // 0x005B0B54 | nintendogs:bytes [tier B]
    ~Directory(); // 0x005B0B90 | nintendogs:bytes [tier B]
    void IsDirectory() const; // 0x00756258 | nintendogs:bytes [tier B]
    void IsFile() const; // 0x007562AC | nintendogs:bytes [tier B]
    void GetEntry() const; // 0x00756304 | nintendogs:bytes [tier B]
};
} // namespace imgdb
