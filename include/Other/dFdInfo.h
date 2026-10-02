#pragma once

#include "decomp.h"

class FdInfo
{
public:
    void ToUnit(long&, long&, nn::math::Vector<float, 3u> const&); // 0x00316FAC | libgarden [tier A]
    void GetCurrentField(); // 0x006A690C | libgarden [tier A]
    void GetHeight(nn::math::Vector<float, 3u> const&, FieldName); // 0x006C67B0 | libgarden [tier A]
    void GetHeightDefault(nn::math::Vector<float, 3u> const&, FieldName); // 0x006C6E28 | libgarden [tier A]
};
