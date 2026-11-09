#pragma once

#include "decomp.h"
#include "script/dRenderBase.h"

namespace script {
// The renderer of the main text: tags for the font scale, colors and the cursor, and the positions and widths of the
// ruby bases (stored for the ruby text, RenderRuby). Member names after libgarden where it names them.
// RTTI N6script10RenderMainE @ 0x008D3738
// vtable 0x00909978 (vptr 0x00909980), offset_to_top 0, 4 entries
class RenderMain : public ::script::RenderBase
{
public:
    // a ruby base: its x and its width (name is ours)
    struct Store
    {
        f32 x;     // 0x0
        f32 width; // 0x4
    };

    RenderMain(u32 textLength, u32 storeNum, u32 tagNum); // 0x005D7338 | libgarden [tier A]
    virtual ~RenderMain();
    virtual Operation Process(u16 code, Context* ctx); // 0x005D704C slot 0x08
    virtual Operation CalcRect(nw::ut::Rect* pRect, u16 code, Context* ctx); // 0x005D71C8 slot 0x0C

    // the tags (names are ours)
    DECOMP_NOINLINE Operation ProcessTag(Context* ctx); // 0x005D6D6C (name is ours)
    DECOMP_NOINLINE Operation ProcessRgbaTag(Context* ctx, const Tag* tag); // 0x005D6FE0 (name is ours)

    // (inline) the stored ruby bases are reset (name is ours)
    void ResetStore()
    {
        m_Offset = 0;
        for (u32 i = 0; i < m_Size; i++) {
            m_pStore[i].x = 0.0f;
            m_pStore[i].width = 0.0f;
        }
    }

private:
    // (inline) the scale of the font during a line feed (names are ours)
    void BeginLineScale(u16 code, Context* ctx);
    void EndLineScale(Context* ctx);

public:
    u32 m_Offset;                 // 0x2C the next ruby base
    Store* m_pStore;              // 0x30
    u32 m_Size;                   // 0x34 the number of ruby bases
    u8 m_IsEndAtLineFeed;         // 0x38 a line feed ends the drawing (name is ours)
    nn::math::VEC2 m_SavedScale;  // 0x3C the scale of the writer during a line feed (none: -1; name is ours)
};
ASSERT_SIZE(RenderMain, 0x44);
} // namespace script
