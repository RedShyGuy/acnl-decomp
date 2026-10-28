#include "nw/lyt/lyt_Drawer.h"

#include <cstring>

#include "nn/gx/detail/gx_Context.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/font/font_TextWriterBase.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TextBox.h"

namespace nw {
namespace lyt {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_TEXUNIT_CONFIG = 0x080;
const u32 REG_TEXUNIT0_DIM = 0x082;
const u32 REG_TEXUNIT0_PARAM = 0x083;
const u32 REG_TEXUNIT0_ADDR1 = 0x085;
const u32 REG_TEXUNIT0_TYPE = 0x08E;
const u32 REG_TEXUNIT1_DIM = 0x092;
const u32 REG_TEXUNIT1_PARAM = 0x093;
const u32 REG_TEXUNIT1_ADDR = 0x095;
const u32 REG_TEXUNIT1_TYPE = 0x096;
const u32 REG_TEXUNIT2_DIM = 0x09A;
const u32 REG_TEXUNIT2_PARAM = 0x09B;
const u32 REG_TEXUNIT2_ADDR = 0x09D;
const u32 REG_TEXUNIT2_TYPE = 0x09E;
const u32 REG_SH_OUTATTR_CLOCK = 0x06F;
const u32 REG_TEXENV0_SOURCE = 0x0C0;
const u32 REG_TEXENV1_SOURCE = 0x0C8;
const u32 REG_TEXENV1_COLOR = 0x0CB;
const u32 REG_TEXENV2_SOURCE = 0x0D0;
const u32 REG_TEXENV2_COLOR = 0x0D3;
const u32 REG_TEXENV3_SOURCE = 0x0D8;
const u32 REG_TEXENV3_COLOR = 0x0DB;
const u32 REG_TEXENV_UPDATE_BUFFER = 0x0E0;
const u32 REG_TEXENV4_SOURCE = 0x0F0;
const u32 REG_TEXENV4_COLOR = 0x0F3;
const u32 REG_TEXENV5_SOURCE = 0x0F8;
const u32 REG_TEXENV5_COLOR = 0x0FB;
const u32 REG_TEXENV_BUFFER_COLOR = 0x0FD;
const u32 REG_COLOR_OPERATION = 0x100;
const u32 REG_BLEND_FUNC = 0x101;
const u32 REG_LOGIC_OP = 0x102;
const u32 REG_FRAGOP_ALPHA_TEST = 0x104;
const u32 REG_FRAMEBUFFER_FLUSH = 0x111;
const u32 REG_INDEXBUFFER_CONFIG = 0x227;
const u32 REG_NUMVERTICES = 0x228;
const u32 REG_VSH_INTUNIFORM_I0 = 0x2B1;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;

// the header of the five registers of a texture combiner stage
constexpr u32 TexEnvHeader(u32 sourceReg)
{
    return Header(sourceReg, 0xF, 4, true);
}

// the commands of a texture combiner stage (struct and names are ours)
struct TexEnvStageCommand
{
    u32 source;
    u32 header;
    u32 operand;
    u32 combiner;
    ut::Color8 color;
    u32 scale;
};

// a register command whose value is a color
struct ColorCommand
{
    ut::Color8 color;
    u32 header;
};

// the commands of a texture unit
struct TexUnitCommand
{
    u32 dim;
    u32 dimHeader;
    u32 addr;
    u32 addrHeader;
    u32 type;
    u32 typeHeader;
    u32 param;
    u32 paramHeader;
};

// The texture combiners of the materials without combiner stages of their own, by the number of
// textures (the colors are set from the material; only changed colors are sent again).
// no texture: the white color
struct TexEnvNoTexture
{
    TexEnvStageCommand stage5;
};
// one texture, mapped between the black and the white color
struct TexEnvOneTexture
{
    TexEnvStageCommand stage3;
    TexEnvStageCommand stage4;
    TexEnvStageCommand stage5;
};
// two textures, blended with C4
struct TexEnvTwoTextures
{
    TexEnvStageCommand stage3;
    TexEnvStageCommand stage4;
    ColorCommand bufferColor;
    TexEnvStageCommand stage5;
    u32 updateBuffer[2];
};
// three textures, blended with C4
struct TexEnvThreeTextures
{
    TexEnvStageCommand stage1;
    TexEnvStageCommand stage2;
    TexEnvStageCommand stage3;
    TexEnvStageCommand stage4;
    ColorCommand bufferColor;
    TexEnvStageCommand stage5;
    u32 updateBuffer[2];
};

// 0x009826F4 (name is ours)
TexEnvNoTexture s_TexEnvNoTexture = {
    {0x0EE00EE0, TexEnvHeader(REG_TEXENV5_SOURCE), 0x00000000, 0x00010001, ut::Color8(0xFF, 0xFF, 0xFF, 0xFF), 0x00000000},
};
// 0x0098270C (name is ours)
TexEnvOneTexture s_TexEnvOneTexture = {
    {0x0E3E0E3E, TexEnvHeader(REG_TEXENV3_SOURCE), 0x00010010, 0x00010001, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x0F3E0F3E, TexEnvHeader(REG_TEXENV4_SOURCE), 0x00000000, 0x00080008, ut::Color8(0xFF, 0xFF, 0xFF, 0xFF), 0x00000000},
    {0x0FF00FF0, TexEnvHeader(REG_TEXENV5_SOURCE), 0x00000000, 0x00010001, ut::Color8(0, 0, 0, 0), 0x00000000},
};
// the texture units as set (only changed textures are sent again)
// 0x00982754 (name is ours)
TexUnitCommand s_TexUnitCommands[Material::TEXMAP_MAX] = {
    {0x00000000, Header(REG_TEXUNIT0_DIM), 0x00000000, Header(REG_TEXUNIT0_ADDR1), 0x00000000, Header(REG_TEXUNIT0_TYPE), 0x00000000,
     Header(REG_TEXUNIT0_PARAM)},
    {0x00000000, Header(REG_TEXUNIT1_DIM), 0x00000000, Header(REG_TEXUNIT1_ADDR), 0x00000000, Header(REG_TEXUNIT1_TYPE), 0x00000000,
     Header(REG_TEXUNIT1_PARAM)},
    {0x00000000, Header(REG_TEXUNIT2_DIM), 0x00000000, Header(REG_TEXUNIT2_ADDR), 0x00000000, Header(REG_TEXUNIT2_TYPE), 0x00000000,
     Header(REG_TEXUNIT2_PARAM)},
};
// 0x009827B4 (name is ours)
TexEnvTwoTextures s_TexEnvTwoTextures = {
    {0x0E430E43, TexEnvHeader(REG_TEXENV3_SOURCE), 0x00000200, 0x00040004, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x0FDE0FDE, TexEnvHeader(REG_TEXENV4_SOURCE), 0x00000000, 0x00040004, ut::Color8(0xFF, 0xFF, 0xFF, 0xFF), 0x00000000},
    {ut::Color8(0, 0, 0, 0), Header(REG_TEXENV_BUFFER_COLOR)},
    {0x0FF00FF0, TexEnvHeader(REG_TEXENV5_SOURCE), 0x00000000, 0x00010001, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x00000000, Header(REG_TEXENV_UPDATE_BUFFER, 0x2)},
};
// 0x0098280C (name is ours)
TexEnvThreeTextures s_TexEnvThreeTextures = {
    {0x0EE30EE3, TexEnvHeader(REG_TEXENV1_SOURCE), 0x00000020, 0x00010001, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x0FE40FE4, TexEnvHeader(REG_TEXENV2_SOURCE), 0x000600C0, 0x00080008, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x0FE50FE5, TexEnvHeader(REG_TEXENV3_SOURCE), 0x00040080, 0x00080008, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x0FDE0FDE, TexEnvHeader(REG_TEXENV4_SOURCE), 0x00000000, 0x00040004, ut::Color8(0xFF, 0xFF, 0xFF, 0xFF), 0x00000000},
    {ut::Color8(0, 0, 0, 0), Header(REG_TEXENV_BUFFER_COLOR)},
    {0x0FF00FF0, TexEnvHeader(REG_TEXENV5_SOURCE), 0x00000000, 0x00010001, ut::Color8(0, 0, 0, 0), 0x00000000},
    {0x00000000, Header(REG_TEXENV_UPDATE_BUFFER, 0x2)},
};

// the source of a stage that passes the previous result
const u32 TEXENV_SOURCE_PASS = 0x0EEF0EEF;

// 0x004D8484 (name is ours)
// the current command list of gx
DECOMP_NOINLINE u32* GetCommandBufferCurrent()
{
    return nn::gx::detail::s_pCommandBufferCurrent;
}

// 0x004D84E4 (name is ours)
DECOMP_NOINLINE void SetCommandBufferCurrent(u32* pCommand)
{
    nn::gx::detail::s_pCommandBufferCurrent = pCommand;
}

inline f32 DegToFIdx(f32 deg)
{
    return deg * (256.0f / 360.0f);
}

inline u32 ColorToU32Raw(const ut::Color8& color)
{
    return static_cast<u32>(color.r) | (static_cast<u32>(color.g) << 8) | (static_cast<u32>(color.b) << 16) | (static_cast<u32>(color.a) << 24);
}
} // namespace

// 0x004B9FE8
nw::lyt::Drawer::Drawer()
{
    // empty (in the original too: the RectDrawer constructor and the vtable)
}

// 0x004D8734 slot 0x00 (name is ours)
// 0x004BA000 (deleting dtor)
nw::lyt::Drawer::~Drawer()
{
    // empty (in the original too: only the RectDrawer destructor)
}

// 0x004BA244 slot 0x08 (name is ours)
void nw::lyt::Drawer::Finalize()
{
    void* pCommandBuffer = m_pCommandBuffer;
    RectDrawer::Finalize();
    Layout::FreeDeviceMemory(pCommandBuffer);
}

// 0x004D8658 slot 0x0C (name is ours)
void nw::lyt::Drawer::DrawBegin(u32 flags)
{
    RectDrawer::DrawBegin(flags);
}

// 0x004D8594 slot 0x10 (name is ours)
u32* nw::lyt::Drawer::DrawBegin(u32* pCommand, u32 flags)
{
    m_Unknown0x698 = 0;
    m_TexCoordNum = 0;
    m_Unknown0x694 = 0;
    m_TexEnvMode = TEXENV_MODE_UNKNOWN;
    m_IsBlendDefault = true;
    pCommand = SetUpTextures(pCommand, nullptr, false);
    return RectDrawer::DrawBegin(pCommand, flags);
}

// 0x004D8558 slot 0x14 (name is ours)
void nw::lyt::Drawer::DrawEnd(u32 flags)
{
    RectDrawer::DrawEnd(flags);
}

// 0x004D84F4 slot 0x18 (name is ours)
u32* nw::lyt::Drawer::DrawEnd(u32* pCommand, u32 flags)
{
    if (m_VertexIndexCount != 0) {
        pCommand = UniformAndDraw(pCommand);
    }
    return RectDrawer::DrawEnd(pCommand, flags);
}

// 0x004B8AEC | nintendogs:callseq [tier A]
void nw::lyt::Drawer::Initialize(GraphicsResource& graphicsResource, void* pBuffer)
{
    const u32 size = GetCommandBufferSize(graphicsResource.m_pRectDrawerShader) + (pBuffer != nullptr ? 0 : VERTEX_BUFFER_SIZE);
    void* pMem = Layout::AllocDeviceMemory(size, 4);
    if (pBuffer != nullptr) {
        RectDrawer::Initialize(pBuffer, pMem, graphicsResource.m_pRectDrawerShader, graphicsResource.m_RectDrawerShaderSize);
    } else {
        RectDrawer::Initialize(pMem, graphicsResource.m_pRectDrawerShader, graphicsResource.m_RectDrawerShaderSize);
    }
    m_Unknown0x694 = 0;
    m_Unknown0x698 = 0;
    m_TexCoordNum = 0;
    m_IsBlendDefault = true;
    m_IsAlphaTestDefault = true;
    m_TexEnvMode = TEXENV_MODE_UNKNOWN;
}

// 0x004B9F54 (name is ours)
void nw::lyt::Drawer::DrawBegin(const DrawInfo& drawInfo, u32 flags)
{
    u32* pCommand = GetCommandBufferCurrent();
    pCommand = DrawBegin(pCommand, flags);
    if ((flags & 1) == 0) {
        pCommand = SetProjectionMtx(pCommand, drawInfo.m_ProjMtx);
    }
    SetCommandBufferCurrent(pCommand);
}

// 0x004D8494 (name is ours)
void nw::lyt::Drawer::Draw(Layout* pLayout, DrawInfo& drawInfo)
{
    u32* pCommand = GetCommandBufferCurrent();
    if (pLayout != nullptr && pLayout->m_pRootPane != nullptr && pLayout->m_pRootPane->IsVisible()) {
        pCommand = pLayout->m_pRootPane->MakeUniformData(pCommand, drawInfo, *this);
    }
    SetCommandBufferCurrent(pCommand);
}

// 0x004B8B84 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::SetUpTexEnv(u32* pCommand, const Material* pMaterial)
{
    if (pMaterial->IsTextureOnly()) {
        if (m_TexEnvMode != 0 && m_TexEnvMode != 1 && m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        return pCommand;
    }

    bool isFlushed = false;

    // the alpha test
    if (pMaterial->m_MemCap.alpCmp) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_IsAlphaTestDefault = false;
        *pCommand++ = 1;
        *pCommand++ = Header(REG_FRAMEBUFFER_FLUSH);
        isFlushed = true;
        // the register values of the alpha test functions (ARMCC's initializer: 0x008C2754)
        const u32 alphaTestFuncs[8] = {0, 4, 5, 2, 3, 7, 6, 1};
        const AlphaCompare* pAlphaCompare = pMaterial->GetAlphaComparePtr();
        const u32 ref = static_cast<u32>(pAlphaCompare->ref * 255.0f + 0.5f);
        *pCommand++ = 1 | (alphaTestFuncs[pAlphaCompare->func] << 4) | ((ref & 0xFF) << 8);
        *pCommand++ = Header(REG_FRAGOP_ALPHA_TEST);
    } else if (!m_IsAlphaTestDefault) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_IsAlphaTestDefault = true;
        *pCommand++ = 1;
        *pCommand++ = Header(REG_FRAMEBUFFER_FLUSH);
        *pCommand++ = 0x61;
        *pCommand++ = Header(REG_FRAGOP_ALPHA_TEST);
        isFlushed = true;
    }

