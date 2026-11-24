#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

struct SvMyDesign;

// RTTI 19BsMyDesignEditModel @ 0x008CC9AC
// vtable 0x008F4EEC (vptr 0x008F4EF4), offset_to_top 0, 22 entries
class BsMyDesignEditModel : public ::UtlBase<Base>
{
public:
    BsMyDesignEditModel(); // ctor candidate(s) 0x002F5B94 (unverified)
    virtual ~BsMyDesignEditModel(); // 0x002F5DF8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x002F5CF0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x002F56D4 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x002F5428 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x002F4DA4 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x002F4764 slot 0x44
    bool EndInitialize() override; // 0x002F3AC4 slot 0x48
    bool StartFinalize() override; // 0x002F5064 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x002F505C slot 0x50
    bool EndFinalize() override; // 0x002F4F18 slot 0x54

    // (game code that is not decompiled yet; called by the pro design button of the editor, names are ours)
    void Func2F4E90(); // 0x002F4E90
    void Func2F507C(); // 0x002F507C
    void Func2F50C8(u8 part); // 0x002F50C8
    // (called by the design editor, names are ours) the view of the model (0..3) / the design of the model /
    // the model leaves (mode 13)
    void SetView(u8 view); // 0x002F4724
    void Func2F56DC(SvMyDesign* design, u32 a); // 0x002F56DC
    void Func53761C(); // 0x0053761C

    static BsMyDesignEditModel* s_pInstance;

    u8 m_Unknown[0x19D4 - sizeof(UtlBase<Base>)];
    s16 m_Angle;  // 0x19D4 (the design editor turns the model)
    u8 m_Mode;    // 0x19D6 (SetView: 8, 9, 11, 12; Func53761C: 13)
};
ASSERT_OFFSET(BsMyDesignEditModel, m_Angle, 0x19D4);
