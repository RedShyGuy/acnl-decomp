#include <cstring>

#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gx/detail/gx_Context.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nw/font/font_CharWriter.h"
#include "nw/font/font_Font.h"
#include "nw/font/font_RectDrawer.h"
#include "nw/font/internal/font_TextureObject.h"
#include "nw/font/internal/internal_Api.h"

namespace nw {
namespace font {
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
const u32 REG_INDEXBUFFER_CONFIG = 0x227;
const u32 REG_NUMVERTICES = 0x228;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;
const u32 REG_VSH_CODETRANSFER_INDEX = 0x2CB;
const u32 REG_VSH_CODETRANSFER_DATA = 0x2CC;
const u32 REG_VSH_OPDESCS_INDEX = 0x2D5;
const u32 REG_VSH_OPDESCS_DATA = 0x2D6;
const u32 REG_VSH_ENTRYPOINT = 0x2BA;
const u32 REG_VSH_OUTMAP_MASK = 0x2BD;
const u32 REG_SH_OUTMAP_TOTAL = 0x04F;
const u32 REG_SH_OUTMAP_O0 = 0x050;
const u32 REG_SH_OUTATTR_MODE = 0x064;
const u32 REG_SH_OUTATTR_CLOCK = 0x06F;
const u32 REG_ATTRIBBUFFERS_LOC = 0x200;

// the float uniform index: 32 bit floats from the register
constexpr u32 UniformIndex32(u32 reg)
{
    return 0x80000000 | reg;
}

// the end of the shader code upload
// 0x008B4748 (name is ours)
const u32 s_ShaderCodeEndCommand[] = {0x00000001, Header(0x2BF)};
// the first output map register of the output semantics (position, normal quaternion, color,
// texture coordinate 0, its w, view, texture coordinates 1 and 2)
// 0x008B4750 (name is ours)
const u8 s_OutputSemanticBase[] = {0x00, 0x04, 0x08, 0x0C, 0x10, 0x0E, 0x16, 0x12};
// the head of the output map command
// 0x008B4758 (name is ours)
const u32 s_OutputMapCommand[] = {0x00000000, Header(REG_SH_OUTMAP_O0, 0xF, 6, true)};
// the texture unit 0 is reset (texture unit config written with mask 0)
// 0x008B4760 (name is ours)
const u32 s_TextureUnitResetCommand[] = {0x00000000, Header(REG_TEXUNIT_CONFIG, 0x0, 2), 0x00000000, 0x00000000};
// the state of the text drawing (the start of the commands of a string)
// 0x008B4770 (name is ours)
const u32 s_TextStateCommands[] = {
    0x00000001, Header(0x2B1),
    0x00000001, Header(0x111),
    0x00E40100, Header(0x100),
    0x76760000, Header(0x101),
    0x00000061, Header(0x104),
    0x0FF00FF0, Header(0x0F8, 0xF, 4, true), 0x00000000, 0x00010001, 0x00000000, 0x00000000,
    0x00000100, Header(REG_SH_OUTATTR_CLOCK, 0x2),
    0x00000000, Header(REG_TEXUNIT_CONFIG, 0x0, 2), 0x00000000, 0x00000000,
    0x00011001, Header(REG_TEXUNIT_CONFIG, 0xB),
};
// the texture combiner change between a color and an alpha texture
// 0x008B47D0 (name is ours)
const u32 s_ColorTextureCombinerCommands[] = {
    0x00000000, Header(0x0D9),
    0x00001001, Header(0x0F1),
};
// 0x008B47E0 (name is ours)
const u32 s_AlphaTextureCombinerCommands[] = {
    0x00000001, Header(0x0D9),
    0x00001000, Header(0x0F1),
};
// the texture combiners 3 and 4 for a color and an alpha texture (the color mapping)
// 0x008B47F0 (name is ours)
const u32 s_ColorTextureCombinerSetupCommands[] = {
    0x0EE30EE3, Header(0x0D8, 0xF, 2, true), 0x00000000, 0x00010001,
    0x00000000, Header(0x0DC),
    0x0FE30FE3, Header(0x0F0, 0xF, 2, true), 0x00001001, 0x00080008,
    0x00000000, Header(0x0F4),
};
// 0x008B4820 (name is ours)
const u32 s_AlphaTextureCombinerSetupCommands[] = {
    0x0EE30EE3, Header(0x0D8, 0xF, 2, true), 0x00000001, 0x00010001,
    0x00000000, Header(0x0DC),
    0x0FE30FE3, Header(0x0F0, 0xF, 2, true), 0x00001000, 0x00080008,
    0x00000000, Header(0x0F4),
};
// the rest of the state commands of InitializeCMD
// 0x008B4850 (name is ours)
const u32 s_StateCommands[] = {
    0x00000001, Header(0x111),
    0x00000001, Header(0x110),
    0x00E40100, Header(0x100),
    0x76760000, Header(0x101),
    0x00000061, Header(0x104),
    0x00000000, Header(0x0E0),
    0x00000000, Header(0x08F),
    0x00000001, Header(0x1C6),
    0x0000000F, Header(0x112),
    0x00000000, Header(0x084),
    0x00000000, Header(0x094),
    0x00000000, Header(0x09C),
    0x00000001, Header(0x08B),
    0x00000000, Header(0x22A),
    0x00000000, Header(0x229, 0x2),
    0x00000000, Header(0x253, 0x3),
    0x00000000, Header(0x25E, 0x2),
    0x00000001, Header(0x25F),
};
// the end of the drawing (the draw command cache)
// 0x008B48E0 (name is ours)
const u32 s_DrawEndCommands[] = {
    0x00000000, Header(0x232, 0xF, 3, true), 0x3F000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, Header(0x111),
    0x00000001, Header(0x110),
};
// the draw call behind the index buffer configuration and the number of vertices
// 0x008B4908 (name is ours)
const u32 s_DrawCommands[] = {
    0x00000001, Header(0x253, 0x1),
    0x00000000, Header(0x245, 0x1),
    0x00000001, Header(0x22E),
    0x00000001, Header(0x245, 0x1),
    0x00000001, Header(0x231),
    0x00000000, Header(0x25E, 0x8),
    0x00000000, Header(0x25E, 0x8),
    0x00000000, Header(0x253, 0x1),
};
// the vertices of a character: the corners of two triangles
// 0x008B4948 (name is ours)
const int s_CharVertexIds[6] = {1, 0, 3, 3, 0, 2};
// the start of the commands of InitializeCMD (registers written with mask 0: placeholders)
// 0x008B4960 (name is ours)
const u32 s_SetupCommands[] = {
    0x00000000, Header(0x251, 0x0, 9), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x00000000, Header(REG_ATTRIBBUFFERS_LOC, 0x0, 29), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x00000000, Header(0x229, 0x1),
    0x00000000, Header(REG_ATTRIBBUFFERS_LOC, 0x0, 29), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x00000000, Header(0x244, 0x1),
};
// the rest of the vertex attribute buffer command (39 parameters from GPUREG_ATTRIBBUFFERS_LOC:
// the first ones are written by the code) and the vertex shader input
// 0x008B4B38 (name is ours)
const u32 s_AttributeBufferCommandsRest[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x00000000, Header(0x2BB),
    0x00000000, Header(0x2BC),
};

// the vertex buffer of the characters: per character six vertices (character index, vertex id,
// x and y of the corner)
// 0x0097F11C (name is ours)
bool s_IsVertexBufferBuilt;
// 0x00AEF554 (name is ours)
f32 s_VertexBuffer[RectDrawer::VERTEX_INDEX_MAX * 6][4];

// the size of the setup commands of the shader in words (code and operand descriptors in blocks
// of 128 words, six words per constant; inline, name is ours)
inline u32 CalcShaderCommandSize(u32 count)
{
    u32 size = (count >> 7) * (128 + 4);
    if ((count & 127) != 0) {
        size += ((count & 127) + 4) & ~1;
    }
    return size;
}
inline u32 CalcShaderCommandSize(const u32* pDvlp, const u32* pDvle)
{
    return CalcShaderCommandSize(pDvlp[3]) + CalcShaderCommandSize(pDvlp[5]) + pDvle[7] * 6;
}
// the DVLP and the first DVLE of a shader binary (DVLB after 3dbrew "SHBIN"; inline, names ours)
inline const u32* GetDvlp(const void* pShaderBinary)
{
    const u32* pDvlb = static_cast<const u32*>(pShaderBinary);
    return pDvlb + 2 + pDvlb[1];
}
inline const u8* GetDvle(const void* pShaderBinary)
{
    const u32* pDvlb = static_cast<const u32*>(pShaderBinary);
    return static_cast<const u8*>(pShaderBinary) + pDvlb[2];
}

// the commands are appended to the command buffer of the string (inline; name is ours)
inline void AddCommands(DispStringBuffer* pBuffer, const void* pCommands, u32 size)
{
    std::memcpy(pBuffer->m_pCommandBuffer + pBuffer->m_CommandSize, pCommands, size);
    pBuffer->m_CommandSize += size / sizeof(u32);
}
inline void AddCommand(DispStringBuffer* pBuffer, u32 param, u32 header)
{
    pBuffer->m_pCommandBuffer[pBuffer->m_CommandSize] = param;
    pBuffer->m_pCommandBuffer[pBuffer->m_CommandSize + 1] = header;
    pBuffer->m_CommandSize += 2;
}

inline bool IsAlphaFormat(int format)
{
    return format == FONT_SHEET_FORMAT_A4 || format == FONT_SHEET_FORMAT_A8;
}
} // namespace

// 0x004D8684 (name is ours)
nw::font::RectDrawer::RectDrawer()
    : m_VertexIndexCount(0), m_MtxCount(0), m_TexCoordCount(0), m_ColorCount(0), m_pCommandBuffer(nullptr), m_pVertexBuffer(nullptr),
      m_VertexBufferAddress(0), m_pFirstSubroutineEnd(nullptr), m_pLastSubroutineEnd(nullptr), m_pSubroutineSizeAddress(nullptr),
      m_IsColorUniformsSet(false)
{
    m_VertexIndex[0] = 0.0f;
    m_VertexIndex[1] = 0.0f;
    m_VertexIndex[2] = 0.0f;
    m_VertexIndex[3] = 0.0f;
    m_pVertexIndexData = m_VertexIndexUniforms.data;
    m_pMtxData = m_MtxUniforms.data;
    m_pTexCoordData = m_TexCoordUniforms.data;
    m_pColorData = m_ColorUniforms.data;
}

// 0x004D8738 slot 0x00 | mk7dlp:bytes
// 0x004D8718 (deleting dtor)
nw::font::RectDrawer::~RectDrawer()
{
    Finalize();
}

// 0x004D8584 slot 0x08
void nw::font::RectDrawer::Finalize()
{
    m_pCommandBuffer = nullptr;
    m_pVertexBuffer = nullptr;
}

// 0x004D865C slot 0x0C
void nw::font::RectDrawer::DrawBegin(u32 flags)
{
    nn::gx::detail::s_pCommandBufferCurrent = DrawBegin(nn::gx::detail::s_pCommandBufferCurrent, flags);
}

// 0x004D85E4 slot 0x10
u32* nw::font::RectDrawer::DrawBegin(u32* pCommand, u32 flags)
{
    if ((flags & 1) == 0) {
        // the texture unit is reset, then the state commands
        pCommand = ResetTextureUnits(pCommand);
        std::memcpy(pCommand, m_CommandCache.m_pBuffer, m_CommandCache.m_Size);
        pCommand += m_CommandCache.m_Size / sizeof(u32);
    }
    m_pFirstSubroutineEnd = nullptr;
    return pCommand;
}

// 0x004D7850 (name is ours)
u32* nw::font::RectDrawer::ResetTextureUnits(u32* pCommand) const
{
    *pCommand++ = 0x00000000;
    *pCommand++ = Header(REG_SH_OUTATTR_CLOCK, 0x2);
    std::memcpy(pCommand, s_TextureUnitResetCommand, sizeof(s_TextureUnitResetCommand));
    pCommand[4] = 0x00001000;
    pCommand[5] = Header(REG_TEXUNIT_CONFIG, 0xB);
    return pCommand + 6;
}

// 0x004D855C slot 0x14
void nw::font::RectDrawer::DrawEnd(u32 flags)
{
    nn::gx::detail::s_pCommandBufferCurrent = DrawEnd(nn::gx::detail::s_pCommandBufferCurrent, flags);
}

// 0x004D8524 slot 0x18
u32* nw::font::RectDrawer::DrawEnd(u32* pCommand, u32 flags)
{
    if ((flags & 1) == 0) {
        std::memcpy(pCommand, m_DrawCommandCache.m_pBuffer, m_DrawCommandCache.m_Size);
        pCommand += m_DrawCommandCache.m_Size / sizeof(u32);
    }
    return pCommand;
}

// 0x004D6EF4 (name is ours)
void nw::font::RectDrawer::Initialize(void* pBuffer, const void* pShaderBinary, u32 size)
{
    void* pVertexBuffer = static_cast<u8*>(pBuffer) + GetCommandBufferSize(pShaderBinary);
    InitializeCMD(pVertexBuffer, pBuffer, pShaderBinary, size, true);
}

// 0x004D6F9C | nintendogs:bytes [tier A]
void nw::font::RectDrawer::Initialize(void* pVertexBuffer, void* pCommandBuffer, const void* pShaderBinary, u32 size)
{
    InitializeCMD(pVertexBuffer, pCommandBuffer, pShaderBinary, size, false);
}

// 0x004D6FBC (name is ours)
void nw::font::RectDrawer::SetParameter(f32 x, f32 y, f32 z)
{
    f32* pData = m_ProjectionUniforms.data[4];
    pData[0] = x;
    pData[1] = y;
    pData[2] = z;
    pData[3] = 0.0f;
}

// 0x004D6FD0 | libgarden [tier A]
u32* nw::font::RectDrawer::AddUniformMtx(u32* pCommand)
{
    UniformBlock<MTX_MAX>& block = m_MtxUniforms;
    const u32 count = m_MtxCount;
    block.firstData = *reinterpret_cast<u32*>(&block.data[0][0]);
    *reinterpret_cast<u32*>(&block.data[0][0]) = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, count * 4 - 1);
    *reinterpret_cast<u32*>(&block.data[count][0]) = 0;
    const u32 size = count * 4 * sizeof(u32) + 16;
    std::memcpy(pCommand, &block, size);
    m_MtxCount = 0;
    return pCommand + size / sizeof(u32);
}

