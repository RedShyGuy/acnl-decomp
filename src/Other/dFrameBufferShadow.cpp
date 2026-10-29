#include "sead/seadFrameBufferCtr.h"
#include "Other/dFrameBufferShadow.h"

// ctor candidate(s) 0x002CDDAC (unverified)
FrameBufferShadow::FrameBufferShadow(const nn::gr::CTR::FrameBuffer& frameBuffer, const sead::Vector2<f32>& virtualSize,
                                     f32 x, f32 y, f32 width, f32 height)
    : FrameBufferCtr(frameBuffer, virtualSize, x, y, width, height)
{
}

// 0x002CDF34 slot 0x08
// 0x002CDF18 (deleting dtor)
FrameBufferShadow::~FrameBufferShadow()
{
    // (only the bases are destroyed)
}

// 0x0071FC04 slot 0x1C | slot vf_0x1C of sead::FrameBuffer
void FrameBufferShadow::bindImpl_() const
{
}
