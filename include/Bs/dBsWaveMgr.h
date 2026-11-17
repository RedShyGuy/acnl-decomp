#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"
#include "field/dFieldAttribute.h"
#include "nn/math/math_Vector3.h"

// RTTI 9BsWaveMgr @ 0x008CD76C
// vtable 0x008FA578 (vptr 0x008FA580), offset_to_top 0, 22 entries
class BsWaveMgr : public ::UtlBase<Base>
{
public:
    BsWaveMgr(); // ctor candidate(s) 0x006E1BBC (unverified)
    virtual ~BsWaveMgr(); // 0x006E1BFC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E1BEC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x006E1B3C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006E1B34 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x006E1700 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x006E16F8 slot 0x44
    bool EndInitialize() override; // 0x006E16E4 slot 0x48
    bool StartFinalize() override; // 0x006E1B24 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x006E1B1C slot 0x50
    bool EndFinalize() override; // 0x006E1AF4 slot 0x54

    static BsWaveMgr* GetInstance(); // 0x006E1B0C (name is ours)
    // the attribute of a water part of a shore unit (s_ShoreAttributes) at an offset from the unit center: WATER where
    // the wave reaches, SAND_16 elsewhere (name is ours)
    FieldAttribute GetShoreAttribute(FieldAttribute attribute, const nn::math::VEC3& offset) const; // 0x006E1708

    static BsWaveMgr* s_pInstance;
};
