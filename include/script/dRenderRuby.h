#pragma once

#include "decomp.h"
#include "script/dRenderBase.h"

namespace script {
class RenderMain;

// The renderer of a ruby text: every ruby is centered over its base, whose position and width the main text stored
// (RenderMain::Store). libgarden gives this name to the renderer of both texts (RenderRubySet). Names are ours.
// RTTI N6script10RenderRubyE @ 0x008D3744
// vtable 0x00909990 (vptr 0x00909998), offset_to_top 0, 4 entries
class RenderRuby : public ::script::RenderBase
{
public:
    // (inline)
    RenderRuby(u32 length, u32 tagNum, RenderMain* pMain) : RenderBase(length, tagNum), m_pMain(pMain), m_Index(0) {}
    virtual ~RenderRuby();
    virtual Operation Process(u16 code, Context* ctx); // 0x005D7E68 slot 0x08
    virtual Operation CalcRect(nw::ut::Rect* pRect, u16 code, Context* ctx); // 0x005D7F24 slot 0x0C

    DECOMP_NOINLINE Operation ProcessTag(Context* ctx); // 0x005D7C14
    // the cursor is moved to center the ruby over the next ruby base
    DECOMP_NOINLINE Operation ProcessRubyPosition(Context* ctx, const Tag* tag); // 0x005D7D74

    RenderMain* m_pMain; // 0x2C
    u32 m_Index;         // 0x30 the next ruby base
};
ASSERT_SIZE(RenderRuby, 0x34);
} // namespace script
