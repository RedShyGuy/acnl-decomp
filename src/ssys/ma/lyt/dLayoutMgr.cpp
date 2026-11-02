#include "ssys/ma/lyt/dLayoutMgr.h"
#include <cwchar>
#include <cstring>
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gx/gx_Api.h"
#include "nw/lyt/lyt_Api.h"
#include "sead/seadNew.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/dFileData.h"
#include "ssys/ma/dVramHeap.h"
#include "ssys/ma/lyt/dLayout.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x00976C40
LayoutMgr* LayoutMgr::s_pInstance = NULL;

// the depth buffer of the screens (PICA DEPTHBUFFER_FORMAT 3 = 24 bits depth, 8 bits stencil); DrawBegin reads the
// format from memory (names are ours)
struct DepthBufferInfo
{
    u32 m_Unknown;  // 0x0
    u8 m_Format;    // 0x4
};
// 0x008AF2A0
extern const DepthBufferInfo s_DepthBufferInfo = {0, 3};

namespace {
const u32 REG_FACECULLING_CONFIG = 0x040;
const u32 REG_VIEWPORT_WIDTH = 0x041;
const u32 REG_DEPTHMAP_SCALE = 0x04D;
const u32 REG_DEPTHMAP_OFFSET = 0x04E;
const u32 REG_EARLYDEPTH_TEST1 = 0x062;
const u32 REG_VIEWPORT_XY = 0x068;
const u32 REG_DEPTHMAP_ENABLE = 0x06D;
const u32 REG_STENCIL_TEST = 0x105;
const u32 REG_DEPTH_COLOR_MASK = 0x107;
const u32 REG_COLORBUFFER_READING = 0x112;
const u32 REG_EARLYDEPTH_TEST2 = 0x118;

using nn::gr::CTR::detail::CommandHeader;

// the render state of the layouts: no culling, no stencil and depth test, color writes only
// 0x008AF3A8
const u32 s_RenderStateCommands[16] = {
    0,      CommandHeader(REG_FACECULLING_CONFIG),
    0,      CommandHeader(REG_STENCIL_TEST),
    0x1F00, CommandHeader(REG_DEPTH_COLOR_MASK),
    0,      CommandHeader(REG_EARLYDEPTH_TEST1, 0x1),
    0,      CommandHeader(REG_EARLYDEPTH_TEST2),
    // COLORBUFFER_READING/WRITING, DEPTHBUFFER_READING/WRITING
    0xF,    CommandHeader(REG_COLORBUFFER_READING, 0xF, 3, true), 0xF, 0, 0, 0,
};

// the size of the vertex buffer of the drawer in VRAM and the part of it that holds the vertices of RectDrawer
const u32 VERTEX_BUFFER_SIZE = 4096;
const u32 VERTEX_DATA_SIZE = 2496;
// the depth offset of the layouts (in units of the depth buffer)
const f32 POLYGON_OFFSET_UNIT = 0.0f;
} // namespace

// 0x005684C0 (name is ours)
void ssys::ma::lyt::LayoutMgr::DrawTopLeft()
{
    m_DrawListA.DrawSave();
    DrawBegin(TOP_SCREEN_WIDTH, SCREEN_HEIGHT);
    m_DrawListA.Draw(Layout::DRAW_MODE_TOP_LEFT);
}

// 0x0056A660 (name is ours)
void ssys::ma::lyt::LayoutMgr::DrawBottom()
{
    m_DrawListB.DrawSave();
    DrawBegin(BOTTOM_SCREEN_WIDTH, SCREEN_HEIGHT);
    m_DrawListB.Draw(Layout::DRAW_MODE_BOTTOM);
    m_DrawListB.Clear();
}

