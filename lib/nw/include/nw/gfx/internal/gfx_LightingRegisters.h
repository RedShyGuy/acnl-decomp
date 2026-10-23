#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
namespace internal {
// The values gfx last sent to registers of the GPU (3dbrew "GPU/Internal Registers"), kept between the
// render context and the material activators; they are in the file of the GraphicsDevice (names are
// ours).

// the fragment operation and the access to the color and depth buffers
extern u8 g_FragmentOperationMode;        // COLOR_OPERATION bits 0-1
extern bool g_IsBufferAccessDirty;        // the buffer access has to be sent again
extern bool g_IsColorRead;                // the blending reads the color buffer
extern bool g_IsDepthTest;
extern bool g_IsStencilTest;
extern bool g_IsDepthWrite;
extern bool g_IsPolygonOffset;
extern u8 g_StencilWriteMask;
extern u32 g_ColorWriteMask;
extern u32 g_DepthMapScale;               // DEPTHMAP_SCALE (float24)
extern u32 g_BufferAccessCommand[10];     // flush, invalidate, COLORBUFFER_READ ... DEPTHBUFFER_WRITE

// the depth buffer
extern u32 g_DepthFormat;                 // the GL format of the depth buffer
extern u32 g_WScale;                      // DEPTHMAP_OFFSET of the w buffer (0: z buffer)
extern f32 g_DepthRangeNear;
extern f32 g_DepthRangeFar;
extern f32 g_PolygonOffsetUnits;

// the fragment lighting
extern u32 g_LightingLutInputAbs;     // LIGHTING_LUTINPUT_ABS
extern u32 g_LightingLutInputSelect;  // LIGHTING_LUTINPUT_SELECT
extern u32 g_LightingLutInputScale;   // LIGHTING_LUTINPUT_SCALE
extern u32 g_DirectionalLightMask;    // per fragment light: its position has w = 0
extern u32 g_ShadowDisableMask;       // LIGHTING_CONFIG1 bits 0-7
extern u32 g_SpotDisableMask;         // LIGHTING_CONFIG1 bits 8-15
extern u32 g_DistanceAttenuationDisableMask; // LIGHTING_CONFIG1 bits 24-31
} // namespace internal
} // namespace gfx
} // namespace nw
