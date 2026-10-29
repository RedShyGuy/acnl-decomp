#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadIDisposer.h"

namespace sead {
class DrawLockContext;

// RTTI N4sead8GraphicsE @ 0x008D21EC
// vtable 0x00906D34 (vptr 0x00906D3C), offset_to_top 0, 28 entries
// The graphics state of the program: the slots set the state of the hardware (GraphicsCtr), the
// draw lock is taken around the drawing.
class Graphics : public ::sead::IDisposer
{
public:
    // how the screen is turned against the frame buffer (the projections turn their matrix and the
    // viewports swap their size for it; names are ours)
    enum DevicePosture : u8
    {
        DEVICE_POSTURE_SAME = 0,
        DEVICE_POSTURE_ROTATE_90 = 1,  // x' = y, y' = -x (the screens of the 3DS)
        DEVICE_POSTURE_ROTATE_270 = 2, // x' = -y, y' = x
        DEVICE_POSTURE_ROTATE_180 = 3,
        DEVICE_POSTURE_FLIP_X = 4,
        DEVICE_POSTURE_FLIP_Y = 5,
        DEVICE_POSTURE_MAX = 6,
    };

    // the posture and the depth mapping of new projections and viewports (names are ours)
    static DevicePosture s_DefaultDevicePosture;
    static f32 s_DefaultDeviceZScale;
    static f32 s_DefaultDeviceZOffset;

    // calls the lock function with true/false instead of the draw lock context when it is set
    typedef void (*LockFunction)(bool lock);

    Graphics(); // 0x00120AAC
    virtual ~Graphics(); // 0x0013F6DC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x0056105C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual DrawLockContext* getDrawLockContext() const; // 0x00545BA8 slot 0x08
    // the slots of the derived class (names are ours, after what GraphicsCtr does; setScissorImpl is
    // the name of the reference symbols)
    virtual void vf_0x0C() = 0; // slot 0x0C
    virtual void setViewportImpl(f32 x, f32 y, f32 width, f32 height) = 0; // slot 0x10
    virtual void setScissorImpl(f32 x, f32 y, f32 width, f32 height) = 0; // slot 0x14
    virtual void setDepthTestEnableImpl(bool enable) = 0; // slot 0x18
    virtual void setDepthWriteEnableImpl(bool enable) = 0; // slot 0x1C
    virtual void setDepthFuncImpl(u8 func) = 0; // slot 0x20
    virtual bool vf_0x24() = 0; // slot 0x24
    virtual void setCullingModeImpl(s32 mode) = 0; // slot 0x28
    virtual void setBlendEnableImpl(bool enable) = 0; // slot 0x2C
    virtual void vf_0x30() = 0; // slot 0x30
    virtual void setBlendFactorImpl(u8 srcRgb, u8 dstRgb, u8 srcAlpha, u8 dstAlpha) = 0; // slot 0x34
    virtual void vf_0x38() = 0; // slot 0x38
    virtual void setBlendEquationImpl(u8 rgb, u8 alpha) = 0; // slot 0x3C
    virtual void vf_0x40() = 0; // slot 0x40
    virtual void setBlendConstantColorImpl(const Color4f& color) = 0; // slot 0x44
    virtual void waitForVBlankImpl() = 0; // slot 0x48
    virtual void setColorMaskImpl(bool r, bool g, bool b, bool a) = 0; // slot 0x4C
    virtual void vf_0x50() = 0; // slot 0x50
    virtual void setAlphaTestEnableImpl(bool enable) = 0; // slot 0x54
    virtual void setAlphaTestFuncImpl(u8 func, f32 ref) = 0; // slot 0x58
    virtual void setStencilTestEnableImpl(bool enable) = 0; // slot 0x5C
    virtual void setStencilTestFuncImpl(u8 func, s32 ref, u32 mask) = 0; // slot 0x60
    virtual void setStencilTestOpImpl(u8 fail, u8 zfail, u8 zpass) = 0; // slot 0x64
    virtual void vf_0x68() = 0; // slot 0x68
    virtual void vf_0x6C() = 0; // slot 0x6C

    // the lock of the drawing (lockDrawContext/unlockDrawContext are the names of the reference
    // symbols; symbols.json had them on the DrawLockContext functions behind these)
    void lockDrawContext(); // 0x0013312C
    void unlockDrawContext(); // 0x0013319C
    // clears the buffers of flags (1 color, 2 depth, 4 stencil) of the bound frame buffer (name is ours)
    void clearFrameBuffer(u32 flags, const Color4f& color, f32 depth, u8 stencil); // 0x00561044

    // the graphics of the program (the game sets it; name is ours)
    static Graphics* s_pInstance;

    LockFunction mLockFunction;        // 0x10
    DrawLockContext* mDrawLockContext; // 0x14
};
ASSERT_SIZE(Graphics, 0x18);
} // namespace sead
