#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "sead/hostio/seadNode.h"

// vtable +0x15308 in ModuleAmiiboCamera.cro, offset_to_top 0, 16 entries
// vtable +0x15350 in ModuleAmiiboCamera.cro, offset_to_top -20, 1 entries
class BsAmiiboCamera : public ::Base, public ::sead::hostio::Node
{
public:
    BsAmiiboCamera(); // ctor address unknown
    virtual ~BsAmiiboCamera(); // ModuleAmiiboCamera.cro +0x002A08 slot 0x00
    virtual void Initialize(); // ModuleAmiiboCamera.cro +0x002064 slot 0x0C
    virtual void Finalize(); // ModuleAmiiboCamera.cro +0x0028D0 slot 0x18
    virtual void Calc(); // ModuleAmiiboCamera.cro +0x002508 slot 0x24
    virtual void Draw(); // ModuleAmiiboCamera.cro +0x00205C slot 0x30
    virtual void Unk0(); // ModuleAmiiboCamera.cro +0x0125BC slot 0x3C
};
