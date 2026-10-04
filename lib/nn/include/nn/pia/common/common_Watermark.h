#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
// The minimum, maximum and last value of a measured quantity (WatermarkManager holds ten). The
// layout is the one of Update / SetName; the member names are ours.
class Watermark
{
public:
    static const int NAME_SIZE = 64;

    ~Watermark(); // 0x0042A0B0
    // takes the value if the watermark is enabled
    void Update(long long value); // 0x0042A048 | fefates:bytes [tier B]
    void SetName(const char* pName); // 0x00151168 | fefates:bytes [tier B]

    s64 m_Min;               // 0x00
    s64 m_Max;               // 0x08
    s64 m_Current;           // 0x10
    s64 m_Count;             // 0x18
    bool m_IsEnabled;        // 0x20
    char m_Name[NAME_SIZE];  // 0x21
};
ASSERT_SIZE(Watermark, 0x68);
} // namespace common
} // namespace pia
} // namespace nn
