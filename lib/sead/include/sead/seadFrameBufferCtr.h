#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_FrameBuffer.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// RTTI N4sead14FrameBufferCtrE @ 0x008D1704
// vtable 0x00905830 (vptr 0x00905838), offset_to_top 0, 8 entries
//
// A frame buffer of nn::gr (binding writes its commands unless it is the one bound last; names are
// ours, bindImpl_ after the reference symbols).
class FrameBufferCtr : public ::sead::FrameBuffer
{
public:
    FrameBufferCtr(const nn::gr::CTR::FrameBuffer& frameBuffer, const Vector2<f32>& virtualSize, f32 x, f32 y,
                   f32 width, f32 height); // 0x0011E900
    virtual ~FrameBufferCtr(); // 0x00544FF0 slot 0x08
    // 0x00544FD4 slot 0x0C (deleting dtor)
    virtual void clearImpl_(u32 flags, const Color4f& color, f32 depth, u8 stencil); // 0x00726B48 slot 0x14
    virtual void bindImpl_() const; // 0x0074C014 slot 0x1C | nintendogs:callseq

    // the frame buffer of nn::gr bound last
    static nn::gr::CTR::FrameBuffer s_BoundGrFrameBuffer;

    nn::gr::CTR::FrameBuffer mGrFrameBuffer; // 0x1C
};
ASSERT_SIZE(FrameBufferCtr, 0x64);
} // namespace sead