// 0x004D703C | nintendogs:callgraph [tier A]
void nw::font::RectDrawer::InitializeCMD(void* pVertexBuffer, void* pCommandBuffer, const void* pShaderBinary, u32 size, bool isInitVertexBuffer)
{
    const u32* pDvlp = GetDvlp(pShaderBinary);
    const u8* pDvle = GetDvle(pShaderBinary);
    const u32* pDvleWords = reinterpret_cast<const u32*>(pDvle);
    m_pVertexBuffer = pVertexBuffer;
    if (isInitVertexBuffer) {
        std::memcpy(pVertexBuffer, GetVertexBufferData(), VERTEX_BUFFER_SIZE);
        nngxlowStoreDataCache(reinterpret_cast<uptr>(pVertexBuffer), VERTEX_BUFFER_SIZE);
    }
    m_pCommandBuffer = pCommandBuffer;

    // the state commands, then the draw end commands
    const u32 commandSize = CalcShaderCommandSize(pDvlp, pDvleWords) * sizeof(u32) + 776;
    m_CommandCache.Init(pCommandBuffer, commandSize, true);
    m_DrawCommandCache.Init(static_cast<u8*>(m_pCommandBuffer) + commandSize, 216, true);
    m_CommandCache.Add(s_SetupCommands, sizeof(s_SetupCommands));

    // the shader code in blocks of up to 128 words
    const u32* pCode = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(pDvlp) + pDvlp[2]);
    for (u32 i = 0; i < pDvlp[3];) {
        u32 count = pDvlp[3] - i;
        if (count >= 128) {
            count = 128;
        }
        const u32 command[4] = {i, Header(REG_VSH_CODETRANSFER_INDEX), pCode[i], Header(REG_VSH_CODETRANSFER_DATA, 0xF, count - 1)};
        m_CommandCache.Add(command, sizeof(command));
        m_CommandCache.Add(&pCode[i + 1], count * sizeof(u32) - sizeof(u32));
        m_CommandCache.RoundUp(8);
        i += count;
    }
    m_CommandCache.Add(s_ShaderCodeEndCommand, sizeof(s_ShaderCodeEndCommand));

    // the operand descriptors (their first words) in blocks of up to 128
    const u32* pOpdescs = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(pDvlp) + pDvlp[4]);
    for (u32 i = 0; i < pDvlp[5];) {
        u32 count = pDvlp[5] - i;
        if (count >= 128) {
            count = 128;
        }
        const u32 command[4] = {i, Header(REG_VSH_OPDESCS_INDEX), pOpdescs[i * 2], Header(REG_VSH_OPDESCS_DATA, 0xF, count - 1)};
        m_CommandCache.Add(command, sizeof(command));
        for (u32 j = 1; j < count; j++) {
            m_CommandCache.Add(&pOpdescs[(i + j) * 2], sizeof(u32));
        }
        m_CommandCache.RoundUp(8);
        i += count;
    }

    // the float constants of the DVLE (float24 x, y, z, w in four words) as three words
    const u8* pConsts = pDvle + pDvleWords[6];
    for (u32 i = 0; i < pDvleWords[7]; i++) {
        const u8* pConst = pConsts + i * 20;
        const u32* pValues = reinterpret_cast<const u32*>(pConst + 4);
        u32 command[6] = {0, Header(REG_VSH_FLOATUNIFORM_INDEX, 0xF, 3, true), 0, 0, 0, 0};
        command[0] = *reinterpret_cast<const u16*>(pConst + 2) & 0xFF;
        command[2] = ((pValues[2] << 8) >> 24) | (pValues[3] << 8);
        command[3] = ((pValues[1] << 8) >> 16) | (pValues[2] << 16);
        command[4] = (pValues[0] & 0xFFFFFF) | (pValues[1] << 24);
        m_CommandCache.Add(command, sizeof(command));
    }

    // the vertex buffer (its physical address / 8 and the offset in 16 bytes)
    const uptr address = nngxGetPhysicalAddr(reinterpret_cast<uptr>(m_pVertexBuffer));
    const u32 alignedAddress = address & ~15;
    const u32 offset = (address - alignedAddress) & 0x0FFFFFFF;
    const u32 baseAddress = (alignedAddress >> 4) << 1;
    {
        const u32 command[7] = {baseAddress, Header(REG_ATTRIBBUFFERS_LOC, 0xF, 38, true), 0x0000000F, 0x00000000, offset, 0x00000000, 0x10100000};
        m_CommandCache.Add(command, sizeof(command));
    }
    m_CommandCache.Add(s_AttributeBufferCommandsRest, sizeof(s_AttributeBufferCommandsRest));

    // the entry point of the shader
    {
        u32 command[6] = {0, Header(0x229, 0x8), 0xA0000000, Header(0x2B9, 0xB), 0, Header(REG_VSH_ENTRYPOINT)};
        command[4] = pDvleWords[2] | 0x7FFF0000;
        m_CommandCache.Add(command, sizeof(command));
    }

    // the output map: the outputs of the DVLE (type, register, component mask) to the semantics
    u32 outMap[7];
    u8 isOutputUsed[7];
    u8 semanticMasks[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (u32 i = 0; i < 7; i++) {
        isOutputUsed[i] = 0;
        outMap[i] = 0x1F1F1F1F;
    }
    const u16* pOutputs = reinterpret_cast<const u16*>(pDvle + pDvleWords[10]);
    for (u32 i = 0; i < (pDvleWords[11] > 7 ? 7 : pDvleWords[11]); i++) {
        const u16* pOutput = pOutputs + i * 4;
        if (pOutput[0] == 9) {
            continue;
        }
        isOutputUsed[pOutput[1]] = 1;
        semanticMasks[pOutput[0]] = static_cast<u8>(pOutput[2]);
        u8* pMap = reinterpret_cast<u8*>(&outMap[pOutput[1]]);
        u8 semantic = s_OutputSemanticBase[pOutput[0]];
        if ((pOutput[2] & 1) != 0) {
            pMap[0] = semantic++;
        }
        if ((pOutput[2] & 2) != 0) {
            pMap[1] = semantic++;
        }
        if ((pOutput[2] & 4) != 0) {
            pMap[2] = semantic++;
        }
        if ((pOutput[2] & 8) != 0) {
            pMap[3] = semantic;
        }
    }
    u32 outputCount = 0;
    for (int i = 0; i < 7; i++) {
        if (static_cast<s8>(isOutputUsed[i]) != 0) {
            outputCount++;
        }
    }
    {
        const u32 command[14] = {
            *reinterpret_cast<const u16*>(pDvle + 18), Header(REG_VSH_OUTMAP_MASK),
            outputCount - 1, Header(0x251),
            0, Header(0x252),
            0, Header(0x242),
            outputCount - 1, Header(0x24A),
            outputCount - 1, Header(0x25E, 0x1),
            outputCount, Header(REG_SH_OUTMAP_TOTAL),
        };
        m_CommandCache.Add(command, sizeof(command));
    }
    {
        const u32 command[2] = {outMap[0], s_OutputMapCommand[1]};
        m_CommandCache.Add(command, sizeof(command));
    }
    m_CommandCache.Add(&outMap[1], sizeof(u32) * 6);

    // the output attribute clock: which semantics there are
    int positionComponents = 0;
    if ((semanticMasks[0] & 1) != 0) {
        positionComponents = 1;
    }
    if ((semanticMasks[0] & 2) != 0) {
        positionComponents++;
    }
    if ((semanticMasks[0] & 4) != 0) {
        positionComponents++;
    }
    if ((semanticMasks[0] & 8) != 0) {
        positionComponents++;
    }
    const u32 clock = (positionComponents >= 3 ? 1 : 0) | ((semanticMasks[2] != 0 ? 1 : 0) << 1) | ((semanticMasks[3] != 0 ? 1 : 0) << 8)
                      | ((semanticMasks[5] != 0 ? 1 : 0) << 9) | ((semanticMasks[6] != 0 ? 1 : 0) << 10)
                      | ((semanticMasks[4] != 0 ? 1 : 0) << 16) | ((semanticMasks[7] != 0 || semanticMasks[1] != 0 ? 1 : 0) << 24);
    {
        const u32 command[4] = {(clock & 0x10700) != 0 ? 1u : 0u, Header(REG_SH_OUTATTR_MODE), clock, Header(REG_SH_OUTATTR_CLOCK)};
        m_CommandCache.Add(command, sizeof(command));
    }
    m_CommandCache.Add(s_StateCommands, sizeof(s_StateCommands));
    m_VertexBufferAddress = alignedAddress;

    // the draw end: the vertex buffer again, the end state
    {
        const u32 command[7] = {baseAddress, Header(REG_ATTRIBBUFFERS_LOC, 0xF, 38, true), 0x0000000F, 0x00000000, offset, 0x00000000, 0x10100000};
        m_DrawCommandCache.Add(command, sizeof(command));
    }
    m_DrawCommandCache.Add(s_AttributeBufferCommandsRest, sizeof(s_AttributeBufferCommandsRest));
    m_DrawCommandCache.Add(s_DrawEndCommands, sizeof(s_DrawEndCommands));

    // the uniform blocks
    m_ProjectionUniforms.index = UniformIndex32(0);
    m_ProjectionUniforms.indexHeader = Header(REG_VSH_FLOATUNIFORM_INDEX);
    m_ProjectionUniforms.data[4][0] = 0.0f;
    m_ProjectionUniforms.data[4][1] = 0.0f;
    m_ProjectionUniforms.data[4][2] = 0.0f;
    m_ProjectionUniforms.data[4][3] = 0.0f;
    m_VertexIndexUniforms.index = UniformIndex32(6);
    m_VertexIndexUniforms.indexHeader = Header(REG_VSH_FLOATUNIFORM_INDEX);
    m_MtxUniforms.index = UniformIndex32(32);
    m_MtxUniforms.indexHeader = Header(REG_VSH_FLOATUNIFORM_INDEX);
    m_TexCoordUniforms.index = UniformIndex32(64);
    m_TexCoordUniforms.indexHeader = Header(REG_VSH_FLOATUNIFORM_INDEX);
}