    // the blending
    if (pMaterial->m_MemCap.blendMode) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_IsBlendDefault = false;
        if (!isFlushed) {
            *pCommand++ = 1;
            *pCommand++ = Header(REG_FRAMEBUFFER_FLUSH);
        }
        pCommand = SetUpBlendMode(pCommand, pMaterial);
    } else if (!m_IsBlendDefault) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_IsBlendDefault = true;
        if (!isFlushed) {
            *pCommand++ = 1;
            *pCommand++ = Header(REG_FRAMEBUFFER_FLUSH);
        }
        // the default blending: source alpha (ARMCC's initializer: 0x008C25B4)
        const u32 blendDefaultCommands[] = {
            0x00E40100, Header(REG_COLOR_OPERATION),
            0x76760000, Header(REG_BLEND_FUNC),
        };
        std::memcpy(pCommand, blendDefaultCommands, sizeof(blendDefaultCommands));
        pCommand += sizeof(blendDefaultCommands) / sizeof(u32);
    }

    // the texture combiner
    if (pMaterial->GetTevStageNum() > 0) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        pCommand = SetUpGLTexEnvUser(pCommand, pMaterial);
        m_TexEnvMode = TEXENV_MODE_USER;
        return pCommand;
    }

    switch (pMaterial->GetTexMapNum()) {
    case 0: {
        const ut::Color8 white = pMaterial->m_Colors[Material::MATCOLOR_WHITE];
        if (m_TexEnvMode != 0) {
            if (m_VertexIndexCount != 0) {
                pCommand = UniformAndDraw(pCommand);
            }
            s_TexEnvNoTexture.stage5.color = white;
            std::memcpy(pCommand, &s_TexEnvNoTexture, sizeof(s_TexEnvNoTexture));
            pCommand += sizeof(s_TexEnvNoTexture) / sizeof(u32);
            m_TexEnvMode = 0;
        } else if (s_TexEnvNoTexture.stage5.color.ToU32() != white.ToU32()) {
            if (m_VertexIndexCount != 0) {
                pCommand = UniformAndDraw(pCommand);
            }
            s_TexEnvNoTexture.stage5.color = white;
            *pCommand++ = ColorToU32Raw(white);
            *pCommand++ = Header(REG_TEXENV5_COLOR);
        }
        break;
    }
    case 1: {
        const ut::Color8 white = pMaterial->m_Colors[Material::MATCOLOR_WHITE];
        const ut::Color8 black = pMaterial->m_Colors[Material::MATCOLOR_BLACK];
        if (m_TexEnvMode != 1) {
            if (m_VertexIndexCount != 0) {
                pCommand = UniformAndDraw(pCommand);
            }
            s_TexEnvOneTexture.stage3.color = black;
            s_TexEnvOneTexture.stage4.color = white;
            std::memcpy(pCommand, &s_TexEnvOneTexture, sizeof(s_TexEnvOneTexture));
            pCommand += sizeof(s_TexEnvOneTexture) / sizeof(u32);
            m_TexEnvMode = 1;
        } else if (s_TexEnvOneTexture.stage3.color.ToU32() != black.ToU32() || s_TexEnvOneTexture.stage4.color.ToU32() != white.ToU32()) {
            if (m_VertexIndexCount != 0) {
                pCommand = UniformAndDraw(pCommand);
            }
            s_TexEnvOneTexture.stage3.color = black;
            s_TexEnvOneTexture.stage4.color = white;
            *pCommand++ = ColorToU32Raw(black);
            *pCommand++ = Header(REG_TEXENV3_COLOR);
            *pCommand++ = ColorToU32Raw(s_TexEnvOneTexture.stage4.color);
            *pCommand++ = Header(REG_TEXENV4_COLOR);
        }
        break;
    }
    case 2:
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        pCommand = SetUpTexEnvType2(pCommand, pMaterial);
        m_TexEnvMode = 2;
        break;
    case 3:
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        pCommand = SetUpTexEnvType3(pCommand, pMaterial);
        m_TexEnvMode = 3;
        break;
    }
    return pCommand;
}

