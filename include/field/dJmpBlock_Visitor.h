#pragma once

#include "decomp.h"
#include "field/dJmpBlock.h"

// RTTI N8JmpBlock7VisitorE @ 0x008D4008
// visits the units of a block (JmpBlock::Accept; the slot name is ours)
class JmpBlock::Visitor
{
public:
    // true stops the visit
    virtual bool Visit(JmpBlock* block, s32 unitX, s32 unitZ) = 0;
};
