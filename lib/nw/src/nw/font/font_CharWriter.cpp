#include <cstring>
#include <new>

#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gx/gx_Api.h"
#include "nw/font/font_CharWriter.h"
#include "nw/font/font_Font.h"
#include "nw/font/font_Glyph.h"
#include "nw/font/font_RectDrawer.h"
#include "nw/font/font_TextWriterResource.h"
#include "nw/font/internal/font_TextureObject.h"
#include "nw/font/internal/internal_Api.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace font {
namespace {
// the values of the OpenGL ES API and its DMP extensions (names are ours)
const GLenum TEXTURE_2D = 0x0DE1;
const GLenum TEXTURE0 = 0x84C0;
const GLenum TRIANGLE_FAN = 0x0006;
const GLenum BLEND = 0x0BE2;
const GLenum COLOR_LOGIC_OP = 0x0BF2;
const GLenum SRC_ALPHA = 0x0302;
const GLenum ONE_MINUS_SRC_ALPHA = 0x0303;
const GLenum FUNC_ADD = 0x8006;
const GLenum GREATER = 0x0204;
const GLenum ARRAY_BUFFER = 0x8892;
const GLenum ELEMENT_ARRAY_BUFFER = 0x8893;
const GLenum TYPE_UNSIGNED_BYTE = 0x1401;
const GLenum TYPE_FLOAT = 0x1406;
const GLint COMBINE_REPLACE = 0x1E01;
const GLint COMBINE_MODULATE = 0x2100;
const GLint COMBINE_MULT_ADD = 0x6401;
// the sources and operands of dmp_TexEnv
const GLint SRC_TEXTURE0 = 0x84C0;
const GLint SRC_CONSTANT = 0x8576;
const GLint SRC_PRIMARY_COLOR = 0x8577;
const GLint SRC_PREVIOUS = 0x8578;
const GLint OPERAND_SRC_COLOR = 0x0300;
const GLint OPERAND_ONE_MINUS_SRC_COLOR = 0x0301;
const GLint OPERAND_SRC_ALPHA = 0x0302;
const GLint OPERAND_ONE_MINUS_SRC_ALPHA = 0x0303;

// the texture combiners of the drawing: without color mapping one stage (TexEnv[5]), with color
// mapping three (the texture maps black to the min color and white to the max color)
// 0x008A3918 (name is ours)
const GLint s_SrcSimple[3] = {SRC_PRIMARY_COLOR, SRC_TEXTURE0, SRC_CONSTANT};
// 0x008A3924 (name is ours)
const GLint s_OperandRgbSimple[3] = {OPERAND_SRC_COLOR, OPERAND_SRC_COLOR, OPERAND_SRC_COLOR};
// 0x008A3930 (name is ours)
const GLint s_OperandAlphaSimple[3] = {OPERAND_SRC_ALPHA, OPERAND_SRC_ALPHA, OPERAND_SRC_ALPHA};
// 0x008A393C (name is ours)
const GLint s_SrcMappingMax[3] = {SRC_TEXTURE0, SRC_CONSTANT, SRC_CONSTANT};
// 0x008A3948 (name is ours)
const GLint s_OperandRgbColor[3] = {OPERAND_SRC_COLOR, OPERAND_SRC_COLOR, OPERAND_SRC_COLOR};
// 0x008A3954 (name is ours)
const GLint s_OperandRgbInvColor[3] = {OPERAND_ONE_MINUS_SRC_COLOR, OPERAND_SRC_COLOR, OPERAND_SRC_COLOR};
// 0x008A3960 (name is ours)
const GLint s_OperandAlpha[3] = {OPERAND_SRC_ALPHA, OPERAND_SRC_ALPHA, OPERAND_SRC_ALPHA};
// 0x008A396C (name is ours)
const GLint s_SrcMappingMin[3] = {SRC_TEXTURE0, SRC_CONSTANT, SRC_PREVIOUS};
// 0x008A3978 (name is ours)
const GLint s_OperandAlphaMappingMin[3] = {OPERAND_ONE_MINUS_SRC_ALPHA, OPERAND_SRC_ALPHA, OPERAND_SRC_ALPHA};
// 0x008A3984 (name is ours)
const GLint s_SrcVertexColor[3] = {SRC_PRIMARY_COLOR, SRC_PREVIOUS, SRC_PREVIOUS};

// the GPU commands of the text colors: the vertex shader float uniform c35 - c38 (registers after
// 3dbrew "GPU/Internal Registers")
const u32 UNIFORM_INDEX_TEXT_COLOR = 0x80000000 | 35;
const u32 HEADER_VSH_FLOAT_UNIFORM_INDEX = 0x000F02C0;
const u32 HEADER_VSH_FLOAT_UNIFORM_DATA_16 = 0x00FF02C1;
// the constant colors of TexEnv[3] and TexEnv[4]
const u32 HEADER_TEXENV3_COLOR = 0x000F00DB;
const u32 HEADER_TEXENV4_COLOR = 0x000F00F3;

// the color as 0xAABBGGRR with the alpha scaled by alpha / 255 (inline; name is ours)
inline u32 ToPicaColor(const ut::Color8& color, u8 alpha)
{
    return color.r | (color.g << 8) | (color.b << 16) | ((color.a * alpha / 255) << 24);
}
// the color as floats with the alpha scaled by alpha / 255 (inline; name is ours)
inline ut::FloatColor ToFloatColor(const ut::Color8& color, u8 alpha)
{
    const f32 scale = 1.0f / 255.0f;
    ut::FloatColor floatColor;
    floatColor.r = color.r * scale;
    floatColor.g = color.g * scale;
    floatColor.b = color.b * scale;
    floatColor.a = color.a * scale * (alpha * scale);
    return floatColor;
}

// the texture combiner stage from the constants (inline; name is ours)
inline void SetTexEnv(const TextWriterResource* pResource, int stage, const GLint* pSrc, const GLint* pOperandRgb, const GLint* pOperandAlpha,
                      GLint combine)
{
    glUniform3iv(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_SRC_RGB), 1, pSrc);
    glUniform3iv(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_SRC_ALPHA), 1, pSrc);
    glUniform3iv(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_OPERAND_RGB), 1, pOperandRgb);
    glUniform3iv(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_OPERAND_ALPHA), 1, pOperandAlpha);
    glUniform1i(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_COMBINE_RGB), combine);
    glUniform1i(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_COMBINE_ALPHA), combine);
    glUniform1f(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_SCALE_RGB), 1.0f);
    glUniform1f(pResource->GetTexEnvUniformLocation(stage, TextWriterResource::TEXENV_SCALE_ALPHA), 1.0f);
}
} // namespace

