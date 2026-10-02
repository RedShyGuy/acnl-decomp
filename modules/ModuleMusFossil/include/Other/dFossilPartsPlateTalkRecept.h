#pragma once

#include "decomp.h"
#include "Other/dObjTalkRecept.h"

// vtable +0x2514 in ModuleMusFossil.cro, offset_to_top 0, 72 entries
// vtable +0x263C in ModuleMusFossil.cro, offset_to_top -124, 14 entries
class FossilPartsPlateTalkRecept : public ::ObjTalkRecept
{
public:
    FossilPartsPlateTalkRecept(); // ctor address unknown
    virtual ~FossilPartsPlateTalkRecept(); // ModuleMusFossil.cro +0x000D28 slot 0x00
};
