#include "nw/lyt/lyt_Window.h"

#include "nn/gx/gx_Api.h"
#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_Drawer.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
namespace {
// How the texture of a frame lies on its quad for a TextureFlip: the texture coordinates of the
// corners of the texture (left top, right top, left bottom, right bottom) and which component of
// the quad (0: u, 1: v) the width and the height of the quad go to. Struct and names are ours.
struct TextureFlipInfo
{
    u8 coords[4][2];
    u8 idx[2];
};

// 0x0097F62C (name is ours)
TextureFlipInfo s_FlipInfos[Window::TEXTUREFLIP_MAX] = {
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {0, 1}}, // TEXTUREFLIP_NONE
    {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}, {0, 1}}, // TEXTUREFLIP_FLIPH
    {{{0, 1}, {1, 1}, {0, 0}, {1, 0}}, {0, 1}}, // TEXTUREFLIP_FLIPV
    {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}, {1, 0}}, // TEXTUREFLIP_ROTATE_90
    {{{1, 1}, {0, 1}, {1, 0}, {0, 0}}, {0, 1}}, // TEXTUREFLIP_ROTATE_180
    {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}, {1, 0}}, // TEXTUREFLIP_ROTATE_270
};

// the frame spec of the pane shader (uFrameSpec): the corner of the quad a frame covers and how
// its texture is flipped (the values are the bits the shader reads)
// 0x008B4D20 (name is ours)
const u32 s_FrameSpecs[8] = {0x03, 0x0B, 0x13, 0x1B, 0x13, 0x0B, 0x03, 0x1B};
// 0x008B4D40 (name is ours)
const u32 s_FlipSpecs[Window::TEXTUREFLIP_MAX] = {0x00, 0x20, 0x40, 0x24, 0x60, 0x44};

// the frames of 4 and 8 (the order of the resource)
enum
{
    FRAME_LT,
    FRAME_RT,
    FRAME_LB,
    FRAME_RB,
    FRAME_L,
    FRAME_R,
    FRAME_T,
    FRAME_B
};

inline f32* Coord(TexCoordQuad* pTexCoords, int corner)
{
    return &(*pTexCoords)[corner].x;
}

// The texture coordinates of a frame quad of polSize that starts at a corner of the texture (the
// texture repeats once per its size). Names are ours.

// 0x004B87F0 (name is ours)
// from the left top
void GetLTFrameTexCoord(TexCoordQuad* pTexCoords, const Size& polSize, const TexSize& texSize, u8 textureFlip)
{
    const TextureFlipInfo& flipInfo = s_FlipInfos[textureFlip];
    const int ix = flipInfo.idx[0];
    const int iy = flipInfo.idx[1];
    const f32 texSz[2] = {static_cast<f32>(texSize.width), static_cast<f32>(texSize.height)};

    Coord(pTexCoords, 2)[ix] = Coord(pTexCoords, 0)[ix] = flipInfo.coords[0][ix];
    Coord(pTexCoords, 1)[iy] = Coord(pTexCoords, 0)[iy] = flipInfo.coords[0][iy];
    Coord(pTexCoords, 3)[ix] = Coord(pTexCoords, 1)[ix] =
        polSize.width / ((flipInfo.coords[1][ix] - flipInfo.coords[0][ix]) * texSz[ix]) + flipInfo.coords[0][ix];
    Coord(pTexCoords, 3)[iy] = Coord(pTexCoords, 2)[iy] =
        polSize.height / ((flipInfo.coords[2][iy] - flipInfo.coords[0][iy]) * texSz[iy]) + flipInfo.coords[0][iy];
}

// 0x004B89EC (name is ours)
// from the right top
void GetRTFrameTexCoord(TexCoordQuad* pTexCoords, const Size& polSize, const TexSize& texSize, u8 textureFlip)
{
    const TextureFlipInfo& flipInfo = s_FlipInfos[textureFlip];
    const int ix = flipInfo.idx[0];
    const int iy = flipInfo.idx[1];
    const f32 texSz[2] = {static_cast<f32>(texSize.width), static_cast<f32>(texSize.height)};

    Coord(pTexCoords, 1)[ix] = Coord(pTexCoords, 3)[ix] = flipInfo.coords[1][ix];
    Coord(pTexCoords, 1)[iy] = Coord(pTexCoords, 0)[iy] = flipInfo.coords[1][iy];
    Coord(pTexCoords, 2)[ix] = Coord(pTexCoords, 0)[ix] =
        polSize.width / ((flipInfo.coords[0][ix] - flipInfo.coords[1][ix]) * texSz[ix]) + flipInfo.coords[1][ix];
    Coord(pTexCoords, 2)[iy] = Coord(pTexCoords, 3)[iy] =
        polSize.height / ((flipInfo.coords[3][iy] - flipInfo.coords[1][iy]) * texSz[iy]) + flipInfo.coords[1][iy];
}