namespace internal {
// 0x004D9210 (name is ours)
void GetVertexColors(f32 (*pVertexColors)[4], const ut::Color8* pTextColors, u8 gradationMode)
{
    struct Color
    {
        f32 rgba[4];
    };
    Color colors[2];
    for (int i = 0; i < 2; i++) {
        const f32 scale = 1.0f / 255.0f;
        colors[i].rgba[0] = pTextColors[i].r * scale;
        colors[i].rgba[1] = pTextColors[i].g * scale;
        colors[i].rgba[2] = pTextColors[i].b * scale;
        colors[i].rgba[3] = pTextColors[i].a * scale;
    }
    Color* pColors = reinterpret_cast<Color*>(pVertexColors);
    pColors[0] = colors[0];
    pColors[1] = colors[gradationMode == CharWriter::GRADATION_MODE_H];
    pColors[2] = colors[gradationMode == CharWriter::GRADATION_MODE_V];
    pColors[3] = colors[gradationMode != CharWriter::GRADATION_MODE_NONE];
}
} // namespace internal

// 0x004D6E38 | nintendogs:callgraph [tier A]
nw::font::CharWriter::CharWriter()
    : m_FixedWidth(0.0f), m_pFont(nullptr), m_pTextWriterResource(nullptr), m_pDispStringBuffer(nullptr), m_IsWidthFixed(false), m_Alpha(0xFF)
{
    m_ColorMappingMin = ut::Color8(0, 0, 0, 0);
    m_ColorMappingMax = ut::Color8();
    m_GradationMode = GRADATION_MODE_NONE;
    m_TextColors[0] = ut::Color8();
    m_VertexColors[VERTEX_LT] = m_TextColors[0];
    m_VertexColors[VERTEX_RT] = m_TextColors[m_GradationMode == GRADATION_MODE_H];
    m_VertexColors[VERTEX_LB] = m_TextColors[m_GradationMode == GRADATION_MODE_V];
    m_VertexColors[VERTEX_RB] = m_TextColors[m_GradationMode != GRADATION_MODE_NONE];
    m_ScaleH = 1.0f;
    m_ScaleV = 1.0f;
    m_CursorX = 0.0f;
    m_CursorY = 0.0f;
    m_CursorZ = 0.0f;
}

