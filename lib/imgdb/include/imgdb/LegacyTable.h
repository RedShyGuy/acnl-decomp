#pragma once

#include "decomp.h"

namespace imgdb {
class LegacyTable
{
public:
    void SetExistingTableData(void*, unsigned); // 0x005A56A0 | nintendogs:bytes [tier B]
    void Validate() const; // 0x00754D68 | nintendogs:bytes [tier B]
};
} // namespace imgdb
