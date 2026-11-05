#pragma once

#include "decomp.h"
#include "oml/framework/dProcess.h"

// the base of the processes of the game (class and slot names after libgarden)
// RTTI 4Base @ 0x008CD240
// vtable 0x008F8540 (vptr 0x008F8548), offset_to_top 0, 16 entries
class Base : public ::oml::framework::Process
{
public:
    DECOMP_EMIT_COPY Base() {} // 0x0052A460 (name after the RTTI)
    virtual ~Base(); // 0x00316FA8 slot 0x00 | libgarden
    // 0x0052A478 slot 0x04 (deleting dtor)
    virtual bool CanInitialize() const; // 0x0052A438 slot 0x08 | libgarden
    virtual void HandleInitializationResult(oml::framework::Result result); // 0x00521610 slot 0x10 | libgarden
    virtual bool CanFinalize() const; // 0x0052A44C slot 0x14 | libgarden
    virtual void HandleFinalizationResult(oml::framework::Result result); // 0x00521618 slot 0x1C | libgarden
    virtual bool CanCalc() const; // 0x0052A3A0 slot 0x20 | libgarden
    virtual void HandleCalcResult(oml::framework::Result result); // 0x00521714 slot 0x28 | libgarden
    virtual bool CanDraw() const; // 0x0052A424 slot 0x2C | libgarden
    virtual bool Unk0(); // 0x00748D08 slot 0x3C | libgarden
};
ASSERT_SIZE(Base, 0x14);
