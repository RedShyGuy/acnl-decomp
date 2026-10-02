#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// vtable +0x153C4 in ModuleAmiiboCamera.cro, offset_to_top 0, 16 entries
class BsAmiiboPhotoMgr : public ::Base
{
public:
    class CstmCamera;
    BsAmiiboPhotoMgr(); // ctor address unknown
    virtual ~BsAmiiboPhotoMgr(); // ModuleAmiiboCamera.cro +0x003924 slot 0x00
    virtual void Initialize(); // ModuleAmiiboCamera.cro +0x002F2C slot 0x0C
    virtual void Finalize(); // ModuleAmiiboCamera.cro +0x003884 slot 0x18
    virtual void Calc(); // ModuleAmiiboCamera.cro +0x00307C slot 0x24
    virtual void Draw(); // ModuleAmiiboCamera.cro +0x002F24 slot 0x30
};
