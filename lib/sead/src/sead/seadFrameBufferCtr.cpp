#include "sead/seadFrameBufferCtr.h"
#include "nn/gx/gx_Api.h"
#include "sead/seadCommandBufferCtr.h"

#include <cstring>

namespace sead {
// 0x00AE64E0
nn::gr::CTR::FrameBuffer FrameBufferCtr::s_BoundGrFrameBuffer;

// the static initializer of s_BoundGrFrameBuffer
// 0x0079CA88 (static initializer)

// 0x0011E900 (name is ours)
sead::FrameBufferCtr::FrameBufferCtr(const nn::gr::CTR::FrameBuffer& frameBuffer, const Vector2<f32>& virtualSize,
                                     f32 x, f32 y, f32 width, f32 height)
    : FrameBuffer(virtualSize, x, y, width, height), mGrFrameBuffer()
{
    mGrFrameBuffer = frameBuffer;
}

// 0x00544FF0 slot 0x08
// 0x00544FD4 (deleting dtor)
sead::FrameBufferCtr::~FrameBufferCtr()
{
    // (only the bases are destroyed)
}

// 0x00726B48 slot 0x14 (name is ours)
void sead::FrameBufferCtr::clearImpl_(u32 flags, const Color4f& color, f32 depth, u8 stencil)
{
    u32 mask = 0;
    if (flags & 1) {
        mask = nn::gr::CTR::FrameBuffer::COLOR_BUFFER_BIT;
    }
    if (flags & 2) {
        mask |= nn::gr::CTR::FrameBuffer::DEPTH_BUFFER_BIT;
    }
    if (flags & 4) {
        mask |= nn::gr::CTR::FrameBuffer::STENCIL_BUFFER_BIT;
    }
    std::memcpy(&mGrFrameBuffer.m_ColorBuffer.m_ClearColorR, &color, sizeof(Color4f));
    mGrFrameBuffer.m_DepthStencilBuffer.m_ClearDepth = depth;
    mGrFrameBuffer.m_DepthStencilBuffer.m_ClearStencil = stencil;
    nngxFlush3DCommand();
    mGrFrameBuffer.MakeClearRequest(mask, false);
}

// 0x0074C014 slot 0x1C | nintendogs:callseq
void sead::FrameBufferCtr::bindImpl_() const
{
    const nn::gr::CTR::FrameBuffer& bound = s_BoundGrFrameBuffer;
    if (bound.m_ColorBuffer.m_VirtualAddr == mGrFrameBuffer.m_ColorBuffer.m_VirtualAddr &&
        bound.m_ColorBuffer.m_Format == mGrFrameBuffer.m_ColorBuffer.m_Format &&
        bound.m_ColorBuffer.m_Unknown5 == mGrFrameBuffer.m_ColorBuffer.m_Unknown5 &&
        bound.m_ColorBuffer.m_Width == mGrFrameBuffer.m_ColorBuffer.m_Width &&
        bound.m_ColorBuffer.m_Height == mGrFrameBuffer.m_ColorBuffer.m_Height &&
        bound.m_DepthStencilBuffer.m_VirtualAddr == mGrFrameBuffer.m_DepthStencilBuffer.m_VirtualAddr &&
        bound.m_DepthStencilBuffer.m_Format == mGrFrameBuffer.m_DepthStencilBuffer.m_Format &&
        bound.m_DepthStencilBuffer.m_Width == mGrFrameBuffer.m_DepthStencilBuffer.m_Width &&
        bound.m_DepthStencilBuffer.m_Height == mGrFrameBuffer.m_DepthStencilBuffer.m_Height &&
        bound.m_Width == mGrFrameBuffer.m_Width && bound.m_Height == mGrFrameBuffer.m_Height) {
        return;
    }
    u32* command = getCurrentCommand();
    u32* start = command;
    command = mGrFrameBuffer.MakeCommand(command, 7, true);
    moveCommandBufferPointer(start, command);
    s_BoundGrFrameBuffer = mGrFrameBuffer;
}
} // namespace sead
