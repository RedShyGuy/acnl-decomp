#pragma once

#include "decomp.h"

namespace imgdb {
class PictureTable
{
public:
    void ComputeTableSize(int, unsigned short); // 0x005A5944 | nintendogs:bytes [tier B]
    void SetInitialTableData(void*, unsigned, int, unsigned short); // 0x005A5958 | nintendogs:bytes [tier B]
    void SetExistingTableData(void*, unsigned); // 0x005A5A40 | nintendogs:bytes [tier B]
    void Validate() const; // 0x00754F30 | nintendogs:bytes [tier B]
};
} // namespace imgdb