// 0x004B86F0 (name is ours)
// from the left bottom
void GetLBFrameTexCoord(TexCoordQuad* pTexCoords, const Size& polSize, const TexSize& texSize, u8 textureFlip)
{
    const TextureFlipInfo& flipInfo = s_FlipInfos[textureFlip];
    const int ix = flipInfo.idx[0];
    const int iy = flipInfo.idx[1];
    const f32 texSz[2] = {static_cast<f32>(texSize.width), static_cast<f32>(texSize.height)};

    Coord(pTexCoords, 2)[ix] = Coord(pTexCoords, 0)[ix] = flipInfo.coords[2][ix];
    Coord(pTexCoords, 3)[iy] = Coord(pTexCoords, 2)[iy] = flipInfo.coords[2][iy];
    Coord(pTexCoords, 1)[ix] = Coord(pTexCoords, 3)[ix] =
        polSize.width / ((flipInfo.coords[3][ix] - flipInfo.coords[2][ix]) * texSz[ix]) + flipInfo.coords[2][ix];
    Coord(pTexCoords, 1)[iy] = Coord(pTexCoords, 0)[iy] =
        polSize.height / ((flipInfo.coords[0][iy] - flipInfo.coords[2][iy]) * texSz[iy]) + flipInfo.coords[2][iy];
}

// 0x004B88EC (name is ours)
// from the right bottom
void GetRBFrameTexCoord(TexCoordQuad* pTexCoords, const Size& polSize, const TexSize& texSize, u8 textureFlip)
{
    const TextureFlipInfo& flipInfo = s_FlipInfos[textureFlip];
    const int ix = flipInfo.idx[0];
    const int iy = flipInfo.idx[1];
    const f32 texSz[2] = {static_cast<f32>(texSize.width), static_cast<f32>(texSize.height)};

    Coord(pTexCoords, 3)[ix] = Coord(pTexCoords, 1)[ix] = flipInfo.coords[3][ix];
    Coord(pTexCoords, 3)[iy] = Coord(pTexCoords, 2)[iy] = flipInfo.coords[3][iy];
    Coord(pTexCoords, 0)[ix] = Coord(pTexCoords, 2)[ix] =
        polSize.width / ((flipInfo.coords[2][ix] - flipInfo.coords[3][ix]) * texSz[ix]) + flipInfo.coords[3][ix];
    Coord(pTexCoords, 0)[iy] = Coord(pTexCoords, 1)[iy] =
        polSize.height / ((flipInfo.coords[1][iy] - flipInfo.coords[3][iy]) * texSz[iy]) + flipInfo.coords[3][iy];
}

typedef void (*GetFrameTexCoordFunc)(TexCoordQuad* pTexCoords, const Size& polSize, const TexSize& texSize, u8 textureFlip);

// (inline) the commands of a frame quad with the texture of the material (its first one)
inline u32* MakeFrameQuad(Drawer& drawer, u32* pCommand, const Material* pMaterial, u8 textureFlip, const Size& polSize,
                          const nn::math::VEC2& pos, GetFrameTexCoordFunc getTexCoord)
{
    TexCoordQuad texCoords;
    nn::math::VEC4 coords[Window::TEXCOORD_VEC_MAX];
    const TexSize texSize = pMaterial->GetTexMapAry()[0].m_Size;
    getTexCoord(&texCoords, polSize, texSize, textureFlip);
    pCommand = drawer.SetUpTextureCoords(pCommand, coords, drawer.CalcTextureCoords(pMaterial, &texCoords, coords));
    drawer.SetUpQuad(polSize, pos);
    return drawer.AddQuad(pCommand);
}

