#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 15BsPikoHanObjMgr @ 0x008CBFA8
// vtable 0x008F15B8 (vptr 0x008F15C0), offset_to_top 0, 16 entries
class BsPikoHanObjMgr : public ::Base
{
public:
    BsPikoHanObjMgr(); // ctor candidate(s) 0x0028E13C (unverified)
    virtual ~BsPikoHanObjMgr(); // 0x0028E1C4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0028E198 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0028DE04 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0028E078 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x0028E000 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x0028DDFC slot 0x30 | slot vf_0x30 of oml::framework::Process
};