// 0x0056A6A4 (name is ours)
bool ssys::ma::lyt::LayoutMgr::Initialize(VramHeap* vramHeap)
{
    nw::lyt::Initialize(s_pAllocator, s_pDeviceAllocator);
    FileData file;
    const wchar_t* resPath;
    for (int i = 0; (resPath = nw::lyt::GraphicsResource::GetResourcePath(i)) != NULL; ++i) {
        // (image 0x008AF2A8)
        char path[256] = "Shader";
        char name[256];
        std::wcstombs(name, resPath, 255);
        const char* fileName = std::strrchr(name, '/');
        std::strncat(path, fileName, sizeof(path) - std::strlen(path));
        file.Load(path, s_pAllocator, 0);
        m_Resource.SetResource(i, file.m_pData, file.m_Size, true);
        file.Clear();
    }
    m_Resource.FinishSetup();
    void* vertexBuffer = reinterpret_cast<void*>(vramHeap->Alloc(VERTEX_BUFFER_SIZE, 16));
    m_Drawer.Initialize(m_Resource, vertexBuffer);
    nngxAddVramDmaCommand(nw::font::RectDrawer::GetVertexBufferData(), vertexBuffer, VERTEX_DATA_SIZE);
    m_StereoCamera.Initialize();
    m_StereoCamera.SetBaseFrustum(-0.04f, 0.04f, -0.024f, 0.024f, -500.0f, 3000.0f);
    return true;
}

// 0x0056A848 (name is ours)
void ssys::ma::lyt::LayoutMgr::Create(sead::Heap* heap)
{
    s_pInstance = new (heap, 4) LayoutMgr;
}

// 0x0056A954 | libgarden [tier A]
void ssys::ma::lyt::LayoutMgr::Register(Base2D* b2d, bool bottomScreen)
{
    switch (bottomScreen) {
    case false:
        m_DrawListA.Add(b2d);
        break;
    case true:
        m_DrawListB.Add(b2d);
        break;
    }
}

// 0x0056A978 (name is ours)
void ssys::ma::lyt::LayoutMgr::DrawTopRight()
{
    DrawBegin(TOP_SCREEN_WIDTH, SCREEN_HEIGHT);
    m_DrawListA.Draw(Layout::DRAW_MODE_TOP_RIGHT);
    m_DrawListA.Clear();
}

// 0x0056A9A4 | libgarden [tier A]
void ssys::ma::lyt::LayoutMgr::DrawBegin(u32 width, u32 height)
{
    const u16 w = width;
    const u16 h = height;
    // the PICA x axis runs along the height of the screen
    // (image 0x008AF3E8)
    u32 command[14] = {
        0, CommandHeader(REG_VIEWPORT_WIDTH, 0xF, 3, true), 0, 0, 0, 0,
        0, CommandHeader(REG_VIEWPORT_XY),
        1, CommandHeader(REG_DEPTHMAP_ENABLE),
        0, CommandHeader(REG_DEPTHMAP_SCALE),
        0, CommandHeader(REG_DEPTHMAP_OFFSET),
    };
    command[0] = nn::gr::CTR::Float32ToFloat24(h * 0.5f);
    command[2] = nn::gr::CTR::detail::Float32ToFloat31(2.0f / h) << 1;
    command[3] = nn::gr::CTR::Float32ToFloat24(w * 0.5f);
    command[4] = nn::gr::CTR::detail::Float32ToFloat31(2.0f / w) << 1;
    command[10] = nn::gr::CTR::Float32ToFloat24(-1.0f);
    const s32 depthBits = (s_DepthBufferInfo.m_Format != 0) ? 24 : 16;
    command[12] = nn::gr::CTR::Float32ToFloat24(POLYGON_OFFSET_UNIT / ((1 << depthBits) - 1));
    nngxAdd3DCommand(command, sizeof(command), true);
    nngxAdd3DCommand(s_RenderStateCommands, sizeof(s_RenderStateCommands), true);
}

// 0x0056AC34 slot 0x00 | libgarden
// 0x0056AC04 (deleting dtor)
ssys::ma::lyt::LayoutMgr::~LayoutMgr()
{
    // (only the members are destroyed)
}

} // namespace lyt
} // namespace ma
} // namespace ssys
