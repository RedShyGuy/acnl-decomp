#include <cstring>

#include "nn/gx/detail/gx_Context.h"
#include "nn/gx/detail/gx_Shader.h"
#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nw/gfx/gfx_ShaderBinaryInfo.h"
#include "nw/gfx/internal/internal_Api.h"
#include "nw/gfx/res/gfx_ResShader.h"
#include "nw/gfx/res/gfx_ResVertexAttribute.h"

namespace nw {
namespace gfx {
namespace internal {
namespace {
// a PICA command header (3dbrew "GPU/Internal Registers"): register, byte mask, extra parameters,
// consecutive registers (name is ours)
constexpr u32 Header(u32 reg, u32 mask = 0xF, u32 extraCount = 0, bool isConsecutive = false)
{
    return reg | (mask << 16) | (extraCount << 20) | (isConsecutive ? 0x80000000 : 0);
}

// the registers (names are ours, after 3dbrew)
const u32 REG_FRAMEBUFFER_INVALIDATE = 0x110;
const u32 REG_FRAMEBUFFER_FLUSH = 0x111;
const u32 REG_ATTRIBBUFFERS_LOC = 0x200;
const u32 REG_ATTRIBBUFFERS_FORMAT_HIGH = 0x202;
const u32 REG_ATTRIBBUFFER0_OFFSET = 0x203;
const u32 REG_ATTRIBBUFFER0_CONFIG2 = 0x205;
const u32 REG_INDEXBUFFER_CONFIG = 0x227;
const u32 REG_NUMVERTICES = 0x228;
const u32 REG_GEOSTAGE_CONFIG = 0x229;
const u32 REG_DRAWELEMENTS = 0x22F;
const u32 REG_VTX_FUNC = 0x231;
const u32 REG_FIXEDATTRIB_INDEX = 0x232;
const u32 REG_VSH_NUM_ATTR = 0x242;
const u32 REG_START_DRAW_FUNC0 = 0x245;
const u32 REG_VSH_OUTMAP_TOTAL2 = 0x251;
const u32 REG_GEOSTAGE_CONFIG2 = 0x253;
const u32 REG_PRIMITIVE_CONFIG = 0x25E;
const u32 REG_RESTART_PRIMITIVE = 0x25F;
const u32 REG_GSH_ATTRIBUTES_PERMUTATION_LOW = 0x28B;
const u32 REG_GSH_ATTRIBUTES_PERMUTATION_HIGH = 0x28C;
const u32 REG_VSH_INPUTBUFFER_CONFIG = 0x2B9;
const u32 REG_VSH_ATTRIBUTES_PERMUTATION_LOW = 0x2BB;

// the GL enums used here (names after the OpenGL ES API; the geometry primitive of the gx extension
// is ours)
namespace gl {
const GLenum TRIANGLES = 0x0004;
const GLenum TRIANGLE_STRIP = 0x0005;
const GLenum TRIANGLE_FAN = 0x0006;
const GLenum BYTE = 0x1400;
const GLenum UNSIGNED_BYTE = 0x1401;
const GLenum SHORT = 0x1402;
const GLenum UNSIGNED_SHORT = 0x1403;
const GLenum FLOAT = 0x1406;
const GLenum GEOMETRY_PRIMITIVE = 0x6010;
} // namespace gl

// the memory area the vertex and index buffers count from
const GLenum MEMORY_AREA_VRAM_A = 0x20000;

// the bytes SetupDrawIndexStreamCommand writes
const u32 DRAW_INDEX_STREAM_COMMAND_SIZE = 112;

// the primitive of a ResIndexStreamData::primitiveMode
// 0x008B4BE8
const GLenum s_PrimitiveModes[] = {gl::TRIANGLES, gl::TRIANGLE_STRIP, gl::TRIANGLE_FAN};

// All twelve attributes fixed, the buffers off and the fixed values 0 (attribute 0 keeps its value).
// The original builds the headers in a static initializer.
// 0x0097F120
u32 s_ClearVertexAttributeCommand[] = {
    0xBFFF0000, Header(REG_ATTRIBBUFFERS_FORMAT_HIGH),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 0),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 1),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 2),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 3),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 4),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 5),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 6),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 7),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 8),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 9),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 10),
    0, Header(REG_ATTRIBBUFFER0_CONFIG2 + 3 * 11),
    1, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    2, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    3, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    4, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    5, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    6, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    7, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    8, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    9, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    10, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
    11, Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true), 0, 0, 0, 0,
};

