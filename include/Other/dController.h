#pragma once

#include "decomp.h"

class Controller
{
public:
    void IsHold(unsigned long); // 0x00304A14 | libgarden [tier A]
    void IsTrig(unsigned long); // 0x005223E8 | libgarden [tier A]
    void GetCirclePad(); // 0x00522520 | libgarden [tier A]
};
