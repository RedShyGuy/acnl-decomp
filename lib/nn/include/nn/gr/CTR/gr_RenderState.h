#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
// The fragment operations. Every part keeps a pointer back to the render state so that it can
// update the frame buffer access (FBAccess) that depends on all of them. LogicOp and WBuffer
// are not named in the symbols (names are ours), all member names are ours. Values are the PICA
// register fields on 3dbrew.
class RenderState
{
public:
    class Blend
    {
    public:
        explicit Blend(RenderState* renderState)
            : m_IsEnable(false), m_EquationRgb(0), m_EquationAlpha(0), m_SrcRgb(6), m_SrcAlpha(6), m_DstRgb(7),
              m_DstAlpha(7), m_ColorR(0xFF), m_ColorG(0xFF), m_ColorB(0xFF), m_ColorA(0xFF), m_RenderState(renderState)
        {
        }

        u32* MakeCommand(u32* command, bool isUpdateFBAccess) const; // 0x0012DF08 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        u8 m_EquationRgb;           // 0x01
        u8 m_EquationAlpha;         // 0x02
        u8 m_SrcRgb;                // 0x03
        u8 m_SrcAlpha;              // 0x04
        u8 m_DstRgb;                // 0x05
        u8 m_DstAlpha;              // 0x06
        u8 m_ColorR;                // 0x07
        u8 m_ColorG;                // 0x08
        u8 m_ColorB;                // 0x09
        u8 m_ColorA;                // 0x0A
        RenderState* m_RenderState; // 0x0C
    };

    // (name is ours)
    class LogicOp
    {
    public:
        explicit LogicOp(RenderState* renderState) : m_IsEnable(false), m_Operation(6), m_RenderState(renderState) {}

        u32* MakeCommand(u32* command) const;

        bool m_IsEnable;            // 0x00
        u8 m_Operation;             // 0x01
        RenderState* m_RenderState; // 0x04
    };

    class ShadowMap
    {
    public:
        explicit ShadowMap(RenderState* renderState)
            : m_IsEnable(false), m_IsPerspective(true), m_ZBias(0.0f), m_ZScale(1.0f), m_PenumbraScale(0.0f),
              m_PenumbraBias(1.0f), m_RenderState(renderState)
        {
        }

        u32* MakeCommand(u32* command, bool isUpdateFBAccess, bool isResetTextureConfig) const; // 0x0012E1E8 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        bool m_IsPerspective;       // 0x01
        f32 m_ZBias;                // 0x04
        f32 m_ZScale;               // 0x08
        f32 m_PenumbraScale;        // 0x0C
        f32 m_PenumbraBias;         // 0x10
        RenderState* m_RenderState; // 0x14
    };

    class AlphaTest
    {
    public:
        explicit AlphaTest(RenderState* renderState) : m_IsEnable(false), m_RefValue(0), m_Func(0), m_RenderState(renderState) {}

        u32* MakeCommand(u32* command, bool isUpdateFBAccess) const; // 0x007270F0 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        u8 m_RefValue;              // 0x01
        u8 m_Func;                  // 0x02
        RenderState* m_RenderState; // 0x04
    };

    class StencilTest
    {
    public:
        explicit StencilTest(RenderState* renderState)
            : m_IsEnable(false), m_WriteMask(0xFF), m_Func(1), m_RefValue(0), m_Mask(0xFF), m_OpFail(0), m_OpZFail(0),
              m_OpZPass(0), m_RenderState(renderState)
        {
        }

        u32* MakeCommand(u32* command, bool isUpdateFBAccess) const; // 0x00726FE8 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        u8 m_WriteMask;             // 0x01
        u8 m_Padding[2];            // 0x02
        u8 m_Func;                  // 0x04
        u32 m_RefValue;             // 0x08
        u32 m_Mask;                 // 0x0C
        u8 m_OpFail;                // 0x10
        u8 m_OpZFail;               // 0x11
        u8 m_OpZPass;               // 0x12
        RenderState* m_RenderState; // 0x14
    };

    class DepthTest
    {
    public:
        explicit DepthTest(RenderState* renderState) : m_IsEnable(true), m_IsEnableWrite(true), m_Func(4), m_RenderState(renderState) {}

        u32* MakeCommand(u32* command, bool isUpdateFBAccess) const; // 0x00727148 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        bool m_IsEnableWrite;       // 0x01
        u8 m_Func;                  // 0x02
        RenderState* m_RenderState; // 0x04
    };

    class Culling
    {
    public:
        explicit Culling(RenderState* renderState) : m_IsEnable(true), m_FrontFace(1), m_CullFace(1), m_RenderState(renderState) {}

        u32* MakeCommand(u32* command, bool isUpdateFBAccess) const; // 0x00727074 | fefates:bytes [tier B]

        bool m_IsEnable;            // 0x00
        u8 m_FrontFace;             // 0x01
        u8 m_CullFace;              // 0x02
        RenderState* m_RenderState; // 0x04
    };

    // w buffer, depth range and polygon offset (name is ours)
    class WBuffer
    {
    public:
        WBuffer()
            : m_WScale(0.0f), m_IsEnablePolygonOffset(false), m_PolygonOffsetUnit(0.0f), m_DepthRangeNear(0.0f),
              m_DepthRangeFar(1.0f), m_DepthBufferBits(24)
        {
        }

        DECOMP_NOINLINE u32* MakeCommand(u32* command) const; // 0x0012DFC8 (name is ours)

        f32 m_WScale;                   // 0x00, 0 = no w buffer
        bool m_IsEnablePolygonOffset;   // 0x04
        f32 m_PolygonOffsetUnit;        // 0x08
        f32 m_DepthRangeNear;           // 0x0C
        f32 m_DepthRangeFar;            // 0x10
        u8 m_DepthBufferBits;           // 0x14
    };

    class FBAccess
    {
    public:
        explicit FBAccess(RenderState* renderState) : m_RenderState(renderState) {}

        u32* MakeCommand(u32* command, bool isFlush) const; // 0x00134564 | fefates:bytes [tier B]

        RenderState* m_RenderState; // 0x00
    };

    RenderState(); // 0x00123C08 | fefates:bytes [tier B]

    u32* MakeCommand(u32* command, bool isFlush) const; // 0x001269F0 | nintendogs:callgraph [confirmed by mk7dlp] [tier A]
    // the default state (name is ours)
    static u32* MakeDisableCommand(u32* command, bool isFlush); // 0x00349984 (name is ours)

    Blend m_Blend;              // 0x00
    LogicOp m_LogicOp;          // 0x10
    ShadowMap m_ShadowMap;      // 0x18
    AlphaTest m_AlphaTest;      // 0x30
    StencilTest m_StencilTest;  // 0x38
    u8 m_ColorMask;             // 0x50, bit 0 red to bit 3 alpha
    DepthTest m_DepthTest;      // 0x54
    Culling m_Culling;          // 0x5C
    WBuffer m_WBuffer;          // 0x64
    FBAccess m_FBAccess;        // 0x7C
};
ASSERT_SIZE(RenderState::Blend, 0x10);
ASSERT_SIZE(RenderState::ShadowMap, 0x18);
ASSERT_SIZE(RenderState::StencilTest, 0x18);
ASSERT_SIZE(RenderState::WBuffer, 0x18);
ASSERT_SIZE(RenderState, 0x80);
} // namespace CTR
} // namespace gr
} // namespace nn
