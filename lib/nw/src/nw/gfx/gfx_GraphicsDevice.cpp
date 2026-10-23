#include "nw/gfx/gfx_GraphicsDevice.h"
#include "nw/gfx/internal/gfx_LightingRegisters.h"

#include "nn/gx/detail/gx_Context.h"
#include "nn/nstd/nstd_String.h"

namespace nw {
namespace gfx {
namespace {
// the PICA registers (3dbrew "GPU/Internal Registers")
const u32 REG_FOG_LUT_INDEX = 0x0E6;
const u32 REG_FRAMEBUFFER_INVALIDATE = 0x110;
const u32 REG_FRAMEBUFFER_FLUSH = 0x111;
const u32 REG_COLORBUFFER_READ = 0x112;
const u32 REG_LIGHTING_LUT_INDEX = 0x1C5;
const u32 REG_LIGHT0_XY = 0x144; // per light 0x10 registers

// (inline) a float as a 1.5.10 float (too large: infinity without sign; names are ours)
inline u16 ToFloat16(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    const u32 sign = (bits.u & 0x80000000) ? 0x8000 : 0;
    const u32 mantissa = ((bits.u << 9) >> 9) >> 13;
    s32 exponent = 0;
    if ((bits.u & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits.u << 1) >> 24) - 112;
        if (exponent < 0) {
            return sign;
        }
        if (exponent > 31) {
            return 0x7C00;
        }
    }
    return mantissa | sign | ((exponent & 31) << 10);
}
} // namespace

namespace internal {
// 0x00976068
u8 g_FragmentOperationMode = 0;
// 0x00976069
bool g_IsBufferAccessDirty = true;
// 0x0097606A
bool g_IsColorRead = true;
// 0x0097606B
bool g_IsDepthTest = true;
// 0x0097606C
bool g_IsStencilTest = false;
// 0x0097606D
bool g_IsDepthWrite = true;
// 0x0097606E
bool g_IsPolygonOffset = false;
// 0x0097606F
u8 g_StencilWriteMask = 0xFF;
// 0x00976070
u32 g_ColorWriteMask = 0xF;
// -1.0 (set by the static initializer)
// 0x00976074
u32 g_DepthMapScale = 0xBF0000;
// flush and invalidate the frame buffer, then the access to its buffers (the static initializer sets
// the headers)
// 0x00976078
u32 g_BufferAccessCommand[10] = {
    1, 0xF0000 | REG_FRAMEBUFFER_FLUSH, 1, 0xF0000 | REG_FRAMEBUFFER_INVALIDATE, 0, 0x80000000 | (3 << 20) | 0xF0000 | REG_COLORBUFFER_READ,
    0, 0, 0, 0,
};
// GL_DEPTH24_STENCIL8_EXT
// 0x009760AC
u32 g_DepthFormat = 0x88F0;
// 0x009760B0
u32 g_WScale = 0;
// 0x009760B4
f32 g_DepthRangeNear = 0.0f;
// 0x009760B8
f32 g_DepthRangeFar = 1.0f;
// 0x009760BC
f32 g_PolygonOffsetUnits = 0.0f;
// 0x009760A0
u32 g_LightingLutInputAbs = 0;
// 0x009760A4
u32 g_LightingLutInputSelect = 0;
// 0x009760A8
u32 g_LightingLutInputScale = 0;
// 0x009760C0
u32 g_DirectionalLightMask = 0;
// 0x009760C4
u32 g_ShadowDisableMask = 0xFF;
// 0x009760C8
u32 g_SpotDisableMask = 0xFF;
// 0x009760CC
u32 g_DistanceAttenuationDisableMask = 0xFF;
} // namespace internal

// 0x00AE4F50 (name is ours)
res::ResImageLookupTable nw::gfx::GraphicsDevice::s_LookupTables[LUT_TARGET_COUNT];

// sets g_DepthMapScale, constructs s_LookupTables and sets the headers of g_BufferAccessCommand
// 0x0079C920 (static initializer)

// 0x004998A4 | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::ActivateLookupTable(res::ResImageLookupTable lut, LutTarget target)
{
    if (s_LookupTables[target].ptr() == lut.ptr()) {
        return;
    }
    s_LookupTables[target] = lut;

    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    if (target == LUT_TARGET_FOG) {
        pCommand[0] = 0;
        pCommand[1] = 0xF0000 | REG_FOG_LUT_INDEX;
    } else {
        // (the PICA has no lighting table 7)
        pCommand[0] = (target + (target >= 7 ? 1 : 0)) << 8;
        pCommand[1] = 0xF0000 | REG_LIGHTING_LUT_INDEX;
    }
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 2;
    const u32 commandSize = lut.ptr()->commandSize;
    nnnstdMemCpy(nn::gx::detail::s_pCommandBufferCurrent, lut.ptr()->commands.ToPtr(), commandSize);
    nn::gx::detail::s_pCommandBufferCurrent =
        reinterpret_cast<u32*>(reinterpret_cast<u8*>(nn::gx::detail::s_pCommandBufferCurrent) + (commandSize & ~3));
}

// 0x00499938 (name is ours)
void nw::gfx::res::CleanupImageLookupTable(ResLookupTableData* pData)
{
    GraphicsDevice::InvalidateLookupTable(res::ResImageLookupTable(pData));
}

// 0x0049993C | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::InvalidateLookupTable(res::ResImageLookupTable lut)
{
    for (int i = 0; i < LUT_TARGET_COUNT; ++i) {
        if (s_LookupTables[i].ptr() == lut.ptr()) {
            s_LookupTables[i] = res::ResImageLookupTable(nullptr);
        }
    }
}

// 0x0049996C | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::InvalidateAllLookupTables()
{
    for (int i = 0; i < LUT_TARGET_COUNT; ++i) {
        s_LookupTables[i] = res::ResImageLookupTable(nullptr);
    }
}

// 0x00499994 | nintendogs:bytes [tier A]
void nw::gfx::GraphicsDevice::ActivateFragmentLightPosition(int lightIdx, const nn::math::VEC4& position)
{
    const u32 xy = ToFloat16(position.x) | (ToFloat16(position.y) << 16);
    const u32 header = 0x80000000 | (1 << 20) | (0xF << 16) | (REG_LIGHT0_XY + lightIdx * 0x10);
    const u32 z = ToFloat16(position.z);
    u32* pCommand = nn::gx::detail::s_pCommandBufferCurrent;
    pCommand[2] = z;
    pCommand[3] = 0;
    pCommand[0] = xy;
    pCommand[1] = header;
    nn::gx::detail::s_pCommandBufferCurrent = pCommand + 4;
}

} // namespace gfx
} // namespace nw
