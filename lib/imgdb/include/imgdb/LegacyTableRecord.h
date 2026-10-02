#pragma once

#include "decomp.h"

namespace imgdb {
class LegacyTableRecord
{
public:
    void Initialize(); // 0x005AB208 | nintendogs:bytes [tier B]
    void SetIndexInfo(const imgdb::IndexInfo&); // 0x005AB224 | nintendogs:bytes [tier B]
    void GetIndexInfo() const; // 0x00755CA4 | nintendogs:bytes [tier B]
};
} // namespace imgdb