// the index of the data word of the geometry stage in s_ShaderProgramModeCommand
const int SHADER_PROGRAM_MODE_INDEX = 44;
// the geometry stage with a geometry shader (REG_GEOSTAGE_CONFIG)
const u32 GEOSTAGE_GEOMETRY_SHADER = 2;

// The switch between the vertex shader alone and with a geometry shader: the geometry stage between
// dummy writes (byte mask 0) that give the GPU time.
// 0x0097F290
u32 s_ShaderProgramModeCommand[] = {
    0, Header(REG_VSH_OUTMAP_TOTAL2, 0x0, 9), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, Header(REG_ATTRIBBUFFERS_LOC, 0x0, 29), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, Header(REG_GEOSTAGE_CONFIG, 0x1),
    0, Header(REG_ATTRIBBUFFERS_LOC, 0x0, 29), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static_assert(sizeof(s_ShaderProgramModeCommand) == 78 * sizeof(u32), "the size FastWordCopyAsm_78 copies");

// The static initializer of the tables above (the headers of s_ClearVertexAttributeCommand); GCC
// initializes them constantly.
// 0x0079D020 (static initializer)

// the command buffer: the one of the info, else the current command list
inline u32* GetCommandBuffer(CommandBufferInfo& info)
{
    return info.pCurrent != nullptr ? info.pCurrent : nn::gx::detail::s_pCommandBufferCurrent;
}

// the commands written: the buffer advanced if they fit
inline void AdvanceCommandBuffer(CommandBufferInfo& info, u32 size)
{
    bool isFit;
    if (info.pCurrent != nullptr) {
        isFit = reinterpret_cast<u8*>(info.pCurrent) + size <= reinterpret_cast<u8*>(info.pEnd);
    } else {
        isFit = reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + size <=
                reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferEnd);
    }
    if (isFit) {
        if (info.pCurrent != nullptr) {
            info.pCurrent = reinterpret_cast<u32*>(reinterpret_cast<u8*>(info.pCurrent) + size);
        } else {
            nn::gx::detail::s_pCommandBufferCurrent =
                reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + size);
        }
    }
}

// the data of a stream: the uploaded copy, else the data in the resource
inline const void* GetStreamAddress(const res::ResVertexStreamBaseData* pStream)
{
    return pStream->pLocationAddress != nullptr ? pStream->pLocationAddress : pStream->data.ToPtr();
}

// the format of an attribute in REG_ATTRIBBUFFERS_FORMAT (3dbrew)
inline u32 GetFormatCode(GLenum formatType)
{
    switch (formatType) {
    case gl::BYTE:
        return 0;
    case gl::UNSIGNED_BYTE:
        return 1;
    case gl::SHORT:
        return 2;
    case gl::FLOAT:
        return 3;
    default:
        return 0;
    }
}

} // namespace

// 0x0014E1F0 | nintendogs:bytes [tier A]
void NWCopyVec4Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0]\n"
        "bx lr\n"
    );
}

// 0x0014E208 | nintendogs:bytes [tier A]
void NWCopyVec4WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "str r2, [r0, #4]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0]\n"
        "bx lr\n"
    );
}

// 0x0014E224 | nintendogs:bytes [tier A]
void NWCopyMtx24Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s7}\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "vstr s6, [r0, #20]\n"
        "vstr s7, [r0, #16]\n"
        "bx lr\n"
    );
}

// 0x0014E24C | nintendogs:bytes [tier A]
void NWCopyMtx24WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s7}\n"
        "str r2, [r0, #4]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #32]\n"
        "vstr s5, [r0, #28]\n"
        "vstr s6, [r0, #24]\n"
        "vstr s7, [r0, #20]\n"
        "bx lr\n"
    );
}

// 0x0014E278 | nintendogs:bytes [tier A]
void NWCopyMtx34Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "vstr s6, [r0, #20]\n"
        "vstr s7, [r0, #16]\n"
        "vstr s8, [r0, #44]\n"
        "vstr s9, [r0, #40]\n"
        "vstr s10, [r0, #36]\n"
        "vstr s11, [r0, #32]\n"
        "bx lr\n"
    );
}

// 0x0014E2B0 | nintendogs:callgraph [tier A]
void NWCopyMtx34WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "str r2, [r0, #4]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #32]\n"
        "vstr s5, [r0, #28]\n"
        "vstr s6, [r0, #24]\n"
        "vstr s7, [r0, #20]\n"
        "vstr s8, [r0, #48]\n"
        "vstr s9, [r0, #44]\n"
        "vstr s10, [r0, #40]\n"
        "vstr s11, [r0, #36]\n"
        "bx lr\n"
    );
}

