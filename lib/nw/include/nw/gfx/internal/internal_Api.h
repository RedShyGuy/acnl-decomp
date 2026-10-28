#pragma once

#include "decomp.h"

#include <cstring>
#include "nw/gfx/res/gfx_ResPrimitive.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResShape.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"

namespace nw {
namespace gfx {
namespace internal {
// (inline) a 1.7.16 float (the float24 of the GPU), too large values the largest
inline u32 Float32ToFloat24Clamped(f32 value)
{
    u32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    const u32 sign = (bits & 0x80000000) ? 0x800000 : 0;
    const u32 mantissa = ((bits << 9) >> 9) >> 7;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
        if (exponent < 0) {
            return sign;
        }
        if (exponent > 127) {
            return 0x7F0000;
        }
    }
    return sign | mantissa | ((exponent & 0x7F) << 16);
}

// A buffer that commands are written to (names are ours; no current position: the current command
// list of nngx).
struct CommandBufferInfo
{
    // (inline)
    CommandBufferInfo(u32* pBuffer, u32 size)
        : pBuffer(pBuffer), pEnd(reinterpret_cast<u32*>(reinterpret_cast<u8*>(pBuffer) + size)), pCurrent(pBuffer), size(size)
    {
    }

    u32* pBuffer;  // 0x0
    u32* pEnd;     // 0x4
    u32* pCurrent; // 0x8
    u32 size;      // 0xC bytes
};
ASSERT_SIZE(CommandBufferInfo, 0x10);

// The vertex attributes of a shape one by one; an interleaved stream gives each of its attributes
// (names are ours). The end: index -1.
class VertexAttributeIterator
{
public:
    // (inline) at the first attribute
    explicit VertexAttributeIterator(const res::ResSeparateDataShapeData* pShape)
        : m_pShape(pShape), m_pInterleaved(nullptr), m_Index(0), m_SubIndex(-1)
    {
        const res::ResVertexAttributeData* pAttribute = GetAttribute(0);
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_INTERLEAVED) {
            m_pInterleaved = static_cast<const res::ResInterleavedVertexStreamData*>(pAttribute);
            m_SubIndex = 0;
        }
    }

    bool IsEnd() const { return m_Index < 0; }

    // (inline) the attribute (of an interleaved stream: only usage, format and dimension)
    const res::ResVertexAttributeData* operator*() const
    {
        if (m_pInterleaved != nullptr) {
            return m_pInterleaved->attributes.To<ut::Offset>()[m_SubIndex].To<res::ResVertexAttributeData>();
        }
        return GetAttribute(m_Index);
    }

    // to the next attribute (past the last: the end)
    DECOMP_EMIT_COPY VertexAttributeIterator& operator++() // 0x004B234C (name is ours)
    {
        if (m_pInterleaved != nullptr) {
            ++m_SubIndex;
            if (m_SubIndex < m_pInterleaved->attributeCount) {
                return *this;
            }
            m_pInterleaved = nullptr;
            m_SubIndex = -1;
        }
        ++m_Index;
        if (m_Index >= m_pShape->vertexAttributes.count) {
            m_Index = -1;
            return *this;
        }
        const res::ResVertexAttributeData* pAttribute = GetAttribute(m_Index);
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_INTERLEAVED) {
            m_pInterleaved = static_cast<const res::ResInterleavedVertexStreamData*>(pAttribute);
            m_SubIndex = 0;
        }
        return *this;
    }

private:
    const res::ResVertexAttributeData* GetAttribute(int index) const
    {
        return static_cast<const res::ResVertexAttributeData*>(m_pShape->vertexAttributes.GetData(index));
    }

    const res::ResSeparateDataShapeData* m_pShape;                 // 0x0
    const res::ResInterleavedVertexStreamData* m_pInterleaved;     // 0x4 the stream whose attributes come now
    s32 m_Index;                                                   // 0x8 the attribute of the shape
    s32 m_SubIndex;                                                // 0xC the attribute of the stream
};
ASSERT_SIZE(VertexAttributeIterator, 0x10);

