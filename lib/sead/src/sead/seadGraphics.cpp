#include "sead/seadGraphics.h"
#include "sead/seadDrawLockContext.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// 0x00976348
Graphics::DevicePosture Graphics::s_DefaultDevicePosture = Graphics::DEVICE_POSTURE_ROTATE_90;
// 0x0097634C
f32 Graphics::s_DefaultDeviceZScale = -1.0f;
// 0x00976350
f32 Graphics::s_DefaultDeviceZOffset = 0.0f;
// 0x00976344
Graphics* Graphics::s_pInstance = nullptr;

// 0x00120AAC (name is ours)
DECOMP_NOINLINE sead::Graphics::Graphics() : IDisposer(), mLockFunction(nullptr), mDrawLockContext(nullptr)
{
}

// 0x0013F6DC slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0056105C (deleting dtor)
sead::Graphics::~Graphics()
{
    // (only the bases are destroyed)
}

// 0x00545BA8 slot 0x08 (name is ours)
sead::DrawLockContext* sead::Graphics::getDrawLockContext() const
{
    return mDrawLockContext;
}

// 0x0013312C | nintendogs:callseq [tier A]
void sead::Graphics::lockDrawContext()
{
    if (mLockFunction != nullptr) {
        mLockFunction(true);
    } else {
        mDrawLockContext->lock();
    }
}

// 0x0013319C | nintendogs:callseq [tier A]
void sead::Graphics::unlockDrawContext()
{
    if (mLockFunction != nullptr) {
        mLockFunction(false);
    } else {
        mDrawLockContext->unlock();
    }
}
// 0x00561044 (name is ours)
void sead::Graphics::clearFrameBuffer(u32 flags, const Color4f& color, f32 depth, u8 stencil)
{
    // (the bound frame buffer is kept const, clearing changes its clear values)
    const_cast<FrameBuffer*>(FrameBuffer::s_pBoundFrameBuffer)->clearImpl_(flags, color, depth, stencil);
}
} // namespace sead