// (inline) the same with the texture combiner and textures of the material first (if it has a
// texture)
inline u32* MakeFrame(Drawer& drawer, u32* pCommand, const Window::Frame& frame, const Size& polSize, const nn::math::VEC2& pos,
                      GetFrameTexCoordFunc getTexCoord)
{
    if (frame.pMaterial->GetTexMapNum() == 0) {
        return pCommand;
    }
    pCommand = drawer.SetUpTexEnv(pCommand, frame.pMaterial);
    pCommand = drawer.SetUpTextures(pCommand, frame.pMaterial, true);
    return MakeFrameQuad(drawer, pCommand, frame.pMaterial, frame.textureFlip, polSize, pos, getTexCoord);
}

// (inline) the uniform of the frame spec
inline void SetFrameSpec(const GraphicsResource& graphicsResource, u32 frameSpec, u32 flipSpec)
{
    glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_FRAMESPEC), static_cast<f32>(frameSpec | flipSpec), 0.0f,
                0.0f, 0.0f);
}

// (inline) the size of the first texture of the material (0: none)
inline bool GetTextureSize(TexSize* pSize, const Material* pMaterial)
{
    if (pMaterial == nullptr || pMaterial->GetTexMapNum() == 0) {
        return false;
    }
    *pSize = pMaterial->GetTexMapAry()[0].m_Size;
    return true;
}
} // namespace

// 0x0097F628 (name is ours)
const PaneTypeInfo nw::lyt::Window::s_TypeInfo(&Pane::s_TypeInfo);

// sets s_TypeInfo
// 0x00790BDC (static initializer)

// 0x004BB690 | nintendogs:callseq [tier A]
nw::lyt::Window::Window(const res::Window* pBlock, const ResBlockSet& resBlockSet) : Pane(pBlock)
{
    const res::WindowContent* pResContent = reinterpret_cast<const res::WindowContent*>(reinterpret_cast<uptr>(pBlock) + pBlock->contentOffset);
    u8 texCoordNum = pResContent->texCoordNum;
    if (texCoordNum > Material::TEXMAP_MAX) {
        texCoordNum = Material::TEXMAP_MAX;
    }
    if (texCoordNum != 0) {
        m_ContentTexCoordAry.Reserve(texCoordNum);
    }
    m_IsContentTexCoordCached = false;
    m_ContentInflation = pBlock->inflation;

    const res::MaterialList* pMaterialList = resBlockSet.pMaterialList;
    const u32* matOffsets = pMaterialList->materialOffsets;
    for (int i = 0; i < 4; ++i) {
        m_ContentVtxColors[i] = pResContent->vtxCols[i];
    }
    if (texCoordNum != 0 && m_ContentTexCoordAry.m_Cap != 0) {
        m_ContentTexCoordAry.Copy(pResContent + 1, texCoordNum);
    }
    m_pContentMaterial = Layout::NewObj<Material>(
        reinterpret_cast<const res::Material*>(reinterpret_cast<uptr>(pMaterialList) + matOffsets[pResContent->materialIdx]), resBlockSet);

    m_FrameNum = 0;
    m_pFrames = nullptr;
    if (pBlock->frameNum > 0) {
        m_pFrames = Layout::NewArray<Frame>(pBlock->frameNum);
        if (m_pFrames != nullptr) {
            m_FrameNum = pBlock->frameNum;
            const u32* frameOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(pBlock) + pBlock->frameOffsetTableOffset);
            for (int i = 0; i < m_FrameNum; ++i) {
                const res::WindowFrame* pResFrame = reinterpret_cast<const res::WindowFrame*>(reinterpret_cast<uptr>(pBlock) + frameOffsets[i]);
                m_pFrames[i].textureFlip = pResFrame->textureFlip;
                m_pFrames[i].pMaterial = Layout::NewObj<Material>(
                    reinterpret_cast<const res::Material*>(reinterpret_cast<uptr>(pMaterialList) + matOffsets[pResFrame->materialIdx]), resBlockSet);
            }
        }
    }
}

// 0x004BB9AC slot 0x00 (name is ours)
// 0x004BB8D8 (deleting dtor)
nw::lyt::Window::~Window()
{
    if (m_pFrames != nullptr) {
        const u8 frameNum = m_FrameNum;
        for (int i = 0; i < frameNum; ++i) {
            Material* pMaterial = m_pFrames[i].pMaterial;
            if (pMaterial != nullptr && !pMaterial->IsUserAllocated()) {
                Layout::DeleteObj(pMaterial);
            }
            m_pFrames[i].pMaterial = nullptr;
        }
        Layout::FreeMemory(m_pFrames);
    }
    if (m_pContentMaterial != nullptr && !m_pContentMaterial->IsUserAllocated()) {
        Layout::DeleteObj(m_pContentMaterial);
        m_pContentMaterial = nullptr;
    }
    m_ContentTexCoordAry.Free();
}

