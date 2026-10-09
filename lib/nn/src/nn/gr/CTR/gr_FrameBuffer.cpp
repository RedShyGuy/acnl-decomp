#include "nn/gr/CTR/gr_FrameBuffer.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gxlow/CTR/CTR_Api.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

namespace {
inline u32* MakeFlushCommand(u32* command)
{
    *command++ = 1;
    *command++ = CommandHeader(0x111);
    *command++ = 1;
    *command++ = CommandHeader(0x110);
    return command;
}
} // namespace

// 0x00123B90 | fefates:bytes [tier B]
nn::gr::CTR::FrameBuffer::FrameBuffer() : m_ColorBuffer(this), m_DepthStencilBuffer(this), m_Width(240), m_Height(400)
{
}

// 0x00726A44 | nintendogs:callseq-callee [confirmed by fefates, mk7dlp] [tier A]
u32* nn::gr::CTR::FrameBuffer::MakeCommand(u32* command, u32 bufferMask, bool isFlush) const
{
    if (isFlush) {
        command = MakeFlushCommand(command);
    }
    if (bufferMask & COLOR_BUFFER_BIT) {
        // RGBA8 has 32 bit pixels, the other formats 16 bit
        *command++ = (m_ColorBuffer.m_Format == 0 ? 2 : 0) | (m_ColorBuffer.m_Format << 16);
        *command++ = CommandHeader(0x117);
        *command++ = nngxGetPhysicalAddr(m_ColorBuffer.m_VirtualAddr) >> 3;
        *command++ = CommandHeader(0x11D);
    }
    if (bufferMask & (DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT)) {
        *command++ = m_DepthStencilBuffer.m_Format;
        *command++ = CommandHeader(0x116);
        *command++ = nngxGetPhysicalAddr(m_DepthStencilBuffer.m_VirtualAddr) >> 3;
        *command++ = CommandHeader(0x11C);
    }
    *command++ = m_Width | ((m_Height - 1) << 12) | 0x01000000;
    *command++ = CommandHeader(0x11E);
    *command++ = m_Width | ((m_Height - 1) << 12) | 0x01000000;
    *command++ = CommandHeader(0x6E);
    return MakeFlushCommand(command);
}

// 0x00726BB0 | fefates:bytes [tier B]
void nn::gr::CTR::FrameBuffer::MakeClearRequest(u32 bufferMask, bool isSplitDrawCmdlist) const
{
    u32 colorSize = 0;
    u32 colorWidth = 0;
    u32 colorValue = 0;
    if (bufferMask & COLOR_BUFFER_BIT) {
        const u8 r = detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorR);
        const u8 g = detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorG);
        const u8 b = detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorB);
        const u32 pixelCount = m_ColorBuffer.m_Width * m_ColorBuffer.m_Height;
        switch (m_ColorBuffer.m_Format) {
        case 0: // RGBA8
            colorValue = (r << 24) | (g << 16) | (b << 8) | detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorA);
            colorWidth = 32;
            colorSize = pixelCount * 4;
            break;
        case 2: // RGB5A1
            colorValue = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1)
                | (detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorA) > 127 ? 1 : 0);
            colorWidth = 16;
            colorSize = pixelCount * 2;
            break;
        case 3: // RGB565
            colorValue = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
            colorWidth = 16;
            colorSize = pixelCount * 2;
            break;
        case 4: // RGBA4
            colorValue = ((r >> 4) << 12) | ((g >> 4) << 8) | ((b >> 4) << 4)
                | (detail::Float32ToUnsignedByte(m_ColorBuffer.m_ClearColorA) >> 4);
            colorWidth = 16;
            colorSize = pixelCount * 2;
            break;
        }
    }

    u32 depthSize = 0;
    u32 depthWidth = 0;
    u32 depthValue = 0;
    if (bufferMask & (DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT)) {
        // the clear depth goes through an integer here (original behavior)
        f32 clearDepth = static_cast<f32>(static_cast<u32>(m_DepthStencilBuffer.m_ClearDepth));
        if (clearDepth > 1.0f) {
            clearDepth = 1.0f;
        }
        depthValue = static_cast<u32>(clearDepth);
        const u32 pixelCount = m_DepthStencilBuffer.m_Width * m_DepthStencilBuffer.m_Height;
        switch (m_DepthStencilBuffer.m_Format) {
        case 3: // D24S8
            depthSize = pixelCount * 4;
            depthValue = detail::Float32ToUnsignedFix24(static_cast<f32>(depthValue)) | (m_DepthStencilBuffer.m_ClearStencil << 24);
            depthWidth = 32;
            break;
        case 0: // D16
            depthSize = pixelCount * 2;
            depthValue = detail::Float32ToUnsignedFix16(static_cast<f32>(depthValue));
            depthWidth = 16;
            break;
        case 2: // D24
            depthSize = pixelCount * 3;
            depthValue = detail::Float32ToUnsignedFix24(static_cast<f32>(depthValue));
            depthWidth = 24;
            break;
        }
    }

    if (bufferMask & (COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT)) {
        if (isSplitDrawCmdlist) {
            nngxSplitDrawCmdlist();
        }
        nngxAddMemoryFillCommand((bufferMask & COLOR_BUFFER_BIT) ? m_ColorBuffer.m_VirtualAddr : 0, colorSize, colorValue,
                                 colorWidth,
                                 (bufferMask & (DEPTH_BUFFER_BIT | STENCIL_BUFFER_BIT)) ? m_DepthStencilBuffer.m_VirtualAddr : 0,
                                 depthSize, depthValue, depthWidth);
    }
}

} // namespace CTR
} // namespace gr
} // namespace nn