// 0x004B9000 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::SetUpTextBox(u32* pCommand, const TextBox* pTextBox, const Material* pMaterial, const DrawInfo& drawInfo)
{
    font::DispStringBuffer* pDispStringBuf = pTextBox->m_pDispStringBuf;
    if (pDispStringBuf->m_CommandSize == 0) {
        return pCommand;
    }

    m_TexEnvMode = TEXENV_MODE_TEXT;
    m_IsBlendDefault = true;
    m_IsAlphaTestDefault = true;
    pTextBox->GetTextGlobalMtx(reinterpret_cast<nn::math::MTX34*>(m_pMtxData));
    m_MtxCount = 3;
    pCommand = AddUniformMtx(pCommand);

    font::TextWriterBase<wchar_t>& writer = drawInfo.m_pGraphicsResource->m_TextWriter;
    writer.m_pDispStringBuffer = pDispStringBuf;
    const ut::Color8 topColor = pTextBox->m_TextColors[0];
    const ut::Color8 bottomColor = pTextBox->m_TextColors[1];
    writer.m_GradationMode = (topColor.ToU32() != bottomColor.ToU32()) ? font::CharWriter::GRADATION_MODE_V : font::CharWriter::GRADATION_MODE_NONE;
    writer.UpdateVertexColors();
    writer.m_TextColors[0] = topColor;
    writer.m_TextColors[1] = bottomColor;
    writer.UpdateVertexColors();
    writer.m_ColorMappingMin = pMaterial->m_Colors[Material::MATCOLOR_BLACK];
    writer.m_ColorMappingMax = pMaterial->m_Colors[Material::MATCOLOR_WHITE];
    writer.m_Alpha = pTextBox->m_GlbAlpha;
    pCommand = writer.UseCommandBuffer(pCommand, this);
    writer.m_pDispStringBuffer = nullptr;
    return SetUpTextures(pCommand, nullptr, true);
}