// 0x0073D4F8 slot 0x08 (name is ours)
const PaneTypeInfo* nw::lyt::Window::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073D394 slot 0x0C
ut::Color8 nw::lyt::Window::GetVtxColor(u32 idx) const
{
    return m_ContentVtxColors[idx];
}

// 0x004BB22C slot 0x10
void nw::lyt::Window::SetVtxColor(u32 idx, ut::Color8 value)
{
    m_ContentVtxColors[idx] = value;
}

// 0x0073D504 slot 0x1C | mk7dlp:bytes
u8 nw::lyt::Window::GetVtxColorElement(u32 idx) const
{
    return reinterpret_cast<const u8*>(&m_ContentVtxColors[idx / 4])[idx % 4];
}

// 0x004BB314 slot 0x20 | nintendogs:bytes
void nw::lyt::Window::SetVtxColorElement(u32 idx, u8 value)
{
    reinterpret_cast<u8*>(&m_ContentVtxColors[idx / 4])[idx % 4] = value;
}

// 0x0073D4E8 slot 0x24
u8 nw::lyt::Window::GetMaterialNum() const
{
    return m_FrameNum + 1;
}

// 0x0073D35C slot 0x28
Material* nw::lyt::Window::GetMaterial(u32 idx)
{
    if (idx == 0) {
        return m_pContentMaterial;
    }
    const u8 frameIdx = idx - 1;
    if (frameIdx >= m_FrameNum) {
        return nullptr;
    }
    return m_pFrames[frameIdx].pMaterial;
}

// 0x004BB23C slot 0x30
Material* nw::lyt::Window::FindMaterialByName(const char* findName, bool bRecursive)
{
    if (m_pContentMaterial != nullptr && internal::EqualsMaterialName(m_pContentMaterial->GetName(), findName)) {
        return m_pContentMaterial;
    }
    for (int i = 0; i < m_FrameNum; ++i) {
        if (internal::EqualsMaterialName(m_pFrames[i].pMaterial->GetName(), findName)) {
            return m_pFrames[i].pMaterial;
        }
    }
    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            Material* pMaterial = FromLink(it.GetNode())->FindMaterialByName(findName, bRecursive);
            if (pMaterial != nullptr) {
                return pMaterial;
            }
        }
    }
    return nullptr;
}

// 0x004BB32C slot 0x64 (name is ours)
void nw::lyt::Window::DrawSelf(DrawInfo& drawInfo)
{
    LoadMtx(drawInfo);
    const WindowFrameSize frameSize = GetFrameSize(m_FrameNum, m_pFrames);
    const nn::math::VEC2 basePt = GetVtxPos();
    DrawContent(drawInfo, basePt, frameSize, m_GlbAlpha);

    switch (m_FrameNum) {
    case 1:
        DrawFrame(drawInfo, basePt, *m_pFrames, frameSize, m_GlbAlpha);
        break;
    case 4:
        DrawFrame4(drawInfo, basePt, m_pFrames, frameSize, m_GlbAlpha);
        break;
    case 8:
        DrawFrame8(drawInfo, basePt, m_pFrames, frameSize, m_GlbAlpha);
        break;
    }
}

