#pragma once

#include "decomp.h"

namespace camera {
// the camera manager of the game (the class name and all member names are ours; only the parts g3d uses: the
// instance and the render data)
class Mgr
{
public:
    // the instance (NULL: none)
    static Mgr* s_pInstance;

    u8 m_Unknown0x0[0x1A8];         // 0x000
    void* m_pRenderData;         // 0x1A8 &m_RenderData while g3d renders (BeginRender), else NULL
    u8 m_Unknown0x1AC[0x154];       // 0x1AC
    u32 m_RenderData;               // 0x300 (type unknown)
};
ASSERT_OFFSET(Mgr, m_pRenderData, 0x1A8);
ASSERT_OFFSET(Mgr, m_RenderData, 0x300);
} // namespace camera