// 0x004D64D0 | libgarden [tier A]
void nw::font::CharWriter::SetFontSize(f32 width, f32 height)
{
    m_ScaleH = width / m_pFont->GetWidth();
    m_ScaleV = height / m_pFont->GetHeight();
}

// 0x00744E34 | nintendogs:bytes [tier A]
f32 nw::font::CharWriter::GetFontWidth() const
{
    return m_pFont->GetWidth() * m_ScaleH;
}

// 0x00744E8C | nintendogs:bytes [tier A]
f32 nw::font::CharWriter::GetFontHeight() const
{
    return m_pFont->GetHeight() * m_ScaleV;
}

// 0x00744E60 | nintendogs:bytes [tier A]
f32 nw::font::CharWriter::GetFontAscent() const
{
    return m_pFont->GetAscent() * m_ScaleV;
}

// 0x004D6738 | libgarden [tier A]
void nw::font::CharWriter::UpdateVertexColors()
{
    m_VertexColors[VERTEX_LT] = m_TextColors[0];
    m_VertexColors[VERTEX_RT] = m_TextColors[m_GradationMode == GRADATION_MODE_H];
    m_VertexColors[VERTEX_LB] = m_TextColors[m_GradationMode == GRADATION_MODE_V];
    m_VertexColors[VERTEX_RB] = m_TextColors[m_GradationMode != GRADATION_MODE_NONE];
}

// 0x004D67D4 (name is ours)
void nw::font::CharWriter::SetupGXWithColorMapping(bool isAlphaTexture)
{
    TextWriterResource* pResource = m_pTextWriterResource;

    // TexEnv[3]: the texture times the max color (an alpha texture: inverted)
    SetTexEnv(pResource, 3, s_SrcMappingMax, isAlphaTexture ? s_OperandRgbInvColor : s_OperandRgbColor, s_OperandAlpha, COMBINE_MODULATE);
    ut::FloatColor maxColor = ToFloatColor(m_ColorMappingMax, m_Alpha);
    glUniform4fv(pResource->GetTexEnvUniformLocation(3, TextWriterResource::TEXENV_CONST_RGBA), 1, &maxColor.r);

    // TexEnv[4]: plus the inverted texture times the min color
    SetTexEnv(pResource, 4, s_SrcMappingMin, isAlphaTexture ? s_OperandRgbColor : s_OperandRgbInvColor, s_OperandAlphaMappingMin, COMBINE_MULT_ADD);
    ut::FloatColor minColor = ToFloatColor(m_ColorMappingMin, m_Alpha);
    glUniform4fv(pResource->GetTexEnvUniformLocation(4, TextWriterResource::TEXENV_CONST_RGBA), 1, &minColor.r);

    // TexEnv[5]: times the vertex color
    SetTexEnv(pResource, 5, s_SrcVertexColor, s_OperandRgbColor, s_OperandAlpha, COMBINE_MODULATE);
}