// 0x0014E2EC | nintendogs:bytes [tier A]
void NWCopyMtx44Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s15}\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "vstr s6, [r0, #20]\n"
        "vstr s7, [r0, #16]\n"
        "vstr s8, [r0, #44]\n"
        "vstr s9, [r0, #40]\n"
        "vstr s10, [r0, #36]\n"
        "vstr s11, [r0, #32]\n"
        "vstr s12, [r0, #60]\n"
        "vstr s13, [r0, #56]\n"
        "vstr s14, [r0, #52]\n"
        "vstr s15, [r0, #48]\n"
        "bx lr\n"
    );
}

// 0x0014E334 | nintendogs:callgraph [tier A]
void NWCopyMtx44WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s15}\n"
        "str r2, [r0, #4]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0]\n"
        "vstr s4, [r0, #32]\n"
        "vstr s5, [r0, #28]\n"
        "vstr s6, [r0, #24]\n"
        "vstr s7, [r0, #20]\n"
        "vstr s8, [r0, #48]\n"
        "vstr s9, [r0, #44]\n"
        "vstr s10, [r0, #40]\n"
        "vstr s11, [r0, #36]\n"
        "vstr s12, [r0, #64]\n"
        "vstr s13, [r0, #60]\n"
        "vstr s14, [r0, #56]\n"
        "vstr s15, [r0, #52]\n"
        "bx lr\n"
    );
}

// 0x0014E380 | nintendogs:bytes [tier A]
void NWCopyVec3Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "bx lr\n"
    );
}

// 0x0014E39C | nintendogs:bytes [tier A]
void NWCopyVec3WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "bx lr\n"
    );
}

// 0x0014E3BC | nintendogs:bytes [tier A]
void NWCopyMtx23Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s5}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #16]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0, #28]\n"
        "vstr s4, [r0, #24]\n"
        "vstr s5, [r0, #20]\n"
        "bx lr\n"
    );
}

// 0x0014E3E8 | nintendogs:bytes [tier A]
void NWCopyMtx23WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s5}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #20]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0, #32]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "bx lr\n"
    );
}

// 0x0014E418 | nintendogs:bytes [tier A]
void NWCopyMtx33Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s8}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #32]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0, #28]\n"
        "vstr s4, [r0, #24]\n"
        "vstr s5, [r0, #20]\n"
        "vstr s6, [r0, #44]\n"
        "vstr s7, [r0, #40]\n"
        "vstr s8, [r0, #36]\n"
        "bx lr\n"
    );
}

// 0x0014E454 | nintendogs:bytes [tier A]
void NWCopyMtx33WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s8}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #36]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0, #32]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "vstr s6, [r0, #48]\n"
        "vstr s7, [r0, #44]\n"
        "vstr s8, [r0, #40]\n"
        "bx lr\n"
    );
}

// 0x0014E494 | nintendogs:bytes [tier A]
void NWCopyMtx43Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #32]\n"
        "str r3, [r0, #48]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #4]\n"
        "vstr s3, [r0, #28]\n"
        "vstr s4, [r0, #24]\n"
        "vstr s5, [r0, #20]\n"
        "vstr s6, [r0, #44]\n"
        "vstr s7, [r0, #40]\n"
        "vstr s8, [r0, #36]\n"
        "vstr s9, [r0, #60]\n"
        "vstr s10, [r0, #56]\n"
        "vstr s11, [r0, #52]\n"
        "bx lr\n"
    );
}

// 0x0014E4E0 | nintendogs:bytes [tier A]
void NWCopyMtx43WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #52]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #8]\n"
        "vstr s3, [r0, #32]\n"
        "vstr s4, [r0, #28]\n"
        "vstr s5, [r0, #24]\n"
        "vstr s6, [r0, #48]\n"
        "vstr s7, [r0, #44]\n"
        "vstr s8, [r0, #40]\n"
        "vstr s9, [r0, #64]\n"
        "vstr s10, [r0, #60]\n"
        "vstr s11, [r0, #56]\n"
        "bx lr\n"
    );
}

// 0x0014E530 | nintendogs:bytes [tier A]
void NWCopyVec2Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s1}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "bx lr\n"
    );
}

// 0x0014E54C | nintendogs:bytes [tier A]
void NWCopyVec2WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s1}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "bx lr\n"
    );
}

// 0x0014E56C | nintendogs:bytes [tier A]
void NWCopyMtx22Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #28]\n"
        "vstr s3, [r0, #24]\n"
        "bx lr\n"
    );
}

// 0x0014E598 | nintendogs:bytes [tier A]
void NWCopyMtx22WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s3, [r0, #28]\n"
        "bx lr\n"
    );
}

