#pragma once

#include "decomp.h"
#include "Ac/dAcSnowBall.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x12AC4 in ModuleWinter.cro, offset_to_top 0, 80 entries
// vtable +0x12C0C in ModuleWinter.cro, offset_to_top -124, 14 entries
class AcSnowBall::SpWrapObjTalkRecept : public ::ObjTalkRecept
{
public:
    SpWrapObjTalkRecept(); // ctor address unknown
    virtual ~SpWrapObjTalkRecept(); // ModuleWinter.cro +0x0076DC slot 0x00
};