// 0x0073D51C slot 0x68
u32* nw::lyt::Window::MakeUniformDataSelf(u32* pCommand, DrawInfo& /*drawInfo*/, Drawer& drawer)
{
    const WindowFrameSize frameSize = GetFrameSize(m_FrameNum, m_pFrames);
    const nn::math::VEC2 basePt = GetVtxPos();

    // the content
    pCommand = drawer.SetUpTexEnv(pCommand, m_pContentMaterial);
    pCommand = drawer.SetUpTextures(pCommand, m_pContentMaterial, true);
    if (!m_IsContentTexCoordCached || !m_pContentMaterial->IsCacheValid()) {
        m_ContentTexCoordNum = drawer.CalcTextureCoords(m_pContentMaterial, m_ContentTexCoordAry.GetArray(), m_ContentTexCoords);
        m_IsContentTexCoordCached = true;
        m_pContentMaterial->SetCacheValid();
    }
    pCommand = drawer.SetUpTextureCoords(pCommand, m_ContentTexCoords, m_ContentTexCoordNum);
    drawer.SetUpMtx(m_GlbMtx);
    {
        const Size contentSize(m_Size.width - frameSize.l + m_ContentInflation.l - frameSize.r + m_ContentInflation.r,
                               m_Size.height - frameSize.t + m_ContentInflation.t - frameSize.b + m_ContentInflation.b);
        const nn::math::VEC2 contentPos(basePt.x + frameSize.l - m_ContentInflation.l, basePt.y - frameSize.t + m_ContentInflation.t);
        drawer.SetUpQuad(contentSize, contentPos);
    }
    drawer.SetUpVtxColors(m_ContentVtxColors, m_GlbAlpha);
    pCommand = drawer.AddQuad(pCommand);

    // the frames
    const ut::Color8 frameColors[4];
    drawer.SetUpVtxColors(frameColors, m_GlbAlpha);

    const f32 x = basePt.x;
    const f32 y = basePt.y;
    const f32 innerX = x + frameSize.l;
    const f32 innerY = y - frameSize.t;
    Size polSize;
    nn::math::VEC2 pos;

    switch (m_FrameNum) {
    case 1: {
        const Frame& frame = m_pFrames[0];
        const Material* pMaterial = frame.pMaterial;
        if (pMaterial->GetTexMapNum() == 0) {
            return pCommand;
        }
        pCommand = drawer.SetUpTexEnv(pCommand, pMaterial);
        pCommand = drawer.SetUpTextures(pCommand, pMaterial, true);

        pos = nn::math::VEC2(x, y);
        polSize = Size(m_Size.width - frameSize.r, frameSize.t);
        pCommand = MakeFrameQuad(drawer, pCommand, pMaterial, TEXTUREFLIP_NONE, polSize, pos, GetLTFrameTexCoord);

        pos = nn::math::VEC2(x + m_Size.width - frameSize.r, y);
        polSize = Size(frameSize.r, m_Size.height - frameSize.b);
        pCommand = MakeFrameQuad(drawer, pCommand, pMaterial, TEXTUREFLIP_FLIPH, polSize, pos, GetRTFrameTexCoord);

        pos = nn::math::VEC2(innerX, y - m_Size.height + frameSize.b);
        polSize = Size(m_Size.width - frameSize.l, frameSize.b);
        pCommand = MakeFrameQuad(drawer, pCommand, pMaterial, TEXTUREFLIP_ROTATE_180, polSize, pos, GetRBFrameTexCoord);

        pos = nn::math::VEC2(x, innerY);
        polSize = Size(frameSize.l, m_Size.height - frameSize.t);
        pCommand = MakeFrameQuad(drawer, pCommand, pMaterial, TEXTUREFLIP_FLIPV, polSize, pos, GetLBFrameTexCoord);
        break;
    }
    case 4:
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_LT], Size(m_Size.width - frameSize.r, frameSize.t), nn::math::VEC2(x, y),
                             GetLTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_RT], Size(frameSize.r, m_Size.height - frameSize.b),
                             nn::math::VEC2(x + m_Size.width - frameSize.r, y), GetRTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_RB], Size(m_Size.width - frameSize.l, frameSize.b),
                             nn::math::VEC2(innerX, y - m_Size.height + frameSize.b), GetRBFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_LB], Size(frameSize.l, m_Size.height - frameSize.t), nn::math::VEC2(x, innerY),
                             GetLBFrameTexCoord);
        break;
    case 8:
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_LT], Size(frameSize.l, frameSize.t), nn::math::VEC2(x, y), GetLTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_T], Size(m_Size.width - frameSize.l - frameSize.r, frameSize.t),
                             nn::math::VEC2(innerX, y), GetLTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_RT], Size(frameSize.r, frameSize.t),
                             nn::math::VEC2(x + m_Size.width - frameSize.r, y), GetRTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_R], Size(frameSize.r, m_Size.height - frameSize.t - frameSize.b),
                             nn::math::VEC2(x + m_Size.width - frameSize.r, innerY), GetRTFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_RB], Size(frameSize.r, frameSize.b),
                             nn::math::VEC2(x + m_Size.width - frameSize.r, y - m_Size.height + frameSize.b), GetRBFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_B], Size(m_Size.width - frameSize.l - frameSize.r, frameSize.b),
                             nn::math::VEC2(innerX, y - m_Size.height + frameSize.b), GetRBFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_LB], Size(frameSize.l, frameSize.b),
                             nn::math::VEC2(x, y - m_Size.height + frameSize.b), GetLBFrameTexCoord);
        pCommand = MakeFrame(drawer, pCommand, m_pFrames[FRAME_L], Size(frameSize.l, m_Size.height - frameSize.t - frameSize.b),
                             nn::math::VEC2(x, innerY), GetLBFrameTexCoord);
        break;
    }

    if (drawer.m_VertexIndexCount != 0) {
        pCommand = drawer.UniformAndDraw(pCommand);
    }
    return pCommand;
}

