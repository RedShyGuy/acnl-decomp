#pragma once

#include "decomp.h"
#include "Ac/dAcStrc.h"
#include "Utl/dUtlBase.h"

// RTTI 16AcStrcCampingCar @ 0x008CC19C
// vtable 0x008F21BC (vptr 0x008F21C4), offset_to_top 0, 60 entries
class AcStrcCampingCar : public ::UtlBase<AcStrc>
{
public:
    AcStrcCampingCar(); // ctor candidate(s) 0x002AA0A0 (unverified)
    virtual void vf_0x00(); // 0x002AA228 slot 0x00 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x04(); // 0x002AA1A8 slot 0x04 | virtual slot, introduced by AcStrcCampingCar
    virtual void CanInitialize() const; // 0x006E5244 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void vf_0x0C(); // 0x00820810 slot 0x0C | virtual slot, introduced by AcStrcCampingCar
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00307C80 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x006E528C slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void vf_0x18(); // 0x008208AC slot 0x18 | virtual slot, introduced by AcStrcCampingCar
    virtual void HandleFinalizationResult(oml::framework::Result); // 0x0057C474 slot 0x1C | slot vf_0x1C of oml::framework::Process
    virtual void CanCalc() const; // 0x0057C478 slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void vf_0x24(); // 0x002A9DF4 slot 0x24 | virtual slot, introduced by AcStrcCampingCar
    virtual void HandleCalcResult(oml::framework::Result); // 0x0057C50C slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void CanDraw() const; // 0x0057C514 slot 0x2C | slot vf_0x2C of oml::framework::Process
    virtual void vf_0x30(); // 0x002A9DD0 slot 0x30 | virtual slot, introduced by AcStrcCampingCar
    virtual void ProcessDrawResult(oml::framework::Result); // 0x005220E4 slot 0x34 | slot vf_0x34 of oml::framework::Process
    virtual void OnNotify(); // 0x00521710 slot 0x38 | libgarden
    virtual void Unk0(); // 0x007537F0 slot 0x3C | slot vf_0x3C of Base
    virtual void vf_0x40(); // 0x0057C510 slot 0x40 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x44(); // 0x00770824 slot 0x44 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x48(); // 0x0071D80C slot 0x48 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x4C(); // 0x00770578 slot 0x4C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x50(); // 0x007B2C88 slot 0x50 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x54(); // 0x007707F8 slot 0x54 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x58(); // 0x007B2C88 slot 0x58 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x5C(); // 0x0077055C slot 0x5C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x60(); // 0x007707F0 slot 0x60 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x64(); // 0x006E5238 slot 0x64 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x68(); // 0x006E5050 slot 0x68 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x6C(); // 0x006E5068 slot 0x6C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x70(); // 0x00770554 slot 0x70 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x74(); // 0x00770564 slot 0x74 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x78(); // 0x00758AB4 slot 0x78 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x7C(); // 0x002A9A6C slot 0x7C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x80(); // 0x002A9A5C slot 0x80 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x84(); // 0x007B2C88 slot 0x84 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x88(); // 0x00758AAC slot 0x88 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x8C(); // 0x005BF3F0 slot 0x8C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x90(); // 0x00758AA4 slot 0x90 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x94(); // 0x005BF3F4 slot 0x94 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x98(); // 0x005BF3E4 slot 0x98 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0x9C(); // 0x007B2C88 slot 0x9C | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xA0(); // 0x007B2C88 slot 0xA0 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xA4(); // 0x005BF400 slot 0xA4 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xA8(); // 0x005BF3FC slot 0xA8 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xAC(); // 0x005BF3F8 slot 0xAC | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xB0(); // 0x005BF3E0 slot 0xB0 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xB4(); // 0x005BF3E8 slot 0xB4 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xB8(); // 0x005BF3DC slot 0xB8 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xBC(); // 0x005BF3D8 slot 0xBC | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xC0(); // 0x005BF3EC slot 0xC0 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xC4(); // 0x007B2C88 slot 0xC4 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xC8(); // 0x002A9CF4 slot 0xC8 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xCC(); // 0x002A9DB4 slot 0xCC | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xD0(); // 0x007B2C88 slot 0xD0 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xD4(); // 0x007B2C88 slot 0xD4 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xD8(); // 0x002A9698 slot 0xD8 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xDC(); // 0x002A93E8 slot 0xDC | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xE0(); // 0x002A8B70 slot 0xE0 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xE4(); // 0x002A99A4 slot 0xE4 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xE8(); // 0x002A98AC slot 0xE8 | virtual slot, introduced by AcStrcCampingCar
    virtual void vf_0xEC(); // 0x002A988C slot 0xEC | virtual slot, introduced by AcStrcCampingCar
};
