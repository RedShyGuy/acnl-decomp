#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 14BsIndoorDoorSE @ 0x008CBB80
// vtable 0x008EFC14 (vptr 0x008EFC1C), offset_to_top 0, 22 entries
class BsIndoorDoorSE : public ::UtlBase<Base>
{
public:
    BsIndoorDoorSE(); // ctor candidate(s) 0x002565D8 (unverified)
    virtual ~BsIndoorDoorSE(); // 0x00256678 slot 0x00 | mk7dlp:bytes
    // 0x00256654 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x00256464 slot 0x24 | slot vf_0x24 of oml::framework::Process
    bool StartInitialize() override; // 0x00256330 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00256328 slot 0x44
    bool EndInitialize() override; // 0x00256314 slot 0x48
    bool StartFinalize() override; // 0x00256394 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x0025638C slot 0x50
    bool EndFinalize() override; // 0x00256384 slot 0x54
};