// 0x004D6B78 (name is ours)
void nw::font::CharWriter::SetupGX()
{
    TextWriterResource* pResource = m_pTextWriterResource;
    pResource->m_pLastTexture = nullptr;
    glUniform1i(pResource->GetUniformLocation(TextWriterResource::UNIFORM_FRAGMENT_LIGHTING), 0);
    glUniform1i(pResource->GetUniformLocation(TextWriterResource::UNIFORM_TEXTURE0_SAMPLER_TYPE), TEXTURE_2D);
    glActiveTexture(TEXTURE0);
    glUniform1i(pResource->GetUniformLocation(TextWriterResource::UNIFORM_FOG_MODE), 0);
    glUniform1i(pResource->GetUniformLocation(TextWriterResource::UNIFORM_ALPHA_TEST_ENABLE), 1);
    glUniform1f(pResource->GetUniformLocation(TextWriterResource::UNIFORM_ALPHA_TEST_REF), 0.0f);
    glUniform1i(pResource->GetUniformLocation(TextWriterResource::UNIFORM_ALPHA_TEST_FUNC), GREATER);
    glEnable(BLEND);
    glBlendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
    glBlendEquation(FUNC_ADD);
    glDisable(COLOR_LOGIC_OP);

    // an alpha texture (A8, A4) gives the shape, the color is the vertex color
    bool isAlphaTexture = false;
    if (m_pFont != nullptr) {
        int format = m_pFont->GetTextureFormat();
        isAlphaTexture = format == FONT_SHEET_FORMAT_A8 || format == FONT_SHEET_FORMAT_A4;
    }
    if (m_Alpha == 0xFF && m_ColorMappingMin.ToU32() == 0x00000000 && m_ColorMappingMax.ToU32() == 0xFFFFFFFF) {
        glUniform3iv(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_SRC_RGB), 1, s_SrcSimple);
        glUniform3iv(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_SRC_ALPHA), 1, s_SrcSimple);
        glUniform3iv(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_OPERAND_RGB), 1, s_OperandRgbSimple);
        glUniform3iv(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_OPERAND_ALPHA), 1, s_OperandAlphaSimple);
        glUniform1i(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_COMBINE_RGB), isAlphaTexture ? COMBINE_REPLACE : COMBINE_MODULATE);
        glUniform1i(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_COMBINE_ALPHA), COMBINE_MODULATE);
        glUniform1f(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_SCALE_RGB), 1.0f);
        glUniform1f(pResource->GetTexEnvUniformLocation(5, TextWriterResource::TEXENV_SCALE_ALPHA), 1.0f);
    } else {
        SetupGXWithColorMapping(isAlphaTexture);
    }

    // the vertices of TextWriterResource
    glBindBuffer(ARRAY_BUFFER, 0);
    glBindBuffer(ELEMENT_ARRAY_BUFFER, 0);
    const TextWriterResource::Vertex* pVertices = pResource->m_Vertices;
    const GLsizei stride = sizeof(TextWriterResource::Vertex);
    glEnableVertexAttribArray(TextWriterResource::ATTRIBUTE_POSITION);
    glVertexAttribPointer(TextWriterResource::ATTRIBUTE_POSITION, 2, TYPE_FLOAT, 0, stride, &pVertices->x);
    glDisableVertexAttribArray(TextWriterResource::ATTRIBUTE_POSITION_Z);
    m_pTextWriterResource->SetPositionZ(m_CursorZ);
    glEnableVertexAttribArray(TextWriterResource::ATTRIBUTE_COLOR);
    glVertexAttribPointer(TextWriterResource::ATTRIBUTE_COLOR, 4, TYPE_UNSIGNED_BYTE, 0, stride, &pVertices->color);
    glEnableVertexAttribArray(TextWriterResource::ATTRIBUTE_TEXCOORD);
    glVertexAttribPointer(TextWriterResource::ATTRIBUTE_TEXCOORD, 2, TYPE_FLOAT, 0, stride, &pVertices->u);
}

// 0x004D64B4 | libgarden [tier A]
void nw::font::CharWriter::StartPrint()
{
    m_pDispStringBuffer->m_CharCount = 0;
    m_pDispStringBuffer->m_IsCommandGenerated = false;
    m_pDispStringBuffer->m_CommandSize = 0;
}