// 0x0014E5C8 | nintendogs:bytes [tier A]
void NWCopyMtx32Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s5}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #32]\n"
        "str r3, [r0, #36]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #28]\n"
        "vstr s3, [r0, #24]\n"
        "vstr s4, [r0, #44]\n"
        "vstr s5, [r0, #40]\n"
        "bx lr\n"
    );
}

// 0x0014E604 | nintendogs:bytes [tier A]
void NWCopyMtx32WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s5}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s3, [r0, #28]\n"
        "vstr s4, [r0, #48]\n"
        "vstr s5, [r0, #44]\n"
        "bx lr\n"
    );
}

// 0x0014E644 | nintendogs:bytes [tier A]
void NWCopyMtx42Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s7}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #32]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #48]\n"
        "str r3, [r0, #52]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #8]\n"
        "vstr s2, [r0, #28]\n"
        "vstr s3, [r0, #24]\n"
        "vstr s4, [r0, #44]\n"
        "vstr s5, [r0, #40]\n"
        "vstr s6, [r0, #60]\n"
        "vstr s7, [r0, #56]\n"
        "bx lr\n"
    );
}

// 0x0014E690 | nintendogs:bytes [tier A]
void NWCopyMtx42WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s7}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "str r3, [r0, #52]\n"
        "str r3, [r0, #56]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s3, [r0, #28]\n"
        "vstr s4, [r0, #48]\n"
        "vstr s5, [r0, #44]\n"
        "vstr s6, [r0, #64]\n"
        "vstr s7, [r0, #60]\n"
        "bx lr\n"
    );
}

// 0x0014E6E0 | nintendogs:bytes [tier A]
void NWCopyVec1Reverse(float*, const float*)
{
    asm volatile(
        "vldr s0, [r1]\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #8]\n"
        "vstr s0, [r0, #12]\n"
        "bx lr\n"
    );
}

// 0x0014E6FC | nintendogs:bytes [tier A]
void NWCopyVec1WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldr s0, [r1]\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #12]\n"
        "vstr s0, [r0, #16]\n"
        "bx lr\n"
    );
}

// 0x0014E71C | nintendogs:bytes [tier A]
void NWCopyMtx21Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s1}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #28]\n"
        "bx lr\n"
    );
}

// 0x0014E748 | nintendogs:bytes [tier A]
void NWCopyMtx21WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s1}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #12]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #28]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #32]\n"
        "bx lr\n"
    );
}

// 0x0014E778 | nintendogs:bytes [tier A]
void NWCopyMtx31Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #32]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #28]\n"
        "vstr s2, [r0, #44]\n"
        "bx lr\n"
    );
}

// 0x0014E7B4 | nintendogs:bytes [tier A]
void NWCopyMtx31WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #12]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #28]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "str r3, [r0, #44]\n"
        "vstr s0, [r0, #16]\n"
        "vstr s1, [r0, #32]\n"
        "vstr s2, [r0, #48]\n"
        "bx lr\n"
    );
}

// 0x0014E7F4 | nintendogs:bytes [tier A]
void NWCopyMtx41Reverse(float*, const float*)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "mov r3, #0\n"
        "str r3, [r0]\n"
        "str r3, [r0, #4]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #16]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #32]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "str r3, [r0, #48]\n"
        "str r3, [r0, #52]\n"
        "str r3, [r0, #56]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #28]\n"
        "vstr s2, [r0, #44]\n"
        "vstr s3, [r0, #60]\n"
        "bx lr\n"
    );
}

// 0x0014E840 | nintendogs:bytes [tier A]
void NWCopyMtx41WithHeader(float*, const float*, unsigned)
{
    asm volatile(
        "vldmia r1!, {s0-s3}\n"
        "mov r3, #0\n"
        "str r2, [r0, #4]\n"
        "str r3, [r0]\n"
        "str r3, [r0, #8]\n"
        "str r3, [r0, #12]\n"
        "str r3, [r0, #20]\n"
        "str r3, [r0, #24]\n"
        "str r3, [r0, #28]\n"
        "str r3, [r0, #36]\n"
        "str r3, [r0, #40]\n"
        "str r3, [r0, #44]\n"
        "str r3, [r0, #52]\n"
        "str r3, [r0, #56]\n"
        "str r3, [r0, #60]\n"
        "vstr s0, [r0, #12]\n"
        "vstr s1, [r0, #28]\n"
        "vstr s2, [r0, #44]\n"
        "vstr s3, [r0, #60]\n"
        "bx lr\n"
    );
}

