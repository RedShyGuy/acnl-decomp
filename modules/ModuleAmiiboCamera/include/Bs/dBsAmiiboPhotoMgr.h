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
    virtual oml::framework::Result Initialize(); // ModuleAmiiboCamera.cro +0x002F2C slot 0x0C
    virtual oml::framework::Result Finalize(); // ModuleAmiiboCamera.cro +0x003884 slot 0x18
    virtual oml::framework::Result Calc(); // ModuleAmiiboCamera.cro +0x00307C slot 0x24
    virtual oml::framework::Result Draw(); // ModuleAmiiboCamera.cro +0x002F24 slot 0x30
};