// 0x004D7FB0 | libgarden [tier A]
u32* nw::font::RectDrawer::SetProjectionMtx(u32* pCommand, const nn::math::MTX44& mtx)
{
    nn::math::ARMv6::MTX44CopyAsm(reinterpret_cast<nn::math::MTX44*>(m_ProjectionUniforms.data), &mtx);
    m_ProjectionUniforms.firstData = *reinterpret_cast<u32*>(&m_ProjectionUniforms.data[0][0]);
    *reinterpret_cast<u32*>(&m_ProjectionUniforms.data[0][0]) = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, 19);
    m_ProjectionUniforms.padding = 0;
    std::memcpy(pCommand, &m_ProjectionUniforms, sizeof(m_ProjectionUniforms));
    return pCommand + sizeof(m_ProjectionUniforms) / sizeof(u32);
}

// 0x004D8000 (name is ours)
void nw::font::RectDrawer::FlushUniforms(DispStringBuffer* pBuffer)
{
    // the index vectors of the characters
    const u32 charCount = m_VertexIndexCount;
    const u32 vertexCount = charCount * 6;
    m_VertexIndexUniforms.firstData = *reinterpret_cast<u32*>(&m_VertexIndexUniforms.data[0][0]);
    *reinterpret_cast<u32*>(&m_VertexIndexUniforms.data[0][0]) = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, charCount * 4 - 1);
    *reinterpret_cast<u32*>(&m_VertexIndexUniforms.data[charCount][0]) = 0;
    AddCommands(pBuffer, &m_VertexIndexUniforms, charCount * 4 * sizeof(u32) + 16);
    m_VertexIndexCount = 0;

    // the text colors (c35 - c38 are set by CharWriter::UseCommandBuffer: the first block of a
    // string starts at c39)
    if (m_IsColorUniformsSet) {
        const u32 colorCount = m_ColorCount;
        m_ColorUniforms.firstData = *reinterpret_cast<u32*>(&m_ColorUniforms.data[0][0]);
        *reinterpret_cast<u32*>(&m_ColorUniforms.data[0][0]) = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, colorCount * 4 - 1);
        *reinterpret_cast<u32*>(&m_ColorUniforms.data[colorCount][0]) = 0;
        const u32 size = colorCount * 4 * sizeof(u32) + 16;
        if (pBuffer->m_Unknown0x24 != 0) {
            AddCommands(pBuffer, &m_ColorUniforms, size);
        } else {
            pBuffer->m_Unknown0x24 = pBuffer->m_CommandSize;
            if (m_ColorCount > 4) {
                AddCommand(pBuffer, UniformIndex32(39), Header(REG_VSH_FLOATUNIFORM_INDEX));
                AddCommand(pBuffer, *reinterpret_cast<u32*>(&m_ColorUniforms.data[4][0]),
                           Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, (m_ColorCount - 4) * 4 - 1));
                AddCommands(pBuffer, &m_ColorUniforms.data[4][1], size - 80);
            }
        }
        // the last four colors stay as the first ones
        const u8 rest = m_ColorCount - 4;
        if (rest != 0) {
            f32 (*pColors)[4] = m_pColorData;
            std::memcpy(pColors[0], pColors[rest], sizeof(pColors[0]));
            std::memcpy(m_pColorData[1], m_pColorData[rest + 1], sizeof(pColors[0]));
            std::memcpy(m_pColorData[2], m_pColorData[rest + 2], sizeof(pColors[0]));
            std::memcpy(m_pColorData[3], m_pColorData[rest + 3], sizeof(pColors[0]));
            m_IsColorUniformsSet = true;
        } else {
            m_IsColorUniformsSet = false;
            *reinterpret_cast<u32*>(&m_ColorUniforms.data[0][0]) = m_ColorUniforms.firstData;
        }
        m_VertexIndex[3] = 3.0f;
        m_ColorCount = 4;
    }

    // the texture coordinates
    if (m_TexCoordCount != 0) {
        const u32 count = m_TexCoordCount;
        m_TexCoordUniforms.firstData = *reinterpret_cast<u32*>(&m_TexCoordUniforms.data[0][0]);
        *reinterpret_cast<u32*>(&m_TexCoordUniforms.data[0][0]) = Header(REG_VSH_FLOATUNIFORM_DATA, 0xF, count * 4 - 1);
        *reinterpret_cast<u32*>(&m_TexCoordUniforms.data[count][0]) = 0;
        AddCommands(pBuffer, &m_TexCoordUniforms, count * 4 * sizeof(u32) + 16);
        m_TexCoordCount = 0;
    }

    // the draw call
    u32* pCommand = pBuffer->m_pCommandBuffer + pBuffer->m_CommandSize;
    *pCommand++ = 0x80000000 | GetVertexIndexAddressOffset(vertexCount);
    *pCommand++ = Header(REG_INDEXBUFFER_CONFIG);
    *pCommand++ = vertexCount;
    *pCommand++ = Header(REG_NUMVERTICES);
    pBuffer->m_CommandSize = pCommand - pBuffer->m_pCommandBuffer;
    AddCommands(pBuffer, s_DrawCommands, sizeof(s_DrawCommands));
}

