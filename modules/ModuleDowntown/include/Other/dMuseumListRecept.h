#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x12A0C in ModuleDowntown.cro, offset_to_top 0, 72 entries
// vtable +0x12B34 in ModuleDowntown.cro, offset_to_top -124, 14 entries
class MuseumListRecept : public ::ObjTalkRecept
{
public:
    MuseumListRecept(); // ctor address unknown
    virtual ~MuseumListRecept(); // ModuleDowntown.cro +0x00265C slot 0x00
};
