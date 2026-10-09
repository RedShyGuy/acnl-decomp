#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_Types.h"

namespace nn {
namespace gr {
namespace CTR {
// The render target: a color and a depth/stencil buffer. The buffer classes are not named in the
// symbols (names are ours), member names are ours.
class FrameBuffer
{
public:
    // bits of the buffer masks
    static const u32 COLOR_BUFFER_BIT = 1;
    static const u32 DEPTH_BUFFER_BIT = 2;
    static const u32 STENCIL_BUFFER_BIT = 4;

    class ColorBuffer
    {
    public:
        explicit ColorBuffer(FrameBuffer* frameBuffer)
            : m_VirtualAddr(0), m_Format(static_cast<PicaDataColor>(0)), m_Unknown5(0), m_Width(240), m_Height(400),
              m_ClearColorR(0.0f), m_ClearColorG(0.0f), m_ClearColorB(0.0f), m_ClearColorA(0.0f), m_FrameBuffer(frameBuffer)
        {
        }

        u32 m_VirtualAddr;          // 0x00
        PicaDataColor m_Format;     // 0x04
        u8 m_Unknown5;              // 0x05
        u32 m_Width;                // 0x08
        u32 m_Height;               // 0x0C
        f32 m_ClearColorR;          // 0x10
        f32 m_ClearColorG;          // 0x14
        f32 m_ClearColorB;          // 0x18
        f32 m_ClearColorA;          // 0x1C
        FrameBuffer* m_FrameBuffer; // 0x20
    };

    class DepthStencilBuffer
    {
    public:
        explicit DepthStencilBuffer(FrameBuffer* frameBuffer)
            : m_VirtualAddr(0), m_Format(static_cast<PicaDataDepth>(3)), m_Width(240), m_Height(400), m_ClearDepth(1.0f),
              m_ClearStencil(0), m_FrameBuffer(frameBuffer)
        {
        }

        u32 m_VirtualAddr;          // 0x00
        PicaDataDepth m_Format;     // 0x04
        u32 m_Width;                // 0x08
        u32 m_Height;               // 0x0C
        f32 m_ClearDepth;           // 0x10
        u8 m_ClearStencil;          // 0x14
        FrameBuffer* m_FrameBuffer; // 0x18
    };

    FrameBuffer(); // 0x00123B90 | fefates:bytes [tier B]

    u32* MakeCommand(u32* command, u32 bufferMask, bool isFlush) const; // 0x00726A44 | nintendogs:callseq-callee [confirmed by fefates, mk7dlp] [tier A]
    void MakeClearRequest(u32 bufferMask, bool isSplitDrawCmdlist) const; // 0x00726BB0 | fefates:bytes [tier B]

    ColorBuffer m_ColorBuffer;                  // 0x00
    DepthStencilBuffer m_DepthStencilBuffer;    // 0x24
    u32 m_Width;                                // 0x40
    u32 m_Height;                               // 0x44
};
ASSERT_SIZE(FrameBuffer::ColorBuffer, 0x24);
ASSERT_SIZE(FrameBuffer::DepthStencilBuffer, 0x1C);
ASSERT_SIZE(FrameBuffer, 0x48);
} // namespace CTR
} // namespace gr
} // namespace nn
