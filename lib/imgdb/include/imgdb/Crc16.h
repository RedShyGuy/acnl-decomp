#pragma once

#include "decomp.h"

namespace imgdb {
class Crc16
{
public:
    Crc16(); // TODO: default ctor added so derived stubs compile - may not exist
    Crc16(const void*, unsigned); // 0x005B03B0 | nintendogs:bytes [tier B]
};
} // namespace imgdb