// 0x004B910C | nintendogs:callseq-callee [tier A]
DECOMP_NOINLINE u32* nw::lyt::Drawer::SetUpTextures(u32* pCommand, const Material* pMaterial, bool isForce)
{
    if (pMaterial == nullptr) {
        // the textures as set are forgotten
        s_TexUnitCommands[0].addr = 0;
        s_TexUnitCommands[1].addr = 0;
        s_TexUnitCommands[2].addr = 0;
        if (isForce) {
            pCommand = ResetTextureUnits(pCommand);
        }
        return pCommand;
    }

    bool isTexChanged = false;
    u32 texEnable = 0;
    const bool isNoQuad = (m_VertexIndexCount == 0);
    u32 texNum = pMaterial->GetTexMapNum();
    if (texNum > Material::TEXMAP_MAX) {
        texNum = Material::TEXMAP_MAX;
    }
    for (u32 i = 0; i < texNum; ++i) {
        texEnable |= 1 << i;
        const TexMap& texMap = pMaterial->GetTexMapAry()[i];
        TexUnitCommand& command = s_TexUnitCommands[i];
        if (command.addr != texMap.m_PhysicalAddress >> 3 || command.param != texMap.m_ParamReg) {
            isTexChanged = true;
            command.dim = texMap.m_SizeReg;
            command.addr = texMap.m_PhysicalAddress >> 3;
            command.type = texMap.m_FormatReg;
            command.param = texMap.m_ParamReg;
        }
    }

    const bool isUpdate = isNoQuad || isTexChanged;
    if (isUpdate) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        // the texture unit config written with mask 0 (ARMCC's initializer: 0x008C25C4)
        const u32 textureUnitResetCommand[] = {0x00000000, Header(REG_TEXUNIT_CONFIG, 0x0, 2), 0x00000000, 0x00000000};
        std::memcpy(pCommand, textureUnitResetCommand, sizeof(textureUnitResetCommand));
        pCommand += sizeof(textureUnitResetCommand) / sizeof(u32);
    }
    const u32 texUnitConfig = texEnable | 0x11000;
    if (isNoQuad) {
        *pCommand++ = texEnable << 8;
        *pCommand++ = Header(REG_SH_OUTATTR_CLOCK, 0x2);
        *pCommand++ = texUnitConfig;
        *pCommand++ = Header(REG_TEXUNIT_CONFIG, 0xB);
    }
    if (isUpdate) {
        *pCommand++ = texUnitConfig;
        *pCommand++ = Header(REG_TEXUNIT_CONFIG, 0x4);
    }
    if (isTexChanged) {
        std::memcpy(pCommand, s_TexUnitCommands, texNum * sizeof(TexUnitCommand));
        pCommand += texNum * sizeof(TexUnitCommand) / sizeof(u32);
    }
    return pCommand;
}