// The copies of vectors and matrices into commands (reversed, or behind a command header) and the
// fixed-size word copies: hand-written assembly in the original, so the bodies are copied from the
// binary as naked functions (like nn::math::ARMv6).
#define GFX_ASM __attribute__((naked))
void NWCopyVec4Reverse(float*, const float*) GFX_ASM; // 0x0014E1F0 | nintendogs:bytes [tier A]
void NWCopyVec4WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E208 | nintendogs:bytes [tier A]
void NWCopyMtx24Reverse(float*, const float*) GFX_ASM; // 0x0014E224 | nintendogs:bytes [tier A]
void NWCopyMtx24WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E24C | nintendogs:bytes [tier A]
void NWCopyMtx34Reverse(float*, const float*) GFX_ASM; // 0x0014E278 | nintendogs:bytes [tier A]
void NWCopyMtx34WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E2B0 | nintendogs:callgraph [tier A]
void NWCopyMtx44Reverse(float*, const float*) GFX_ASM; // 0x0014E2EC | nintendogs:bytes [tier A]
void NWCopyMtx44WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E334 | nintendogs:callgraph [tier A]
void NWCopyVec3Reverse(float*, const float*) GFX_ASM; // 0x0014E380 | nintendogs:bytes [tier A]
void NWCopyVec3WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E39C | nintendogs:bytes [tier A]
void NWCopyMtx23Reverse(float*, const float*) GFX_ASM; // 0x0014E3BC | nintendogs:bytes [tier A]
void NWCopyMtx23WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E3E8 | nintendogs:bytes [tier A]
void NWCopyMtx33Reverse(float*, const float*) GFX_ASM; // 0x0014E418 | nintendogs:bytes [tier A]
void NWCopyMtx33WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E454 | nintendogs:bytes [tier A]
void NWCopyMtx43Reverse(float*, const float*) GFX_ASM; // 0x0014E494 | nintendogs:bytes [tier A]
void NWCopyMtx43WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E4E0 | nintendogs:bytes [tier A]
void NWCopyVec2Reverse(float*, const float*) GFX_ASM; // 0x0014E530 | nintendogs:bytes [tier A]
void NWCopyVec2WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E54C | nintendogs:bytes [tier A]
void NWCopyMtx22Reverse(float*, const float*) GFX_ASM; // 0x0014E56C | nintendogs:bytes [tier A]
void NWCopyMtx22WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E598 | nintendogs:bytes [tier A]
void NWCopyMtx32Reverse(float*, const float*) GFX_ASM; // 0x0014E5C8 | nintendogs:bytes [tier A]
void NWCopyMtx32WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E604 | nintendogs:bytes [tier A]
void NWCopyMtx42Reverse(float*, const float*) GFX_ASM; // 0x0014E644 | nintendogs:bytes [tier A]
void NWCopyMtx42WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E690 | nintendogs:bytes [tier A]
void NWCopyVec1Reverse(float*, const float*) GFX_ASM; // 0x0014E6E0 | nintendogs:bytes [tier A]
void NWCopyVec1WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E6FC | nintendogs:bytes [tier A]
void NWCopyMtx21Reverse(float*, const float*) GFX_ASM; // 0x0014E71C | nintendogs:bytes [tier A]
void NWCopyMtx21WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E748 | nintendogs:bytes [tier A]
void NWCopyMtx31Reverse(float*, const float*) GFX_ASM; // 0x0014E778 | nintendogs:bytes [tier A]
void NWCopyMtx31WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E7B4 | nintendogs:bytes [tier A]
void NWCopyMtx41Reverse(float*, const float*) GFX_ASM; // 0x0014E7F4 | nintendogs:bytes [tier A]
void NWCopyMtx41WithHeader(float*, const float*, unsigned) GFX_ASM; // 0x0014E840 | nintendogs:bytes [tier A]
// (symbols.json names the start of the copy functions "copy_asm_declarations")
u32* FastWordCopyAsm_2(u32*, u32*) GFX_ASM; // 0x0014E890 | nintendogs:bytes
u32* FastWordCopyAsm_4(u32*, u32*) GFX_ASM; // 0x0014E8AC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_6(u32*, u32*) GFX_ASM; // 0x0014E8C8 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_8(u32*, u32*) GFX_ASM; // 0x0014E8E4 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_10(u32*, u32*) GFX_ASM; // 0x0014E904 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_12(u32*, u32*) GFX_ASM; // 0x0014E934 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_14(u32*, u32*) GFX_ASM; // 0x0014E964 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_16(u32*, u32*) GFX_ASM; // 0x0014E994 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_18(u32*, u32*) GFX_ASM; // 0x0014E9C8 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_20(u32*, u32*) GFX_ASM; // 0x0014EA0C | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_22(u32*, u32*) GFX_ASM; // 0x0014EA50 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_24(u32*, u32*) GFX_ASM; // 0x0014EA94 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_26(u32*, u32*) GFX_ASM; // 0x0014EADC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_28(u32*, u32*) GFX_ASM; // 0x0014EB34 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_30(u32*, u32*) GFX_ASM; // 0x0014EB8C | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_32(u32*, u32*) GFX_ASM; // 0x0014EBE4 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_34(u32*, u32*) GFX_ASM; // 0x0014EC40 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_36(u32*, u32*) GFX_ASM; // 0x0014ECAC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_38(u32*, u32*) GFX_ASM; // 0x0014ED18 | nintendogs:bytes [tier A]
// the 78 words copied, the end of the destination returned
u32* FastWordCopyAsm_78(u32* pDst, u32* pSrc) GFX_ASM; // 0x0014ED84 | nintendogs:bytes [tier A]
// all vertex attributes fixed to 0, the buffers off (to the current command list)
void ClearVertexAttribute(); // 0x004B22E4 | nintendogs:bytes [tier A]
// the geometry stage for the vertex shader alone or with a geometry shader; returns the bytes
u32 SetupShaderProgramMode(bool isGeometryShaderEnabled); // 0x004B2314 | nintendogs:bytes [tier A]
// the draw of an index stream; returns the bytes
u32 SetupDrawIndexStreamCommand(CommandBufferInfo& info, res::ResIndexStream indexStream, bool isGeometryPrimitive); // 0x004B23D8 | nintendogs:bytes [tier A]
// the vertex buffers of a shape off, its attributes fixed to 0; returns the bytes
u32 SetupDeactivateVertexAttributeCommand(CommandBufferInfo& info, res::ResSeparateDataShape shape, res::ResShaderProgramDescription description); // 0x004B25E0 | nintendogs:bytes-fuzzy [tier A]
// the bytes SetupVertexAttributeCommand writes
u32 GetVertexAttributeCommandSize(res::ResSeparateDataShape shape, res::ResShaderProgramDescription description); // 0x004B27EC (name is ours)
// the bytes SetupDeactivateVertexAttributeCommand writes
u32 GetDeactivateVertexAttributeCommandSize(res::ResSeparateDataShape shape); // 0x004B2860 (name is ours)
// the vertex buffers and fixed attributes of a shape for the input registers of the vertex shader;
// returns the bytes
u32 SetupVertexAttributeCommand(CommandBufferInfo& info, res::ResSeparateDataShape shape, res::ResShaderProgramDescription description); // 0x007E6F70 | nintendogs:bytes [tier A]
} // namespace internal
} // namespace gfx
} // namespace nw
