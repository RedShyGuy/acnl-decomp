#pragma once

#include "decomp.h"
#include "nw/math/math_Frustum.h"

namespace camera {
// the camera manager of the game (the class name and all member names are ours; only the parts g3d uses: the
// instance and the render data)
class Mgr
{
public:
    // the instance (NULL: none)
    static Mgr* s_pInstance;

    // the data behind m_pUnk0x1A4 (type unknown; only the field fgobj reads)
    struct Unk0x1A4
    {
        u8 m_Unknown0x0[0x1E];      // 0x00
        s16 m_RotateY;              // 0x1E the rotation about y (fgobj turns objects with ObjectBase::FLAG_FACE_CAMERA to it)
        u8 m_Unknown0x20[4];        // 0x20
        nn::math::VEC3_ m_Point;    // 0x24 a point of the camera (the areas of the field objects are around it; meaning unknown)
        nn::math::VEC3_ m_Eye;      // 0x30 the position of the camera (the room walls fade when they turn away from it)
    };

    u8 m_Unknown0x0[0x1A4];         // 0x000
    const Unk0x1A4* m_pUnk0x1A4;    // 0x1A4
    void* m_pRenderData;            // 0x1A8 &m_RenderData while g3d renders (BeginRender), else NULL
    nw::math::Frustum m_Frustum;    // 0x1AC the view volume (fgobj tests objects against it)
    u8 m_Unknown0x29C[0x64];        // 0x29C
    u32 m_RenderData;               // 0x300 (type unknown)
};
ASSERT_OFFSET(Mgr, m_pRenderData, 0x1A8);
ASSERT_OFFSET(Mgr, m_Frustum, 0x1AC);
ASSERT_OFFSET(Mgr, m_RenderData, 0x300);
} // namespace camera
