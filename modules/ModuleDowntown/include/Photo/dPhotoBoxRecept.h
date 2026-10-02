#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x128A4 in ModuleDowntown.cro, offset_to_top 0, 72 entries
// vtable +0x129CC in ModuleDowntown.cro, offset_to_top -124, 14 entries
class PhotoBoxRecept : public ::ObjTalkRecept
{
public:
    PhotoBoxRecept(); // ctor address unknown
    virtual ~PhotoBoxRecept(); // ModuleDowntown.cro +0x002464 slot 0x00
};
