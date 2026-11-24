#pragma once

#include "decomp.h"
#include "Other/dMyDesignDragIconNode.h"

// An icon of a design in one of four sizes (class name from the RTTI, all other names are ours): the layout of the kind
// (s_LayoutNames) shows the design in P_texture_00; the 32x48 kind shows the upper three quarters of the texture.
// RTTI 23MyDesignProDragIconNode @ 0x008CCF20
// vtable 0x008F7250 (vptr 0x008F7258), offset_to_top 0, 29 entries
class MyDesignProDragIconNode : public ::MyDesignDragIconNode
{
public:
    // the layouts (s_LayoutNames)
    enum Kind
    {
        KIND_16X16 = 0,
        KIND_16X32 = 1,
        KIND_ICON = 2,
        KIND_32X48 = 3,
    };

    MyDesignProDragIconNode(); // 0x00335740
    // 0x00335798 (deleting dtor)
    ~MyDesignProDragIconNode() override; // 0x003357E0 slot 0x00

    // the layout of kind and the panes; the button gets index
    void Setup(s32 index, s32 kind, ssys::ma::lyt::ArcResAccReader* reader); // 0x00335548

    // 0x0095456C
    static const char* s_LayoutNames[4];

    u8 m_Kind;          // 0x290
    u8 _padding291[3];  // 0x291
};
ASSERT_SIZE(MyDesignProDragIconNode, 0x294);
