#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x97BC0 in ModuleOutdoor.cro, offset_to_top 0, 72 entries
// vtable +0x97CE8 in ModuleOutdoor.cro, offset_to_top -124, 14 entries
class RentalHaniwaTalkRecept : public ::ObjTalkRecept
{
public:
    RentalHaniwaTalkRecept(); // ctor address unknown
    virtual ~RentalHaniwaTalkRecept(); // ModuleOutdoor.cro +0x05E8A8 slot 0x00
};