// 0x004D7890 | libgarden [tier A]
void nw::font::RectDrawer::BuildTextCommand(nw::font::CharWriter* pWriter)
{
    DispStringBuffer* pBuffer = pWriter->m_pDispStringBuffer;
    u32 charCount = pBuffer->m_CharCount;
    if (charCount > pBuffer->m_DrawCharCount) {
        charCount = pBuffer->m_DrawCharCount;
    }
    pBuffer->m_CommandSize = 0;
    pBuffer->m_Unknown0x24 = 0;
    pBuffer->m_IsCommandGenerated = true;
    if (charCount == 0) {
        return;
    }

    // the state and the first colors
    m_VertexIndex[0] = 0.0f;
    m_IsColorUniformsSet = false;
    m_ColorUniforms.index = UniformIndex32(35);
    m_ColorUniforms.indexHeader = Header(REG_VSH_FLOATUNIFORM_INDEX);
    AddCommands(pBuffer, s_TextStateCommands, sizeof(s_TextStateCommands));
    ut::Color8 textColors[2];
    textColors[0] = pWriter->m_TextColors[0];
    textColors[1] = pWriter->m_TextColors[1];
    internal::GetVertexColors(&m_pColorData[m_ColorCount], textColors, pWriter->m_GradationMode);
    m_VertexIndex[3] = static_cast<f32>(static_cast<u32>(m_ColorCount + 3));
    m_ColorCount += 4;
    m_IsColorUniformsSet = true;

    // a bit per character: drawn
    u8* pFlags = pBuffer->m_pCharFlags;
    for (int i = (charCount + 7) / 8; i > 0; i--) {
        pFlags[i - 1] = 0;
    }

    // per texture: the characters of the texture
    u32 imageAddress = 0;
    u32 textureSize = 0;
    int textureFormat = -1;
    u32 wrapFilter = 0xFFFFFFFF;
    int nextIndex = 0;
    u32 drawnCount = 0;
    while (drawnCount < charCount) {
        int first = nextIndex;
        while (((pFlags[first / 8] >> (first % 8)) & 1) != 0) {
            first++;
        }
        nextIndex = first + 1;
        const DispStringBuffer::CharAttribute* pFirst = &pBuffer->GetCharAttributes()[first];
        if (imageAddress != pFirst->pTextureObject->m_ImageAddress) {
            imageAddress = pFirst->pTextureObject->m_ImageAddress;
            if (drawnCount != 0 && m_VertexIndexCount != 0) {
                FlushUniforms(pBuffer);
            }
            const int format = pFirst->pTextureObject->m_Format;
            if (format != textureFormat) {
                const bool isAlpha = IsAlphaFormat(format);
                if (textureFormat == -1) {
                    if (isAlpha) {
                        AddCommands(pBuffer, s_AlphaTextureCombinerSetupCommands, sizeof(s_AlphaTextureCombinerSetupCommands));
                    } else {
                        AddCommands(pBuffer, s_ColorTextureCombinerSetupCommands, sizeof(s_ColorTextureCombinerSetupCommands));
                    }
                } else if (IsAlphaFormat(textureFormat) != isAlpha) {
                    if (isAlpha) {
                        AddCommands(pBuffer, s_AlphaTextureCombinerCommands, sizeof(s_AlphaTextureCombinerCommands));
                    } else {
                        AddCommands(pBuffer, s_ColorTextureCombinerCommands, sizeof(s_ColorTextureCombinerCommands));
                    }
                }
                AddCommands(pBuffer, s_TextureUnitResetCommand, sizeof(s_TextureUnitResetCommand));
                textureFormat = format;
                AddCommand(pBuffer, format, Header(REG_TEXUNIT0_TYPE));
            } else {
                AddCommands(pBuffer, s_TextureUnitResetCommand, sizeof(s_TextureUnitResetCommand));
            }
            AddCommand(pBuffer, 0x00011001, Header(REG_TEXUNIT_CONFIG, 0x4));
            AddCommand(pBuffer, imageAddress, Header(REG_TEXUNIT0_ADDR1));
            const u32 size = *reinterpret_cast<const u32*>(&pFirst->pTextureObject->m_Height);
            if (size != textureSize) {
                textureSize = size;
                AddCommand(pBuffer, size, Header(REG_TEXUNIT0_DIM));
            }
            const u32 filter = pFirst->pTextureObject->m_pFont->GetTextureWrapFilterValue();
            if (filter != wrapFilter) {
                wrapFilter = filter;
                AddCommand(pBuffer, filter, Header(REG_TEXUNIT0_PARAM));
            }
        }

        for (u32 i = first; i < charCount; i++) {
            u8* pFlag = &pFlags[static_cast<int>(i) / 8];
            const int bit = static_cast<int>(i) % 8;
            if (((*pFlag >> bit) & 1) != 0) {
                continue;
            }
            const DispStringBuffer::CharAttribute* pChar = &pBuffer->GetCharAttributes()[i];
            if (pChar->pTextureObject->m_ImageAddress != imageAddress) {
                continue;
            }
            if (textColors[0].ToU32() != pChar->color[0].ToU32() || textColors[1].ToU32() != pChar->color[1].ToU32()) {
                textColors[0] = pChar->color[0];
                textColors[1] = pChar->color[1];
                internal::GetVertexColors(&m_pColorData[m_ColorCount], textColors, pWriter->m_GradationMode);
                m_VertexIndex[3] = static_cast<f32>(static_cast<u32>(m_ColorCount + 3));
                m_ColorCount += 4;
                m_IsColorUniformsSet = true;
            }
            // the texture rectangle and the position rectangle
            m_VertexIndex[2] = static_cast<f32>(m_TexCoordCount);
            std::memcpy(m_pTexCoordData[m_TexCoordCount++], &pChar->texLeft, sizeof(f32) * 4);
            m_VertexIndex[1] = static_cast<f32>(m_TexCoordCount);
            std::memcpy(m_pTexCoordData[m_TexCoordCount++], &pChar->width, sizeof(f32) * 4);
            *pFlag = (*pFlag & ~(1 << bit)) | (1 << bit);
            std::memcpy(m_pVertexIndexData[m_VertexIndexCount], m_VertexIndex, sizeof(m_VertexIndex));
            drawnCount++;
            m_VertexIndexCount++;
            if (m_VertexIndexCount > VERTEX_INDEX_MAX - 1 || m_TexCoordCount > TEXCOORD_MAX - 2 || m_ColorCount > COLOR_MAX - 4) {
                FlushUniforms(pBuffer);
            }
        }
    }
    if (m_VertexIndexCount != 0) {
        FlushUniforms(pBuffer);
    }
    m_ColorCount = 0;

    // a subroutine ends with a kick (the end 16 byte aligned)
    if ((pBuffer->m_Flags & DispStringBuffer::FLAG_SUBROUTINE) != 0) {
        u32* pCommand = pBuffer->m_pCommandBuffer + pBuffer->m_CommandSize;
        const u32 rest = (8 + pBuffer->m_CommandSize * sizeof(u32)) & 15;
        if (rest != 0) {
            const int count = 4 - (rest >> 2);
            for (int i = 0; i < count; i++) {
                *pCommand++ = 0;
            }
        }
        pCommand = nn::gr::CTR::MakeChannelKickCommand(pCommand, nn::gr::CTR::COMMAND_BUFFER_CHANNEL_1);
        pBuffer->m_CommandSize = pCommand - pBuffer->m_pCommandBuffer;
        nngxlowStoreDataCache(reinterpret_cast<uptr>(pBuffer->m_pCommandBuffer), pBuffer->m_CommandSize * sizeof(u32));
    }
}

