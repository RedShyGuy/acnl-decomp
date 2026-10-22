#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector4.h"
#include "nw/font/font_RectDrawer.h"
#include "nw/lyt/lyt_Types.h"
#include "nw/ut/ut_Color8.h"

namespace nw {
namespace lyt {
class DrawInfo;
class GraphicsResource;
class Layout;
class Material;
class TextBox;

// RTTI N2nw3lyt6DrawerE @ 0x008D087C
// vtable 0x00902C20 (vptr 0x00902C28), offset_to_top 0, 7 entries
//
// Draws layouts with GPU commands instead of GL: the panes put their quads into the uniform
// batches of the font RectDrawer (the vertex shader draws up to 26 quads per draw call) and the
// materials their texture combiner, blending and alpha test as register commands. The names of
// the SetUp* functions and UniformAndDraw are from the reference symbols (mk7dlp; the binary
// passes the command list as the first argument and returns its end); the others are ours.
class Drawer : public ::nw::font::RectDrawer
{
public:
    // the texture combiner the last material set (m_TexEnvMode; 4 is the one of the text, 5 a
    // user combiner)
    static const u8 TEXENV_MODE_TEXT = 4;
    static const u8 TEXENV_MODE_USER = 5;
    static const u8 TEXENV_MODE_UNKNOWN = 6;

    Drawer(); // 0x004B9FE8
    virtual ~Drawer(); // 0x004D8734 slot 0x00 (name is ours)
    // 0x004BA000 (deleting dtor)
    // the command buffer is released as well
    virtual void Finalize(); // 0x004BA244 slot 0x08 (name is ours)
    virtual void DrawBegin(u32 flags); // 0x004D8658 slot 0x0C (name is ours)
    // the textures are reset first
    virtual u32* DrawBegin(u32* pCommand, u32 flags); // 0x004D8594 slot 0x10 (name is ours)
    virtual void DrawEnd(u32 flags); // 0x004D8558 slot 0x14 (name is ours)
    // the pending quads are drawn first
    virtual u32* DrawEnd(u32* pCommand, u32 flags); // 0x004D84F4 slot 0x18 (name is ours)

    // the buffers (pBuffer: the vertex buffer and the command buffer; NULL: the command buffer
    // from the device memory and the vertex buffer written) and the shader of the resource
    void Initialize(GraphicsResource& graphicsResource, void* pBuffer); // 0x004B8AEC | nintendogs:callseq [tier A]
    // the drawing state into the global command list, with the projection of the draw info
    void DrawBegin(const DrawInfo& drawInfo, u32 flags); // 0x004B9F54 (name is ours)
    // the commands of the visible panes of the layout into the global command list
    void Draw(Layout* pLayout, DrawInfo& drawInfo); // 0x004D8494 (name is ours)

    u32* SetUpTexEnv(u32* pCommand, const Material* pMaterial); // 0x004B8B84 | nintendogs:callseq [tier A]
    u32* SetUpTextBox(u32* pCommand, const TextBox* pTextBox, const Material* pMaterial, const DrawInfo& drawInfo); // 0x004B9000 | nintendogs:callseq [tier A]
    u32* SetUpTextures(u32* pCommand, const Material* pMaterial, bool isForce); // 0x004B910C | nintendogs:callseq-callee [tier A]
    u32* SetUpBlendMode(u32* pCommand, const Material* pMaterial); // 0x004B92A0 | nintendogs:callseq [tier A]
    // the vertex colors of the next quad (multiplied with alpha)
    void SetUpVtxColors(const ut::Color8* vtxColors, u8 alpha); // 0x004B93B0 | nintendogs:bytes [tier A]
    // the batched uniforms and the draw call of the quads
    u32* UniformAndDraw(u32* pCommand); // 0x004B9680 | nintendogs:callseq [tier A]
    // (symbols.json swaps the two: by the order of mk7dlp and the modes they serve 0x004B97E0 is
    // Type2 and 0x004B98F8 Type3)
    u32* SetUpTexEnvType2(u32* pCommand, const Material* pMaterial); // 0x004B97E0 | nintendogs:callseq [tier A]
    u32* SetUpTexEnvType3(u32* pCommand, const Material* pMaterial); // 0x004B98F8 | nintendogs:callseq-callee [tier A]
    u32* SetUpGLTexEnvUser(u32* pCommand, const Material* pMaterial); // 0x004B9AB0 | nintendogs:callseq-callee [tier A]
    u32* SetUpTextureCoords(u32* pCommand, const nn::math::VEC4* pCoords, int coordNum); // 0x004B9E74 | nintendogs:callseq [tier A]
    void SetUpMtx(const nn::math::MTX34& mtx); // 0x004B9F1C | nintendogs:bytes [tier A]
    void SetUpQuad(const Size& size, const nn::math::VEC2& pos); // 0x004B9FA8 | nintendogs:bytes [tier A]
    // the quad is finished (the batches drawn when they are full)
    u32* AddQuad(u32* pCommand); // 0x004B9A40 (name is ours)
    // the texture coordinates of the textures of the material for the quad (two vectors per
    // texture, NULL coordinates: the whole texture); the vectors to send (6: all)
    int CalcTextureCoords(const Material* pMaterial, const TexCoordQuad* pTexCoords, nn::math::VEC4* pCoords) const; // 0x0073CEB0 | nintendogs:bytes-fuzzy [tier A]

    u32 m_Unknown0x694;     // 0x694 (only reset)
    u32 m_Unknown0x698;     // 0x698 (only reset)
    int m_TexCoordNum;      // 0x69C the texture coordinate vectors of the quads
    bool m_IsBlendDefault;  // 0x6A0 the default blending is set
    bool m_IsAlphaTestDefault; // 0x6A1 the default alpha test is set
    u8 m_TexEnvMode;        // 0x6A2 TEXENV_MODE_*
};
ASSERT_SIZE(Drawer, 0x6A4);
} // namespace lyt
} // namespace nw
