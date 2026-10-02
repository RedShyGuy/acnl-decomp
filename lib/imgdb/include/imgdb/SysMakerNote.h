#pragma once

#include "decomp.h"

namespace imgdb {
class SysMakerNote
{
public:
    void Initialize(); // 0x005A5AE4 | nintendogs:bytes [tier B]
    void SetShootingType(imgdb::ShootingType); // 0x005A5BAC | nintendogs:bytes [tier B]
    void SetDistinctionTypeBit(imgdb::DistinctionTypeBit); // 0x005A5C18 | nintendogs:bytes [tier B]
};
} // namespace imgdb