// 0x004D8330 | mk7dlp:bytes [tier B]
const f32* nw::font::RectDrawer::GetVertexBufferData()
{
    if (!s_IsVertexBufferBuilt) {
        for (int i = 0; i < VERTEX_INDEX_MAX; i++) {
            for (int j = 0; j < 6; j++) {
                const int id = s_CharVertexIds[j];
                f32* pVertex = s_VertexBuffer[i * 6 + j];
                pVertex[0] = static_cast<f32>(i);
                pVertex[1] = static_cast<f32>(id);
                pVertex[2] = (id & 1) != 0 ? 1.0f : 0.0f;
                pVertex[3] = static_cast<u32>(id + 1) >= 3 ? -1.0f : 0.0f;
            }
        }
        s_IsVertexBufferBuilt = true;
    }
    return &s_VertexBuffer[0][0];
}

// 0x004D8404 (name is ours)
u32 nw::font::RectDrawer::GetCommandBufferSize(const void* pShaderBinary)
{
    const u32* pDvlp = GetDvlp(pShaderBinary);
    const u32* pDvle = reinterpret_cast<const u32*>(GetDvle(pShaderBinary));
    return CalcShaderCommandSize(pDvlp, pDvle) * sizeof(u32) + 776 + 216;
}

// 0x00744EB8 | nintendogs:callseq-callee [tier A]
const u32* nw::font::RectDrawer::GetDrawCommands()
{
    return s_DrawCommands;
}

// 0x00744EC4 | nintendogs:bytes [tier A]
u32 nw::font::RectDrawer::GetVertexIndexAddressOffset(u32 vertexCount) const
{
    const u32 offset = vertexCount > 16 ? vertexCount * 2 - 32 : 0;
    return ((m_VertexBufferAddress + offset) & 0xFFF) >= 0xFE0 ? 32 : 0;
}

} // namespace font
} // namespace nw