// 0x0014E8AC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_4(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E8C8 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_6(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E8E4 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_8(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E904 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_10(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2}\n"
        "ldm r1!, {r3}\n"
        "stmia r0!, {r2}\n"
        "stmia r0!, {r3}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E934 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_12(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E964 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_14(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E994 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_16(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014E9C8 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_18(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2}\n"
        "ldm r1!, {r3}\n"
        "stmia r0!, {r2}\n"
        "stmia r0!, {r3}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EA0C | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_20(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EA50 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_22(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EA94 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_24(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EADC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_26(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2}\n"
        "ldm r1!, {r3}\n"
        "stmia r0!, {r2}\n"
        "stmia r0!, {r3}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EB34 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_28(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EB8C | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_30(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EBE4 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_32(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014EC40 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_34(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2}\n"
        "ldm r1!, {r3}\n"
        "stmia r0!, {r2}\n"
        "stmia r0!, {r3}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014ECAC | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_36(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014ED18 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_38(u32*, u32*)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x0014ED84 | nintendogs:bytes [tier A]
u32* FastWordCopyAsm_78(u32* /*pDst*/, u32* /*pSrc*/)
{
    asm volatile(
        "push {r4, r5, r9, sl, fp}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r9, sl, fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {r9, sl, fp, ip}\n"
        "ldm r1!, {r2, r3, r4, r5}\n"
        "ldm r1!, {fp, ip}\n"
        "stmia r0!, {r2, r3, r4, r5}\n"
        "stmia r0!, {fp, ip}\n"
        "pop {r4, r5, r9, sl, fp}\n"
        "bx lr\n"
    );
}

// 0x004B22E4 | nintendogs:bytes [tier A]
void ClearVertexAttribute()
{
    std::memcpy(nn::gx::detail::s_pCommandBufferCurrent, s_ClearVertexAttributeCommand, sizeof(s_ClearVertexAttributeCommand));
    nn::gx::detail::s_pCommandBufferCurrent += sizeof(s_ClearVertexAttributeCommand) / sizeof(u32);
}

// 0x004B2314 | nintendogs:bytes [tier A]
u32 SetupShaderProgramMode(bool isGeometryShaderEnabled)
{
    s_ShaderProgramModeCommand[SHADER_PROGRAM_MODE_INDEX] = isGeometryShaderEnabled ? GEOSTAGE_GEOMETRY_SHADER : 0;
    nn::gx::detail::s_pCommandBufferCurrent = FastWordCopyAsm_78(nn::gx::detail::s_pCommandBufferCurrent, s_ShaderProgramModeCommand);
    return sizeof(s_ShaderProgramModeCommand);
}

// 0x004B23D8 | nintendogs:bytes [tier A]
u32 SetupDrawIndexStreamCommand(CommandBufferInfo& info, res::ResIndexStream indexStream, bool isGeometryPrimitive)
{
    const res::ResIndexStreamData* pStream = indexStream.ptr();
    GLenum mode = s_PrimitiveModes[pStream->primitiveMode];
    if (isGeometryPrimitive) {
        mode = gl::GEOMETRY_PRIMITIVE;
    }
    u32* pCommand = GetCommandBuffer(info);
    const u32 base = nngxGetPhysicalAddr(nngxGetVramStartAddr(MEMORY_AREA_VRAM_A));
    const void* pIndices = pStream->pLocationAddress != nullptr ? pStream->pLocationAddress : pStream->data.ToPtr();
    const u32 offset = nngxGetPhysicalAddr(reinterpret_cast<uptr>(pIndices)) - base;

    // the indices (bit 31: 16-bit indices)
    pCommand[0] = offset;
    pCommand[1] = Header(REG_INDEXBUFFER_CONFIG);
    pCommand[2] = pStream->size;
    pCommand[3] = Header(REG_NUMVERTICES);
    if (pStream->format == gl::UNSIGNED_SHORT) {
        pCommand[0] = offset | 0x80000000;
        pCommand[2] = pStream->size >> 1;
    }

    // the primitive
    if (mode == gl::TRIANGLES) {
        pCommand[7] = Header(REG_GEOSTAGE_CONFIG2, 0x2);
        pCommand[4] = 0x100;
        pCommand[5] = Header(REG_GEOSTAGE_CONFIG, 0x2);
        pCommand[6] = 0x100;
        pCommand[8] = 0x300;
    } else {
        pCommand[7] = Header(REG_GEOSTAGE_CONFIG2, 0x2);
        pCommand[4] = 0;
        pCommand[5] = Header(REG_GEOSTAGE_CONFIG, 0x2);
        pCommand[6] = 0;
        if (mode == gl::TRIANGLE_STRIP) {
            pCommand[8] = 0x100;
        } else if (mode == gl::TRIANGLE_FAN) {
            pCommand[8] = 0x200;
        } else if (mode == gl::GEOMETRY_PRIMITIVE) {
            pCommand[8] = 0x300;
        }
    }
    pCommand[9] = Header(REG_PRIMITIVE_CONFIG, 0x2);

    // the draw
    pCommand[10] = 1;
    pCommand[11] = Header(REG_RESTART_PRIMITIVE);
    pCommand[12] = 0;
    pCommand[13] = Header(REG_START_DRAW_FUNC0, 0x1);
    pCommand[14] = 1;
    pCommand[15] = Header(REG_DRAWELEMENTS);
    pCommand[16] = 1;
    pCommand[17] = Header(REG_START_DRAW_FUNC0, 0x1);
    pCommand[18] = 1;
    pCommand[19] = Header(REG_VTX_FUNC);
    pCommand[20] = 0;
    pCommand[21] = Header(REG_PRIMITIVE_CONFIG, 0x8);
    pCommand[22] = 0;
    pCommand[23] = Header(REG_PRIMITIVE_CONFIG, 0x8);
    pCommand[24] = 1;
    pCommand[25] = Header(REG_FRAMEBUFFER_FLUSH);
    pCommand[26] = 1;
    pCommand[27] = Header(REG_FRAMEBUFFER_INVALIDATE);

    const u32 size = DRAW_INDEX_STREAM_COMMAND_SIZE;
    AdvanceCommandBuffer(info, size);
    return size;
}

