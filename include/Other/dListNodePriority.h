#pragma once

#include "decomp.h"
#include "ssys/st/dListNodePriorityBase.h"

// a node of a priority list of the game (no members and no vtable of its own; the derived class compares)
// RTTI 16ListNodePriority @ 0x008CC344
class ListNodePriority : public ::ssys::st::ListNodePriorityBase
{
};