// 0x004BB150 slot 0x70 (name is ours)
void nw::lyt::Window::DrawContent(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const WindowFrameSize& frameSize, u8 alpha)
{
    m_pContentMaterial->SetupGL(drawInfo, alpha, true);
    const nn::math::VEC2 pos(basePt.x + frameSize.l - m_ContentInflation.l, basePt.y - frameSize.t + m_ContentInflation.t);
    const Size size(m_Size.width - frameSize.l + m_ContentInflation.l - frameSize.r + m_ContentInflation.r,
                    m_Size.height - frameSize.t + m_ContentInflation.t - frameSize.b + m_ContentInflation.b);
    internal::DrawQuad(drawInfo, pos, size, m_ContentTexCoordAry.GetSize(), m_ContentTexCoordAry.GetArray(), m_ContentVtxColors);
}

// 0x004BB43C slot 0x74 (name is ours)
void nw::lyt::Window::DrawFrame(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame& frame, const WindowFrameSize& frameSize, u8 alpha)
{
    if (frame.pMaterial->GetTexMapNum() == 0) {
        return;
    }
    frame.pMaterial->SetupGL(drawInfo, alpha, false);
    const GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;

    Size polSize;
    nn::math::VEC2 pos;

    pos = basePt;
    polSize = Size(m_Size.width - frameSize.r, frameSize.t);
    SetFrameSpec(graphicsResource, s_FrameSpecs[FRAME_LT], s_FlipSpecs[TEXTUREFLIP_NONE]);
    internal::DrawQuad(drawInfo, pos, polSize, 0, nullptr, nullptr);

    pos = nn::math::VEC2(basePt.x + m_Size.width - frameSize.r, basePt.y);
    polSize = Size(frameSize.r, m_Size.height - frameSize.b);
    SetFrameSpec(graphicsResource, s_FrameSpecs[FRAME_RT], s_FlipSpecs[TEXTUREFLIP_FLIPH]);
    internal::DrawQuad(drawInfo, pos, polSize, 0, nullptr, nullptr);

    pos = nn::math::VEC2(basePt.x + frameSize.l, basePt.y - m_Size.height + frameSize.b);
    polSize = Size(m_Size.width - frameSize.l, frameSize.b);
    SetFrameSpec(graphicsResource, s_FrameSpecs[FRAME_RB], s_FlipSpecs[TEXTUREFLIP_ROTATE_180]);
    internal::DrawQuad(drawInfo, pos, polSize, 0, nullptr, nullptr);

    pos = nn::math::VEC2(basePt.x, basePt.y - frameSize.t);
    polSize = Size(frameSize.l, m_Size.height - frameSize.t);
    SetFrameSpec(graphicsResource, s_FrameSpecs[FRAME_LB], s_FlipSpecs[TEXTUREFLIP_FLIPV]);
    internal::DrawQuad(drawInfo, pos, polSize, 0, nullptr, nullptr);
}

// 0x004BAD98 slot 0x78 (name is ours)
void nw::lyt::Window::DrawFrame4(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha)
{
    const GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    bool bModelViewMtxLoaded = false;

    Size polSizes[4];
    nn::math::VEC2 poss[4];
    poss[FRAME_LT] = basePt;
    polSizes[FRAME_LT] = Size(m_Size.width - frameSize.r, frameSize.t);
    poss[FRAME_RT] = nn::math::VEC2(basePt.x + m_Size.width - frameSize.r, basePt.y);
    polSizes[FRAME_RT] = Size(frameSize.r, m_Size.height - frameSize.b);
    poss[FRAME_LB] = nn::math::VEC2(basePt.x, basePt.y - frameSize.t);
    polSizes[FRAME_LB] = Size(frameSize.l, m_Size.height - frameSize.t);
    poss[FRAME_RB] = nn::math::VEC2(basePt.x + frameSize.l, basePt.y - m_Size.height + frameSize.b);
    polSizes[FRAME_RB] = Size(m_Size.width - frameSize.l, frameSize.b);

    for (int i = 0; i <= FRAME_RB; ++i) {
        const Frame& frame = frames[i];
        if (frame.pMaterial == nullptr || frame.pMaterial->GetTexMapNum() == 0) {
            continue;
        }
        frame.pMaterial->SetupGL(drawInfo, alpha, false);
        SetFrameSpec(graphicsResource, s_FrameSpecs[static_cast<u8>(i)], s_FlipSpecs[frame.textureFlip]);
        if (bModelViewMtxLoaded) {
            internal::DrawQuad(drawInfo, poss[i], polSizes[i]);
        } else {
            internal::DrawQuad(drawInfo, poss[i], polSizes[i], 0, nullptr, nullptr);
            bModelViewMtxLoaded = true;
        }
    }
}