// 0x004B92A0 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::SetUpBlendMode(u32* pCommand, const Material* pMaterial)
{
    // ARMCC's initializers of the tables: 0x008C2690 - 0x008C2753
    // no blending
    const u32 blendNoneCommands[] = {
        0x00E40100, Header(REG_COLOR_OPERATION),
        0x01010000, Header(REG_BLEND_FUNC),
    };
    // the register values of the indices of BlendMode
    const u32 blendOps[4] = {0, 0, 1, 2};
    const u32 blendSrcFactors[8] = {0, 1, 4, 5, 6, 7, 8, 9};
    const u32 blendDstFactors[8] = {0, 1, 2, 3, 6, 7, 8, 9};
    // the blend commands (the value of the blend function is set)
    const u32 blendCommands[] = {
        0x00E40100, Header(REG_COLOR_OPERATION),
        0x00000000, Header(REG_BLEND_FUNC),
    };
    const u32 logicOps[17] = {0, 6, 0, 4, 3, 5, 7, 1, 8, 9, 10, 11, 12, 2, 13, 14, 15};
    // the logical operation commands (the operation is set)
    const u32 logicOpCommands[] = {
        0x00E40000, Header(REG_COLOR_OPERATION),
        0x00000000, Header(REG_LOGIC_OP),
    };

    const BlendMode* pBlendMode = pMaterial->GetBlendModePtr();
    if (pBlendMode->blendOp != 0) {
        const u32 op = blendOps[pBlendMode->blendOp];
        const u32 src = blendSrcFactors[pBlendMode->srcFactor];
        const u32 dst = blendDstFactors[pBlendMode->dstFactor];
        pCommand[0] = blendCommands[0];
        pCommand[1] = blendCommands[1];
        pCommand[2] = op | (op << 8) | (src << 16) | (dst << 20) | (src << 24) | (dst << 28);
        pCommand[3] = blendCommands[3];
        return pCommand + 4;
    }
    if (pBlendMode->logicOp != 0) {
        pCommand[0] = logicOpCommands[0];
        pCommand[1] = logicOpCommands[1];
        pCommand[2] = logicOps[pBlendMode->logicOp];
        pCommand[3] = logicOpCommands[3];
        return pCommand + 4;
    }
    std::memcpy(pCommand, blendNoneCommands, sizeof(blendNoneCommands));
    return pCommand + sizeof(blendNoneCommands) / sizeof(u32);
}

// 0x004B93B0 | nintendogs:bytes [tier A]
void nw::lyt::Drawer::SetUpVtxColors(const ut::Color8* vtxColors, u8 alpha)
{
    const f32 a = alpha * (1.0f / 255.0f);
    f32 colors[4][4];
    bool isWhite = true;
    for (int i = 0; i < 4; ++i) {
        if (vtxColors[i].ToU32() != 0xFFFFFFFF) {
            colors[i][0] = vtxColors[i].r * (1.0f / 255.0f);
            colors[i][1] = vtxColors[i].g * (1.0f / 255.0f);
            colors[i][2] = vtxColors[i].b * (1.0f / 255.0f);
            colors[i][3] = vtxColors[i].a * (1.0f / 255.0f);
            for (int j = 0; isWhite && j < i; ++j) {
                colors[j][0] = 1.0f;
                colors[j][1] = 1.0f;
                colors[j][2] = 1.0f;
                colors[j][3] = a;
            }
            colors[i][3] *= a;
            isWhite = false;
        } else if (!isWhite) {
            colors[i][0] = 1.0f;
            colors[i][1] = 1.0f;
            colors[i][2] = 1.0f;
            colors[i][3] = a;
        }
    }

    if (isWhite) {
        // only the alpha
        m_VertexIndex[3] = a;
    } else {
        // the colors in the matrix uniforms
        m_VertexIndex[3] = m_MtxCount;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m_pMtxData[m_MtxCount + i][j] = colors[i][j];
            }
        }
        m_MtxCount += 4;
    }
}