// 0x004B25E0 | nintendogs:bytes-fuzzy [tier A]
u32 SetupDeactivateVertexAttributeCommand(CommandBufferInfo& info, res::ResSeparateDataShape shape, res::ResShaderProgramDescription /*description*/)
{
    const res::ResSeparateDataShapeData* pShape = shape.ptr();
    u32* pCommand = GetCommandBuffer(info);
    int attributeCount = 0;
    int bufferIndex = 0;
    int index = 0;

    // the buffers off
    const int vertexAttributeCount = pShape->vertexAttributes.count;
    for (int i = 0; i < vertexAttributeCount; ++i) {
        const res::ResVertexAttributeData* pAttribute = static_cast<const res::ResVertexAttributeData*>(pShape->vertexAttributes.GetData(i));
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_CONSTANT) {
            ++attributeCount;
            continue;
        }
        pCommand[index] = 0;
        pCommand[index + 1] = Header(static_cast<u16>(REG_ATTRIBBUFFER0_CONFIG2 + bufferIndex * 3));
        index += 2;
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_INTERLEAVED) {
            attributeCount += static_cast<const res::ResInterleavedVertexStreamData*>(pAttribute)->attributeCount;
        } else {
            ++attributeCount;
        }
        ++bufferIndex;
    }

    // the attributes all fixed (0)
    pCommand[index++] = 0x76543210;
    pCommand[index++] = Header(REG_VSH_ATTRIBUTES_PERMUTATION_LOW, 0xF, 1, true);
    pCommand[index++] = 0xBA98;
    pCommand[index++] = 0;
    pCommand[index++] = ((attributeCount - 1) << 28) | (((1 << attributeCount) - 2) << 16);
    pCommand[index++] = Header(REG_ATTRIBBUFFERS_FORMAT_HIGH);
    for (int i = 1; i < attributeCount; ++i) {
        pCommand[index] = i;
        pCommand[index + 1] = Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true);
        pCommand[index + 2] = 0;
        pCommand[index + 3] = 0;
        pCommand[index + 4] = 0;
        pCommand[index + 5] = 0;
        index += 6;
    }

    const u32 size = index * sizeof(u32);
    AdvanceCommandBuffer(info, size);
    return size;
}

// 0x004B27EC (name is ours)
u32 GetVertexAttributeCommandSize(res::ResSeparateDataShape shape, res::ResShaderProgramDescription description)
{
    const res::ResSeparateDataShapeData* pShape = shape.ptr();
    u32 words = 12;
    for (int i = 0; i < pShape->vertexAttributes.count; ++i) {
        const res::ResVertexAttributeData* pAttribute = static_cast<const res::ResVertexAttributeData*>(pShape->vertexAttributes.GetData(i));
        words += (pAttribute->flags & res::ResVertexAttributeData::FLAG_CONSTANT) ? 6 : 4;
    }
    if (description.ptr()->geometryShaderIndex >= 0) {
        words += 4;
    }
    return words * sizeof(u32);
}

