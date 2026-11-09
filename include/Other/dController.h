#pragma once

#include "decomp.h"

class Controller
{
public:
    static bool IsHold(u32 buttons); // 0x00304A14 | libgarden [tier A]
    static bool IsTrig(u32 buttons); // 0x005223E8 | libgarden [tier A]
    void GetCirclePad(); // 0x00522520 | libgarden [tier A]
};
