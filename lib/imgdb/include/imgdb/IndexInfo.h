#pragma once

#include "decomp.h"

namespace imgdb {
class IndexInfo
{
public:
    void SetFileIndex(int); // 0x005B10F4 | nintendogs:bytes [tier B]
    void IsValidFileIndex(int); // 0x005B1110 | nintendogs:bytes [tier B]
    void SetDirectoryIndex(int); // 0x005B1128 | nintendogs:bytes [tier B]
    void UpdateNextIndexByOfficial(); // 0x005B114C | nintendogs:bytes [tier B]
    void IsValidFileIndex() const; // 0x00756718 | nintendogs:bytes [tier B]
    void IsValidDirectoryIndex() const; // 0x00756734 | nintendogs:bytes [tier B]
    void operator ==(const imgdb::IndexInfo&) const; // 0x00756748 | nintendogs:bytes [tier B]
    void operator !=(const imgdb::IndexInfo&) const; // 0x00756774 | nintendogs:bytes [tier B]
};
} // namespace imgdb