// 0x004B9680 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::UniformAndDraw(u32* pCommand)
{
    // the index vectors of the quads
    const u32 count = m_VertexIndexCount;
    m_VertexIndexUniforms.firstData = m_VertexIndexUniforms.GetWord(0);
    m_VertexIndexUniforms.SetWord(0, Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, count * 4 - 1));
    m_VertexIndexUniforms.SetWord(count, 0);
    const u32 size = count * 4 * sizeof(u32) + 16;
    std::memcpy(pCommand, &m_VertexIndexUniforms, size);
    pCommand += size / sizeof(u32);

    // the matrices and colors
    if (m_MtxCount != 0) {
        pCommand = AddUniformMtx(pCommand);
    }

    // the sizes and texture coordinates
    if (m_TexCoordCount != 0) {
        const u32 texCoordCount = m_TexCoordCount;
        m_TexCoordUniforms.firstData = m_TexCoordUniforms.GetWord(0);
        m_TexCoordUniforms.SetWord(0, Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, texCoordCount * 4 - 1));
        m_TexCoordUniforms.SetWord(texCoordCount, 0);
        const u32 texCoordSize = texCoordCount * 4 * sizeof(u32) + 16;
        std::memcpy(pCommand, &m_TexCoordUniforms, texCoordSize);
        pCommand += texCoordSize / sizeof(u32);
        m_TexCoordCount = 0;
    }

    // the draw call
    const u32 vertexCount = m_VertexIndexCount * 6;
    const u32 offset = GetVertexIndexAddressOffset(vertexCount);
    *pCommand++ = m_TexCoordNum;
    *pCommand++ = Header(REG_VSH_INTUNIFORM_I0);
    *pCommand++ = 0x80000000 | offset;
    *pCommand++ = Header(REG_INDEXBUFFER_CONFIG);
    *pCommand++ = vertexCount;
    *pCommand++ = Header(REG_NUMVERTICES);
    const u32 drawCommandSize = 64;
    std::memcpy(pCommand, GetDrawCommands(), drawCommandSize);
    pCommand += drawCommandSize / sizeof(u32);
    m_VertexIndexCount = 0;
    return pCommand;
}

// 0x004B97E0 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::SetUpTexEnvType2(u32* pCommand, const Material* pMaterial)
{
    const ut::Color8 c4 = pMaterial->m_Colors[Material::MATCOLOR_C4];
    const ut::Color8 white = pMaterial->m_Colors[Material::MATCOLOR_WHITE];
    const ut::Color8 black = pMaterial->m_Colors[Material::MATCOLOR_BLACK];
    if (m_TexEnvMode != 2) {
        s_TexEnvTwoTextures.stage3.color = c4;
        s_TexEnvTwoTextures.stage4.color = white;
        s_TexEnvTwoTextures.bufferColor.color = black;
        std::memcpy(pCommand, &s_TexEnvTwoTextures, sizeof(s_TexEnvTwoTextures));
        pCommand += sizeof(s_TexEnvTwoTextures) / sizeof(u32);
    } else if (s_TexEnvTwoTextures.stage3.color.ToU32() != c4.ToU32() || s_TexEnvTwoTextures.stage4.color.ToU32() != white.ToU32() ||
               s_TexEnvTwoTextures.bufferColor.color.ToU32() != black.ToU32()) {
        s_TexEnvTwoTextures.stage3.color = c4;
        s_TexEnvTwoTextures.stage4.color = white;
        s_TexEnvTwoTextures.bufferColor.color = black;
        *pCommand++ = ColorToU32Raw(c4);
        *pCommand++ = Header(REG_TEXENV3_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvTwoTextures.stage4.color);
        *pCommand++ = Header(REG_TEXENV4_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvTwoTextures.bufferColor.color);
        *pCommand++ = Header(REG_TEXENV_BUFFER_COLOR);
    }
    return pCommand;
}

// 0x004B98F8 | nintendogs:callseq-callee [tier A]
u32* nw::lyt::Drawer::SetUpTexEnvType3(u32* pCommand, const Material* pMaterial)
{
    const ut::Color8 c4 = pMaterial->m_Colors[Material::MATCOLOR_C4];
    const ut::Color8 white = pMaterial->m_Colors[Material::MATCOLOR_WHITE];
    const ut::Color8 black = pMaterial->m_Colors[Material::MATCOLOR_BLACK];
    if (m_TexEnvMode != 3) {
        s_TexEnvThreeTextures.stage1.color = c4;
        s_TexEnvThreeTextures.stage2.color = c4;
        s_TexEnvThreeTextures.stage3.color = c4;
        s_TexEnvThreeTextures.stage4.color = white;
        s_TexEnvThreeTextures.bufferColor.color = black;
        std::memcpy(pCommand, &s_TexEnvThreeTextures, sizeof(s_TexEnvThreeTextures));
        pCommand += sizeof(s_TexEnvThreeTextures) / sizeof(u32);
    } else if (s_TexEnvThreeTextures.stage1.color.ToU32() != c4.ToU32() || s_TexEnvThreeTextures.stage4.color.ToU32() != white.ToU32() ||
               s_TexEnvThreeTextures.bufferColor.color.ToU32() != black.ToU32()) {
        s_TexEnvThreeTextures.stage1.color = c4;
        s_TexEnvThreeTextures.stage2.color = c4;
        s_TexEnvThreeTextures.stage3.color = c4;
        s_TexEnvThreeTextures.stage4.color = white;
        s_TexEnvThreeTextures.bufferColor.color = black;
        *pCommand++ = ColorToU32Raw(c4);
        *pCommand++ = Header(REG_TEXENV1_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvThreeTextures.stage2.color);
        *pCommand++ = Header(REG_TEXENV2_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvThreeTextures.stage3.color);
        *pCommand++ = Header(REG_TEXENV3_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvThreeTextures.stage4.color);
        *pCommand++ = Header(REG_TEXENV4_COLOR);
        *pCommand++ = ColorToU32Raw(s_TexEnvThreeTextures.bufferColor.color);
        *pCommand++ = Header(REG_TEXENV_BUFFER_COLOR);
    }
    return pCommand;
}

