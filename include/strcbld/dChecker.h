#pragma once

#include "decomp.h"
#include "Other/dJmpBlock.h"
#include "Other/dJmpBlock_Visitor.h"

namespace strcbld {
// RTTI N7strcbld7CheckerE @ 0x008D3FA0
// vtable 0x0090BD9C (vptr 0x0090BDA4), offset_to_top 0, 1 entries
class Checker : public ::JmpBlock::Visitor
{
public:
    Checker(); // ctor address unknown
    virtual void vf_0x00(); // 0x0062F8F4 slot 0x00 | virtual slot, introduced by strcbld::Checker
};
} // namespace strcbld
