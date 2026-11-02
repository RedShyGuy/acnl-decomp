#pragma once

#include "decomp.h"
#include "nn/ulcd/CTR/ulcd_StereoCamera.h"
#include "nw/lyt/lyt_Drawer.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "ssys/ma/lyt/dLayoutList.h"

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
class VramHeap;

namespace lyt {
class Base2D;

// draws the layouts of both screens: the draw lists, the shaders of nw::lyt and the 3D camera (names after libgarden
// where it has them, the rest are ours)
// RTTI N4ssys2ma3lyt9LayoutMgrE @ 0x008D24C8
// vtable 0x00907358 (vptr 0x00907360), offset_to_top 0, 2 entries
class LayoutMgr
{
public:
    static const u32 TOP_SCREEN_WIDTH = 400;
    static const u32 BOTTOM_SCREEN_WIDTH = 320;
    static const u32 SCREEN_HEIGHT = 240;

    // (inline)
    LayoutMgr() {}
    virtual ~LayoutMgr(); // 0x0056AC34 slot 0x00 | libgarden
    // 0x0056AC04 (deleting dtor)

    // the instance on heap
    static void Create(sead::Heap* heap); // 0x0056A848
    DECOMP_EMIT_COPY static LayoutMgr* Get() { return s_pInstance; } // 0x00133A78 | libgarden [tier A]

    // the shaders (from the files of the resource paths), the drawer with its vertex buffer in vramHeap and the camera
    bool Initialize(VramHeap* vramHeap); // 0x0056A6A4
    // the viewport and the render state of a screen
    void DrawBegin(u32 width, u32 height); // 0x0056A9A4 | libgarden [tier A]
    void DrawBegin(bool isBottomScreen)
    {
        DrawBegin(isBottomScreen ? BOTTOM_SCREEN_WIDTH : TOP_SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    void Register(Base2D* b2d, bool bottomScreen); // 0x0056A954 | libgarden [tier A]
    // the upper screen: the left eye saves and draws, the right eye draws and empties the list
    void DrawTopLeft(); // 0x005684C0
    void DrawTopRight(); // 0x0056A978
    // the lower screen: saves, draws and empties the list
    void DrawBottom(); // 0x0056A660

    static LayoutMgr* s_pInstance;

    LayoutList m_DrawListA;                        // 0x04 the upper screen
    LayoutList m_DrawListB;                        // 0x10 the lower screen
    nw::lyt::GraphicsResource m_Resource;          // 0x1C
    nn::ulcd::CTR::StereoCamera m_StereoCamera;    // 0x300
    nw::lyt::Drawer m_Drawer;                      // 0x364
};
ASSERT_OFFSET(LayoutMgr, m_Resource, 0x1C);
ASSERT_OFFSET(LayoutMgr, m_StereoCamera, 0x300);
ASSERT_OFFSET(LayoutMgr, m_Drawer, 0x364);
ASSERT_SIZE(LayoutMgr, 0xA08);
} // namespace lyt
} // namespace ma
} // namespace ssys
