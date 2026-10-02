#pragma once

#include "decomp.h"
#include "Other/dBase.h"

// RTTI 16BsInsectFieldMgr @ 0x008CC1C8
// vtable 0x008F23B0 (vptr 0x008F23B8), offset_to_top 0, 16 entries
class BsInsectFieldMgr : public ::Base
{
public:
    BsInsectFieldMgr(); // ctor candidate(s) 0x002ADD48 (unverified)
    virtual ~BsInsectFieldMgr(); // 0x002ADEEC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002ADE60 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x002ACE68 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x002AD290 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x002AD144 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x002ACE60 slot 0x30 | slot vf_0x30 of oml::framework::Process
    void CreateInsect(item::InsectID, nn::math::Vector<float, 3u> const&, nn::math::Vector<unsigned short, 3u> const&); // 0x002AC924 | libgarden [tier A]
};
