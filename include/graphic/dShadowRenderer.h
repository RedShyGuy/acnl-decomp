#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class Camera;
class RenderContext;
} // namespace gfx
} // namespace nw

namespace graphic {
// the process that draws the shadows (scene 2 of g3d) with a camera of its own (an oml::framework::Process; the
// class name and all names are ours; only the parts g3d uses)
class ShadowRenderer
{
public:
    // the render context for the shadow pass / back to normal
    void BeginDraw(nw::gfx::RenderContext* context); // 0x00264C54
    void EndDraw(nw::gfx::RenderContext* context); // 0x00264C24

    // the instance (NULL: none)
    static ShadowRenderer* s_pInstance;

    u8 m_Unknown0x0[0x4C];          // 0x00
    nw::gfx::Camera* m_pCamera;     // 0x4C
    u8 m_Unknown0x50[0x8];          // 0x50
    bool m_IsDrawing;               // 0x58 between BeginDraw and EndDraw
};
ASSERT_OFFSET(ShadowRenderer, m_IsDrawing, 0x58);
} // namespace graphic