// 0x004B9A40 (name is ours)
u32* nw::lyt::Drawer::AddQuad(u32* pCommand)
{
    std::memcpy(m_pVertexIndexData[m_VertexIndexCount], m_VertexIndex, sizeof(m_VertexIndex));
    ++m_VertexIndexCount;
    // the uniform batches are full
    if (m_VertexIndexCount + 1 > VERTEX_INDEX_MAX || m_MtxCount + 7 > 32 || m_TexCoordCount + 7 > 32) {
        pCommand = UniformAndDraw(pCommand);
    }
    return pCommand;
}

// 0x004B9AB0 | nintendogs:callseq-callee [tier A]
u32* nw::lyt::Drawer::SetUpGLTexEnvUser(u32* pCommand, const Material* pMaterial)
{
    // ARMCC's initializers of the tables: 0x008C25D4 - 0x008C268F
    // the register values of the indices of TevStage (sources, combine modes, operands, scales)
    const u32 texEnvSources[8] = {0x3, 0x4, 0x5, 0x6, 0xE, 0x0, 0xF, 0xD};
    const u32 texEnvCombines[8] = {0, 1, 2, 3, 4, 5, 9, 8};
    const u32 texEnvOperandsRgb[10] = {0, 1, 2, 3, 4, 5, 8, 9, 12, 13};
    const u32 texEnvOperandsAlpha[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    const u32 texEnvScales[3] = {0, 1, 2};
    // the first registers of the stages
    const u32 texEnvStageRegs[Material::TEVSTAGE_MAX] = {REG_TEXENV0_SOURCE, REG_TEXENV1_SOURCE, REG_TEXENV2_SOURCE,
                                                         REG_TEXENV3_SOURCE, REG_TEXENV4_SOURCE, REG_TEXENV5_SOURCE};
    // the operand, combiner, color and scale of a stage that passes the previous result
    const u32 texEnvPassParams[4] = {0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000};

    u32 updateBuffer = 0;
    u32 tevStageNum = pMaterial->GetTevStageNum();
    if (tevStageNum >= Material::TEVSTAGE_MAX) {
        tevStageNum = Material::TEVSTAGE_MAX;
    }

    if (tevStageNum > 0) {
        // the stages are the last ones
        const u32 firstStage = Material::TEVSTAGE_MAX - tevStageNum;
        for (u32 i = 0; i < tevStageNum; ++i) {
            const TevStage& tevStage = pMaterial->GetTevStageAry()[i];
            const ut::Color8& konstRgb = pMaterial->m_Colors[tevStage.konstColor.rgb];
            const ut::Color8& konstAlpha = pMaterial->m_Colors[tevStage.konstColor.alpha];
            u32 stage = firstStage + i;

            if (i == 0 && !((tevStage.rgb.src0 == 4 || tevStage.rgb.src1 == 4 || tevStage.rgb.src2 == 4) &&
                            (tevStage.alpha.src0 == 4 || tevStage.alpha.src1 == 4 || tevStage.alpha.src2 == 4))) {
                // the stages before pass the previous result on, the first one is stage 0
                for (u32 j = 1; j <= stage; ++j) {
                    *pCommand++ = TEXENV_SOURCE_PASS;
                    *pCommand++ = TexEnvHeader(texEnvStageRegs[j]);
                    std::memcpy(pCommand, texEnvPassParams, sizeof(texEnvPassParams));
                    pCommand += sizeof(texEnvPassParams) / sizeof(u32);
                }
                stage = 0;
            } else if (i - 1 <= 3) {
                updateBuffer |= (tevStage.rgb.isSavePrevReg << (stage + 7)) | (tevStage.alpha.isSavePrevReg << (stage + 11));
            }

            const TevStageInOp& rgb = tevStage.rgb;
            const TevStageInOp& alpha = tevStage.alpha;
            ut::Color8 konstColor = konstRgb;
            konstColor.a = konstAlpha.a;
            pCommand[0] = texEnvSources[rgb.src0] | (texEnvSources[rgb.src1] << 4) | (texEnvSources[rgb.src2] << 8) |
                          (texEnvSources[alpha.src0] << 16) | (texEnvSources[alpha.src1] << 20) | (texEnvSources[alpha.src2] << 24);
            pCommand[1] = TexEnvHeader(texEnvStageRegs[stage]);
            pCommand[2] = texEnvOperandsRgb[rgb.op0] | (texEnvOperandsRgb[rgb.op1] << 4) | (texEnvOperandsRgb[rgb.op2] << 8) |
                          (texEnvOperandsAlpha[alpha.op0] << 12) | (texEnvOperandsAlpha[alpha.op1] << 16) | (texEnvOperandsAlpha[alpha.op2] << 20);
            pCommand[3] = texEnvCombines[rgb.combine] | (texEnvCombines[alpha.combine] << 16);
            pCommand[4] = ColorToU32Raw(konstColor);
            pCommand[5] = texEnvScales[rgb.scale] | (texEnvScales[alpha.scale] << 16);
            pCommand += 6;
        }
    }

    *pCommand++ = ColorToU32Raw(pMaterial->m_Colors[Material::MATCOLOR_BLACK]);
    *pCommand++ = Header(REG_TEXENV_BUFFER_COLOR);
    *pCommand++ = updateBuffer;
    *pCommand++ = Header(REG_TEXENV_UPDATE_BUFFER, 0x2);
    return pCommand;
}

// 0x004B9E74 | nintendogs:callseq [tier A]
u32* nw::lyt::Drawer::SetUpTextureCoords(u32* pCommand, const nn::math::VEC4* pCoords, int coordNum)
{
    if (m_TexCoordNum != coordNum) {
        if (m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_TexCoordNum = coordNum;
    }
    if (coordNum != 0) {
        if (coordNum >= 2 && m_VertexIndexCount != 0) {
            pCommand = UniformAndDraw(pCommand);
        }
        m_VertexIndex[2] = m_TexCoordCount;
        std::memcpy(m_pTexCoordData[m_TexCoordCount], pCoords, coordNum * sizeof(nn::math::VEC4));
        m_TexCoordCount += coordNum;
    }
    return pCommand;
}

// 0x004B9F1C | nintendogs:bytes [tier A]
void nw::lyt::Drawer::SetUpMtx(const nn::math::MTX34& mtx)
{
    nn::math::ARMv6::MTX34CopyAsm(reinterpret_cast<nn::math::MTX34*>(m_pMtxData[m_MtxCount]), &mtx);
    m_VertexIndex[0] = m_MtxCount;
    m_MtxCount += 3;
}

// 0x004B9FA8 | nintendogs:bytes [tier A]
void nw::lyt::Drawer::SetUpQuad(const Size& size, const nn::math::VEC2& pos)
{
    const u8 idx = m_TexCoordCount;
    f32* pData = m_pTexCoordData[idx];
    pData[0] = size.width;
    pData[1] = size.height;
    pData[2] = pos.x;
    pData[3] = pos.y;
    m_VertexIndex[1] = idx;
    m_TexCoordCount = idx + 1;
}

// 0x0073CEB0 | nintendogs:bytes-fuzzy [tier A]
int nw::lyt::Drawer::CalcTextureCoords(const Material* pMaterial, const TexCoordQuad* pTexCoords, nn::math::VEC4* pCoords) const
{
    u32 texNum = pMaterial->GetTexCoordGenNum();
    if (texNum > Material::TEXMAP_MAX) {
        texNum = Material::TEXMAP_MAX;
    } else if (texNum == 0) {
        return 0;
    }

    // the center of the rotation and scale of the texture coordinates
    // 0x0097F4CC
    static const nn::math::VEC2 center(0.5f, 0.5f);

    for (u32 i = 0; i < texNum; ++i) {
        const u8 src = pMaterial->GetTexCoordGenAry()[i].texGenSrc;
        const TexMap& texMap = pMaterial->GetTexMapAry()[i];
        const f32 scaleU = static_cast<f32>(texMap.m_Size.width) / static_cast<f32>(texMap.m_RealSize.width);
        const f32 scaleV = static_cast<f32>(texMap.m_Size.height) / static_cast<f32>(texMap.m_RealSize.height);
        const TexSRT& texSRT = pMaterial->GetTexSRTAry()[i];
        const nn::math::VEC2* tc = pTexCoords[src];
        nn::math::VEC4& coord0 = pCoords[i];
        nn::math::VEC4& coord1 = pCoords[i + 3];

        if (texSRT.rotate == 0.0f && texSRT.scale.x == 1.0f && texSRT.scale.y == 1.0f) {
            // only the translation
            coord0.x = (tc[0].x + texSRT.translate.x) * scaleU;
            coord0.y = 1.0f - (tc[0].y + texSRT.translate.y) * scaleV;
            coord0.z = (tc[3].x + texSRT.translate.x) * scaleU;
            coord0.w = 1.0f - (tc[3].y + texSRT.translate.y) * scaleV;
            coord1.x = (tc[2].x + texSRT.translate.x) * scaleU;
            coord1.y = 1.0f - (tc[2].y + texSRT.translate.y) * scaleV;
            coord1.z = (tc[1].x + texSRT.translate.x) * scaleU;
            coord1.w = 1.0f - (tc[1].y + texSRT.translate.y) * scaleV;
        } else {
            f32 sinR, cosR;
            nn::math::SinCosFIdx(&sinR, &cosR, DegToFIdx(texSRT.rotate));
            const f32 m00 = cosR * texSRT.scale.x * scaleU;
            const f32 m10 = sinR * texSRT.scale.x * scaleV;
            const f32 m11 = cosR * texSRT.scale.y * scaleV;
            const f32 m01 = -(sinR * texSRT.scale.y) * scaleU;
            const f32 m02 = (center.x + texSRT.translate.x) * scaleU;
            const f32 m12 = (center.y + texSRT.translate.y) * scaleV;

            f32 dx = tc[0].x - center.x;
            f32 dy = tc[0].y - center.y;
            coord0.x = m00 * dx + m01 * dy + m02;
            coord0.y = 1.0f - (m10 * dx + m11 * dy + m12);
            dx = tc[3].x - center.x;
            dy = tc[3].y - center.y;
            coord0.z = m00 * dx + m01 * dy + m02;
            coord0.w = 1.0f - (m10 * dx + m11 * dy + m12);
            dx = tc[2].x - center.x;
            dy = tc[2].y - center.y;
            coord1.x = m00 * dx + m01 * dy + m02;
            coord1.y = 1.0f - (m10 * dx + m11 * dy + m12);
            dx = tc[1].x - center.x;
            dy = tc[1].y - center.y;
            coord1.z = m00 * dx + m01 * dy + m02;
            coord1.w = 1.0f - (m10 * dx + m11 * dy + m12);
        }
    }

    // the quads are rectangles parallel to the axes: two vectors per texture are enough
    for (u32 i = 0; i < texNum; ++i) {
        const nn::math::VEC4& coord0 = pCoords[i];
        const nn::math::VEC4& coord1 = pCoords[i + 3];
        if (coord0.x != coord1.x || coord0.z != coord1.z || coord0.y != coord1.w || coord0.w != coord1.y) {
            return 6;
        }
    }
    return texNum;
}

} // namespace lyt
} // namespace nw
