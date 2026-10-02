#pragma once

#include "decomp.h"

namespace sead {
class PrintFormatter
{
public:
    void proceedToFormatMark_(char*); // 0x00545508 | nintendogs:bytes [tier B]
    void flush(); // 0x00545A00 | nintendogs:bytes [tier B]
};
} // namespace sead
