#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 8MenuBase @ 0x008CD508
// vtable 0x008F9C48 (vptr 0x008F9C50), offset_to_top 0, 25 entries
class MenuBase : public ::Base
{
public:
    class List;
    virtual ~MenuBase(); // 0x006AB508 slot 0x00 | libgarden
    // 0x006AB4E8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x006AB460 slot 0x08 | libgarden
    virtual void Initialize(); // 0x006AB414 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x006AB39C slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x006AB494 slot 0x14 | libgarden
    virtual void Finalize(); // 0x006AB458 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void HandleFinalizationResult(oml::framework::Result); // 0x006AB3A0 slot 0x1C | libgarden
    virtual void CanCalc() const; // 0x006AB3C4 slot 0x20 | libgarden
    virtual void Calc(); // 0x006AB448 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void HandleCalcResult(oml::framework::Result); // 0x006AB3DC slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void Draw(); // 0x006AB40C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void FUN_006ab3d8(); // 0x006AB3D8 slot 0x40 | libgarden
    virtual void OnClose(); // 0x006AB3E8 slot 0x44 | slot vf_0x44 of MenuBase
    virtual void FUN_006ab3f4(); // 0x006AB3F4 slot 0x48 | libgarden
    virtual void FUN_006ab450(); // 0x006AB450 slot 0x4C | libgarden
    virtual void FUN_006ab3e0(); // 0x006AB3E0 slot 0x50 | libgarden
    virtual void FUN_006ab440(); // 0x006AB440 slot 0x54 | libgarden
    virtual void FUN_006ab3fc(); // 0x006AB3FC slot 0x58 | libgarden
    virtual void FUN_006ab3ec(); // 0x006AB3EC slot 0x5C | libgarden
    virtual void FUN_00767b30(); // 0x00767B30 slot 0x60 | libgarden
    MenuBase(); // 0x006AB4A8 | libgarden [tier A]
};
