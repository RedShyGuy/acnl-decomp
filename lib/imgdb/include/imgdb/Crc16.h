#pragma once

#include "imgdb/Crc16Context.h"

namespace imgdb {

// the CRC-16 of a block of data
class Crc16
{
public:
    Crc16(const void* data, u32 size);
    ~Crc16();

    u16 GetValue() const { return m_Context.GetCrc(); }

private:
    Crc16Context m_Context;  // +0x00
};
ASSERT_SIZE(Crc16, 0x8);

} // namespace imgdb