// 0x004D61CC | nintendogs:bytes-fuzzy [tier A]
void nw::font::CharWriter::PrintGlyph(f32 x, const nw::font::Glyph& glyph)
{
    // the cell on the sheet (the texture is upside down)
    const f32 texLeft = static_cast<f32>(glyph.m_CellX) / glyph.m_TexWidth;
    const f32 texRight = static_cast<f32>(glyph.m_CellX + glyph.m_Widths.glyphWidth) / glyph.m_TexWidth;
    const f32 texTop = static_cast<f32>(glyph.m_TexHeight - glyph.m_CellY) / glyph.m_TexHeight;
    const f32 texBottom = static_cast<f32>(glyph.m_TexHeight - (glyph.m_CellY + glyph.m_Height)) / glyph.m_TexHeight;
    const f32 y = m_CursorY;

    if (m_pDispStringBuffer != nullptr) {
        const f32 width = glyph.m_Widths.glyphWidth * m_ScaleH;
        const f32 height = -glyph.m_Height * m_ScaleV;
        DispStringBuffer* pBuffer = m_pDispStringBuffer;
        if (pBuffer->m_CharCount < pBuffer->m_CharCountMax) {
            DispStringBuffer::CharAttribute& attr = m_pDispStringBuffer->GetCharAttributes()[pBuffer->m_CharCount++];
            attr.width = width;
            attr.height = height;
            attr.x = x;
            attr.y = y;
            attr.color[0] = m_TextColors[0];
            attr.color[1] = m_TextColors[1];
            attr.texLeft = texLeft;
            attr.texTop = texTop;
            attr.texRight = texRight;
            attr.texBottom = texBottom;
            attr.pTextureObject = glyph.m_pTextureObject;
        }
        return;
    }

    TextWriterResource* pResource = m_pTextWriterResource;
    const f32 right = x + glyph.m_Widths.glyphWidth * m_ScaleH;
    const f32 bottom = y + glyph.m_Height * m_ScaleV;
    TextWriterResource::Vertex* pVertices = pResource->m_Vertices;
    pVertices[VERTEX_RT].x = right;
    pVertices[VERTEX_RT].y = y;
    pVertices[VERTEX_LT].x = x;
    pVertices[VERTEX_LT].y = y;
    pVertices[VERTEX_LB].x = x;
    pVertices[VERTEX_LB].y = bottom;
    pVertices[VERTEX_RB].x = right;
    pVertices[VERTEX_RB].y = bottom;
    pVertices[VERTEX_RT].color = m_VertexColors[VERTEX_RT];
    pVertices[VERTEX_LT].color = m_VertexColors[VERTEX_LT];
    pVertices[VERTEX_LB].color = m_VertexColors[VERTEX_LB];
    pVertices[VERTEX_RB].color = m_VertexColors[VERTEX_RB];
    pVertices[VERTEX_RT].u = texRight;
    pVertices[VERTEX_RT].v = texTop;
    pVertices[VERTEX_LT].u = texLeft;
    pVertices[VERTEX_LT].v = texTop;
    pVertices[VERTEX_LB].u = texLeft;
    pVertices[VERTEX_LB].v = texBottom;
    pVertices[VERTEX_RB].u = texRight;
    pVertices[VERTEX_RB].v = texBottom;

    // the texture: the one of the sheet, or the texture of the resource with the sheet image
    GLuint textureName = 0;
    bool isLoad;
    internal::TextureObject* pObject = glyph.m_pTextureObject;
    if (pObject != nullptr) {
        textureName = pObject->m_Name;
        if (textureName == 0) {
            glGenTextures(1, &textureName);
            pObject->m_Name = textureName;
            glBindTexture(TEXTURE_2D, textureName);
            isLoad = true;
        } else {
            glBindTexture(TEXTURE_2D, textureName);
            isLoad = glyph.m_IsSheetUpdated;
        }
    } else {
        textureName = pResource->m_TextureId;
        const void* pLastTexture = pResource->m_pLastTexture;
        pResource->m_pLastTexture = glyph.m_pTexture;
        isLoad = pLastTexture != glyph.m_pTexture;
        glBindTexture(TEXTURE_2D, textureName);
    }
    if (isLoad) {
        internal::LoadTexture(glyph.m_TexWidth, glyph.m_TexHeight, glyph.m_TexFormat, glyph.m_pTexture, m_pFont->IsLinearFilterEnableAtSmall(),
                              m_pFont->IsLinearFilterEnableAtLarge());
    }
    if (m_pTextWriterResource->m_PositionZ != m_CursorZ) {
        m_pTextWriterResource->SetPositionZ(m_CursorZ);
    }
    glDrawArrays(TRIANGLE_FAN, 0, TextWriterResource::VERTEX_COUNT);
}

// 0x004D6AA8 | nintendogs:bytes [tier A]
f32 nw::font::CharWriter::Print(u16 code)
{
    Glyph glyph;
    m_pFont->GetGlyph(&glyph, code);
    f32 width;
    f32 offset;
    if (m_IsWidthFixed) {
        // centered in the fixed width
        width = m_FixedWidth;
        offset = (width - glyph.m_Widths.charWidth * m_ScaleH) * 0.5f + glyph.m_Widths.left * m_ScaleH;
    } else {
        width = glyph.m_Widths.charWidth * m_ScaleH;
        offset = glyph.m_Widths.left * m_ScaleH;
    }
    PrintGlyph(m_CursorX + offset, glyph);
    m_CursorX += width;
    return width;
}

