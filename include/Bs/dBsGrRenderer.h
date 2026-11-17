#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 12BsGrRenderer @ 0x008CB520
// vtable 0x008EE044 (vptr 0x008EE04C), offset_to_top 0, 22 entries
class BsGrRenderer : public ::UtlBase<Base>
{
public:
    class Functor;
    BsGrRenderer(); // ctor candidate(s) 0x001F9C88 (unverified)
    virtual ~BsGrRenderer(); // 0x001F9D24 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F9CF8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x001F9C80 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x001F9C68 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x001F9B68 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x001F9B60 slot 0x44
    bool EndInitialize() override; // 0x001F9B10 slot 0x48
    bool StartFinalize() override; // 0x001F9C58 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x001F9C50 slot 0x50
    bool EndFinalize() override; // 0x001F9BC8 slot 0x54
};