// 0x004B2860 (name is ours)
u32 GetDeactivateVertexAttributeCommandSize(res::ResSeparateDataShape shape)
{
    const res::ResSeparateDataShapeData* pShape = shape.ptr();
    int attributeCount = 0;
    int words = 0;
    for (int i = 0; i < pShape->vertexAttributes.count; ++i) {
        const res::ResVertexAttributeData* pAttribute = static_cast<const res::ResVertexAttributeData*>(pShape->vertexAttributes.GetData(i));
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_CONSTANT) {
            ++attributeCount;
        } else {
            words += 2;
            if (pAttribute->flags & res::ResVertexAttributeData::FLAG_INTERLEAVED) {
                attributeCount += static_cast<const res::ResInterleavedVertexStreamData*>(pAttribute)->attributeCount;
            } else {
                ++attributeCount;
            }
        }
    }
    words += 6;
    if (attributeCount - 1 > 0) {
        words += (attributeCount - 1) * 6;
    }
    return words * sizeof(u32);
}

// 0x007E6F70 | nintendogs:bytes [tier A]
u32 SetupVertexAttributeCommand(CommandBufferInfo& info, res::ResSeparateDataShape shape, res::ResShaderProgramDescription description)
{
    res::ResSeparateDataShapeData* pShape = shape.ptr();
    const res::ResShaderProgramDescriptionData* pDescription = description.ptr();
    const ShaderBinaryInfo* pBinaryInfo = pDescription->owner.To<res::ResBinaryShaderData>()->pBinaryInfo;
    const nn::gx::detail::DvleHeader* pVertexShader =
        static_cast<const nn::gx::detail::DvleHeader*>(pBinaryInfo->m_pShaders[pDescription->vertexShaderIndex]);

    // the input registers of the vertex shader
    int inputCount = 0;
    for (int i = 0; i < 16; ++i) {
        if (pVertexShader->m_InputMask & (1 << i)) {
            ++inputCount;
        }
    }

    const u32 base = nngxGetPhysicalAddr(nngxGetVramStartAddr(MEMORY_AREA_VRAM_A));
    pShape->vertexBufferBase = base;
    const int vertexAttributeCount = pShape->vertexAttributes.count;
    u32* pCommand = GetCommandBuffer(info);
    pCommand[0] = (inputCount - 1) | 0xA0000000;
    pCommand[1] = Header(REG_VSH_INPUTBUFFER_CONFIG, 0xB);
    pCommand[2] = inputCount - 1;
    pCommand[3] = Header(REG_VSH_NUM_ATTR, 0x1);
    pCommand[4] = 0;
    pCommand[5] = Header(REG_VSH_ATTRIBUTES_PERMUTATION_LOW, 0xF, 1, true);
    pCommand[8] = base >> 3;
    pCommand[6] = 0;
    pCommand[7] = 0;
    pCommand[9] = Header(REG_ATTRIBBUFFERS_LOC, 0xF, 2, true);
    pCommand[10] = 0;
    pCommand[11] = (inputCount - 1) << 28;
    u32* pPermutation = &pCommand[4];
    u32* pFormat = &pCommand[10];
    u32* pFormatHigh = &pCommand[11];

    u32 usedRegisters = 0;
    int attributeIndex = 0;
    int bufferIndex = 0;
    int index = 12;
    for (int i = 0; i < vertexAttributeCount; ++i) {
        const res::ResVertexAttributeData* pAttribute = static_cast<const res::ResVertexAttributeData*>(pShape->vertexAttributes.GetData(i));
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_CONSTANT) {
            continue;
        }
        if (pAttribute->flags & res::ResVertexAttributeData::FLAG_INTERLEAVED) {
            // a buffer with the attributes one after the other in each vertex
            const res::ResInterleavedVertexStreamData* pStream = static_cast<const res::ResInterleavedVertexStreamData*>(pAttribute);
            const u32 offset = nngxGetPhysicalAddr(reinterpret_cast<uptr>(GetStreamAddress(pStream))) - base;
            const int count = pStream->attributeCount;
            pCommand[index] = offset;
            pCommand[index + 2] = 0;
            pCommand[index + 1] = Header(static_cast<u16>(REG_ATTRIBBUFFER0_OFFSET + bufferIndex * 3), 0xF, 2, true);
            u32* pConfig = &pCommand[index + 2];
            index += 4;
            pConfig[1] = (pStream->stride << 16) | (count << 28);
            for (int k = 0; k < count; ++k) {
                const res::ResVertexStreamData* pElement = pStream->attributes.To<ut::Offset>()[k].To<res::ResVertexStreamData>();
                const u8 reg = pDescription->attributeRegisters[pElement->usage];
                pPermutation[attributeIndex / 8] |= (reg & 0xF) << ((attributeIndex % 8) * 4);
                usedRegisters |= 1 << reg;
                pFormat[attributeIndex / 8] |= (GetFormatCode(pElement->formatType) + pElement->dimension * 4 - 4) << ((attributeIndex % 8) * 4);
                if (k < 8) {
                    pConfig[0] |= attributeIndex << (k * 4);
                } else {
                    pConfig[1] |= attributeIndex << (k * 4 - 32);
                }
                ++attributeIndex;
            }
        } else {
            // a buffer of one attribute
            const res::ResVertexStreamData* pStream = static_cast<const res::ResVertexStreamData*>(pAttribute);
            const u8 reg = pDescription->attributeRegisters[pStream->usage];
            pPermutation[attributeIndex / 8] |= (reg & 0xF) << ((attributeIndex % 8) * 4);
            usedRegisters |= 1 << reg;
            pFormat[attributeIndex / 8] |= (GetFormatCode(pStream->formatType) + pStream->dimension * 4 - 4) << ((attributeIndex % 8) * 4);
            const u32 offset = nngxGetPhysicalAddr(reinterpret_cast<uptr>(GetStreamAddress(pStream))) - base;
            pCommand[index] = offset;
            pCommand[index + 1] = Header(static_cast<u16>(REG_ATTRIBBUFFER0_OFFSET + bufferIndex * 3), 0xF, 2, true);
            pCommand[index + 2] = attributeIndex;
            u32 stride;
            switch (pStream->formatType) {
            case gl::BYTE:
            case gl::UNSIGNED_BYTE:
                stride = pStream->dimension;
                break;
            case gl::SHORT:
                stride = pStream->dimension * 2;
                break;
            case gl::FLOAT:
                stride = pStream->dimension * 4;
                break;
            default:
                stride = 0;
                break;
            }
            index += 4;
            ++attributeIndex;
            pCommand[index - 1] = 0x10000000 + (stride << 16);
        }
        ++bufferIndex;
    }

    // the constant attributes: fixed values
    for (int i = 0; i < vertexAttributeCount; ++i) {
        const res::ResVertexAttributeData* pAttribute = static_cast<const res::ResVertexAttributeData*>(pShape->vertexAttributes.GetData(i));
        if (!(pAttribute->flags & res::ResVertexAttributeData::FLAG_CONSTANT)) {
            continue;
        }
        const res::ResVertexParamAttributeData* pParam = static_cast<const res::ResVertexParamAttributeData*>(pAttribute);
        const u8 reg = pDescription->attributeRegisters[pParam->usage];
        pPermutation[attributeIndex / 8] |= (reg & 0xF) << ((attributeIndex % 8) * 4);
        u32 values[4] = {0, 0, 0, 0};
        usedRegisters |= 1 << reg;
        const f32* pValues = pParam->values.To<f32>();
        for (int k = 0; k < pParam->valueCount; ++k) {
            values[k] = Float32ToFloat24Clamped(pValues[k]);
        }
        pCommand[index] = attributeIndex;
        pCommand[index + 1] = Header(REG_FIXEDATTRIB_INDEX, 0xF, 3, true);
        pCommand[index + 2] = (values[3] << 8) | (values[2] >> 16);
        pCommand[index + 3] = (values[2] << 16) | (values[1] >> 8);
        pCommand[index + 4] = values[0] | (values[1] << 24);
        pCommand[index + 5] = 0;
        index += 6;
        *pFormatHigh |= 1 << (attributeIndex + 16);
        ++attributeIndex;
    }

    // the input registers no attribute uses: fixed, on free registers
    while (static_cast<u32>(attributeIndex) < static_cast<u32>(inputCount)) {
        int reg = pDescription->attributeRegisters[inputCount - 1];
        while (usedRegisters & (1 << reg)) {
            --reg;
        }
        pPermutation[attributeIndex / 8] |= (reg & 0xF) << ((attributeIndex % 8) * 4);
        usedRegisters |= 1 << reg;
        *pFormatHigh |= 1 << (attributeIndex + 16);
        ++attributeIndex;
    }

    // the geometry shader takes the outputs in order
    if (pDescription->geometryShaderIndex >= 0) {
        pCommand[index] = 0x76543210;
        pCommand[index + 3] = Header(REG_GSH_ATTRIBUTES_PERMUTATION_HIGH);
        pCommand[index + 1] = Header(REG_GSH_ATTRIBUTES_PERMUTATION_LOW);
        pCommand[index + 2] = 0xFEDCBA98;
        index += 4;
    }

    const u32 size = index * sizeof(u32);
    AdvanceCommandBuffer(info, size);
    return size;
}

} // namespace internal
} // namespace gfx
} // namespace nw