// 0x004D652C | libgarden [tier A]
u32* nw::font::CharWriter::UseCommandBuffer(u32* pCommand, nw::font::RectDrawer* pDrawer)
{
    if (m_pDispStringBuffer->m_CommandSize == 0) {
        return pCommand;
    }

    // the text colors as vertex shader uniforms (four colors, sixteen words, one word of padding)
    f32 vertexColors[VERTEX_COUNT][4];
    internal::GetVertexColors(vertexColors, m_TextColors, m_GradationMode);
    const u32* pColorWords = reinterpret_cast<const u32*>(vertexColors);
    *pCommand++ = UNIFORM_INDEX_TEXT_COLOR;
    *pCommand++ = HEADER_VSH_FLOAT_UNIFORM_INDEX;
    *pCommand++ = pColorWords[0];
    *pCommand++ = HEADER_VSH_FLOAT_UNIFORM_DATA_16;
    std::memcpy(pCommand, pColorWords + 1, sizeof(vertexColors) - sizeof(u32));
    pCommand += sizeof(vertexColors) / sizeof(u32) - 1;
    *pCommand++ = 0;

    // the color mapping
    *pCommand++ = ToPicaColor(m_ColorMappingMax, m_Alpha);
    *pCommand++ = HEADER_TEXENV3_COLOR;
    *pCommand++ = ToPicaColor(m_ColorMappingMin, m_Alpha);
    *pCommand++ = HEADER_TEXENV4_COLOR;

    DispStringBuffer* pBuffer = m_pDispStringBuffer;
    if ((pBuffer->m_Flags & DispStringBuffer::FLAG_SUBROUTINE) == 0) {
        // the commands are copied
        std::memcpy(pCommand, pBuffer->m_pCommandBuffer, pBuffer->m_CommandSize * sizeof(u32));
        return pCommand + m_pDispStringBuffer->m_CommandSize;
    }

    // a call of the commands as a subroutine: the subroutine command (six words) ends 16 byte
    // aligned
    const u32 rest = (reinterpret_cast<uptr>(pCommand) + 24) & 15;
    if (rest != 0) {
        const int count = 4 - (rest >> 2);
        for (int i = 0; i < count; i++) {
            *pCommand++ = 0;
        }
    }
    u32 sizeAddress;
    if ((pBuffer->m_Flags & DispStringBuffer::FLAG_ALT_BUFFER) != 0) {
        pCommand = nn::gr::CTR::MakeChannel0SubroutineCommand(pCommand, &sizeAddress, reinterpret_cast<uptr>(pBuffer->m_pCommandBufferAlt),
                                                              pBuffer->m_CommandSize * sizeof(u32));
    } else {
        pCommand = nn::gr::CTR::MakeChannel0SubroutineCommand(pCommand, &sizeAddress, reinterpret_cast<uptr>(pBuffer->m_pCommandBuffer),
                                                              pBuffer->m_CommandSize * sizeof(u32));
    }
    if (pDrawer != nullptr) {
        // the size of the way back of the last subroutine: up to here
        if (pDrawer->m_pSubroutineSizeAddress != nullptr) {
            *pDrawer->m_pSubroutineSizeAddress = (reinterpret_cast<uptr>(pCommand) - reinterpret_cast<uptr>(pDrawer->m_pLastSubroutineEnd)) >> 3;
            pDrawer->m_pSubroutineSizeAddress = nullptr;
        }
        pDrawer->m_pLastSubroutineEnd = pCommand;
        pDrawer->m_pSubroutineSizeAddress = reinterpret_cast<u32*>(sizeAddress);
        if (pDrawer->m_pFirstSubroutineEnd == nullptr) {
            pDrawer->m_pFirstSubroutineEnd = pCommand;
        }
    }
    return pCommand;
}

// 0x004D67A0 | libgarden [tier A]
u32 nw::font::CharWriter::GetDispStringBufferSize(u32 charCount)
{
    const u32 headerSize = sizeof(DispStringBuffer) + sizeof(DispStringBuffer::CharAttribute) * charCount
                           + (((charCount + 7) / 8 + 3) & ~3);
    return ((headerSize + 15) & ~15) + DispStringBuffer::CalcCommandBufferSize(charCount) * sizeof(u32);
}

// 0x004D6790 | libgarden [tier A]
nw::font::DispStringBuffer* nw::font::CharWriter::InitDispStringBuffer(void* pBuffer, u32 charCount)
{
    if (pBuffer == nullptr) {
        return nullptr;
    }
    return ::new (pBuffer) DispStringBuffer(charCount);
}

} // namespace font
} // namespace nw