// 0x004BAF8C slot 0x7C (name is ours)
void nw::lyt::Window::DrawFrame8(DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Frame* frames, const WindowFrameSize& frameSize, u8 alpha)
{
    const GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    bool bModelViewMtxLoaded = false;

    struct
    {
        nn::math::VEC2 pos;
        Size size;
    } quads[8] = {
        {nn::math::VEC2(basePt.x, basePt.y), Size(frameSize.l, frameSize.t)},
        {nn::math::VEC2(basePt.x + m_Size.width - frameSize.r, basePt.y), Size(frameSize.r, frameSize.t)},
        {nn::math::VEC2(basePt.x, basePt.y - m_Size.height + frameSize.b), Size(frameSize.l, frameSize.b)},
        {nn::math::VEC2(basePt.x + m_Size.width - frameSize.r, basePt.y - m_Size.height + frameSize.b), Size(frameSize.r, frameSize.b)},
        {nn::math::VEC2(basePt.x, basePt.y - frameSize.t), Size(frameSize.l, m_Size.height - frameSize.t - frameSize.b)},
        {nn::math::VEC2(basePt.x + m_Size.width - frameSize.r, basePt.y - frameSize.t),
         Size(frameSize.r, m_Size.height - frameSize.t - frameSize.b)},
        {nn::math::VEC2(basePt.x + frameSize.l, basePt.y), Size(m_Size.width - frameSize.l - frameSize.r, frameSize.t)},
        {nn::math::VEC2(basePt.x + frameSize.l, basePt.y - m_Size.height + frameSize.b),
         Size(m_Size.width - frameSize.l - frameSize.r, frameSize.b)},
    };

    for (int i = 0; i < 8; ++i) {
        const Frame& frame = frames[i];
        if (frame.pMaterial->GetTexMapNum() == 0) {
            continue;
        }
        frame.pMaterial->SetupGL(drawInfo, alpha, false);
        SetFrameSpec(graphicsResource, s_FrameSpecs[static_cast<u8>(i)], s_FlipSpecs[frame.textureFlip]);
        if (bModelViewMtxLoaded) {
            internal::DrawQuad(drawInfo, quads[i].pos, quads[i].size);
        } else {
            internal::DrawQuad(drawInfo, quads[i].pos, quads[i].size, 0, nullptr, nullptr);
            bModelViewMtxLoaded = true;
        }
    }
}

// 0x0073D3A4 (name is ours)
WindowFrameSize nw::lyt::Window::GetFrameSize(u8 frameNum, const Frame* frames) const
{
    WindowFrameSize ret = {0.0f, 0.0f, 0.0f, 0.0f};
    TexSize texSize;

    switch (frameNum) {
    case 1:
        if (GetTextureSize(&texSize, frames[0].pMaterial)) {
            ret.l = texSize.width;
            ret.r = texSize.width;
            ret.t = texSize.height;
            ret.b = texSize.height;
        }
        break;
    case 4:
    case 8:
        if (GetTextureSize(&texSize, frames[FRAME_LT].pMaterial)) {
            ret.t = texSize.height;
        }
        if (GetTextureSize(&texSize, frames[FRAME_RT].pMaterial)) {
            ret.r = texSize.width;
        }
        if (GetTextureSize(&texSize, frames[FRAME_RB].pMaterial)) {
            ret.b = texSize.height;
        }
        if (GetTextureSize(&texSize, frames[FRAME_LB].pMaterial)) {
            ret.l = texSize.width;
        }
        break;
    }
    return ret;
}

} // namespace lyt
} // namespace nw
