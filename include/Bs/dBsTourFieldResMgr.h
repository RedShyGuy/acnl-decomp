#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

class FgDataLoader;
namespace sead {
class Random;
} // namespace sead

// the acre kinds of the tour fields from a file of the BsTourFieldResMgr (names are ours): entries of 20 kinds (u16, 40
// bytes) at +0xD0, their number at +0xEC; Copy copies one of them (nothing for an index outside)
struct TourAcreList
{
    void Copy(u16* kinds, u32 index) const; // 0x002EEE30

    u8 unk_0x0[0xEC];
    u32 m_Num; // 0xEC
};

// RTTI 17BsTourFieldResMgr @ 0x008CC4D8
// vtable 0x008F3680 (vptr 0x008F3688), offset_to_top 0, 22 entries
class BsTourFieldResMgr : public ::UtlBase<Base>
{
public:
    BsTourFieldResMgr(); // ctor candidate(s) 0x002CB260 (unverified)

    // (game code, not decompiled yet; names are ours) the instance (0x009517FC) / the item layout of a file of the tour
    // (NULL if it is not loaded; found by the CRC of the name in its BaseResourceMgr<FgDataLoader, 18u>)
    static BsTourFieldResMgr* GetInstance(); // 0x002CB058
    FgDataLoader* GetFgData(const char* name); // 0x002CB088
    // the acre lists for the state of the tour data (TourData::m_State): 0 with and without the ocean side, 1, 2
    TourAcreList* GetAcreList0WithOceanSide(); // 0x002CB244
    TourAcreList* GetAcreList0(); // 0x002CB238
    TourAcreList* GetAcreList1(); // 0x002CB22C
    TourAcreList* GetAcreList2(); // 0x002CB220
    virtual ~BsTourFieldResMgr(); // 0x002CB3A8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002CB314 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x002CB258 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002CB250 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x002CACCC slot 0x40 | virtual slot, introduced by BsTourFieldResMgr
    virtual void vf_0x44(); // 0x002CA624 slot 0x44 | virtual slot, introduced by BsTourFieldResMgr
    virtual void vf_0x48(); // 0x002CA610 slot 0x48 | virtual slot, introduced by BsTourFieldResMgr
    virtual void vf_0x4C(); // 0x002CB070 slot 0x4C | virtual slot, introduced by BsTourFieldResMgr
    virtual void vf_0x50(); // 0x002CB068 slot 0x50 | virtual slot, introduced by BsTourFieldResMgr
    virtual void vf_0x54(); // 0x002CAEC0 slot 0x54 | virtual slot, introduced by BsTourFieldResMgr
};
