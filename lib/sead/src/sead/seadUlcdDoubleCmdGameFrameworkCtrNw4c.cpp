#include "nn/gx/gx_Api.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDoubleCmdGameFrameworkCtrNw4c.h"
#include "sead/seadFrameBuffer.h"
#include "sead/seadUlcdDoubleCmdGameFrameworkCtrNw4c.h"
#include "sead/seadUlcdMethodTreeMgr.h"

namespace sead {
// 0x0011EC38 | nintendogs:bytes [tier A]
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::setUlcdEnable(bool enable)
{
    // the display mode of the top screen: 3D or 2D (values after the code)
    if (enable) {
        nngxSetDisplayMode(0x702);
    } else {
        nngxSetDisplayMode(0x700);
    }
}

// (out of line: the derived constructors call it)
// 0x0011EC54 (name is ours)
DECOMP_NOINLINE sead::UlcdDoubleCmdGameFrameworkCtrNw4c::UlcdDoubleCmdGameFrameworkCtrNw4c(const CreateArg& arg)
    : DoubleCmdGameFrameworkCtrNw4c(arg)
{
}

// 0x00549000 slot 0x30
sead::MethodTreeMgr* sead::UlcdDoubleCmdGameFrameworkCtrNw4c::createMethodTreeMgr_(Heap* heap)
{
    return new (heap, 4) UlcdMethodTreeMgr();
}

// 0x0054DF5C slot 0x08
// 0x0054E1B4 (deleting dtor)
sead::UlcdDoubleCmdGameFrameworkCtrNw4c::~UlcdDoubleCmdGameFrameworkCtrNw4c()
{
    // empty (in the original too)
}

// 0x0054DF94 slot 0x78 | nintendogs:callseq
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::swapBuffer_()
{
    if (mFrameCount <= 0) {
        return;
    }
    nngxBindCmdlist(mCmdLists[1 - mCmdListIndex]);
    GLint isRunning;
    nngxGetCmdlistParameteri(0x201, &isRunning);
    if (isRunning != 0) {
        nngxWaitCmdlistDone();
    }
    nngxStopCmdlist();
    nngxClearCmdlist();
    s32 index = (mDisplayBufferIndex + 2) % 3;
    nngxActiveDisplay(0x400);
    nngxBindDisplaybuffer(mTopDisplayBuffers3[index]);
    nngxActiveDisplay(0x410);
    nngxBindDisplaybuffer(mRightDisplayBuffers[index]);
    nngxActiveDisplay(0x401);
    nngxBindDisplaybuffer(mBtmDisplayBuffers3[index]);
    nngxBindCmdlist(mCmdLists[mCmdListIndex]);
}

// 0x0054E050 slot 0x90 (name is ours)
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::presentTopLeft_()
{
    presentTop_();
}

// 0x0054E05C slot 0x94 (name is ours)
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::presentTopRight_()
{
    requestTransferRenderImage_(mRightDisplayBuffers[mDisplayBufferIndex], mGrFrameBuffer, mArg.topTransferX,
                                mArg.topTransferY, 0x500, false);
}

// 0x0054E094 slot 0x88
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::vf_0x88()
{
    // empty (in the original too)
}

// 0x0054E098 slot 0x68
void sead::UlcdDoubleCmdGameFrameworkCtrNw4c::procDraw_()
{
    UlcdMethodTreeMgr* mgr = DynamicCast<UlcdMethodTreeMgr>(mMethodTreeMgr);
    mTopFrameBuffer->bind();
    clearFrameBuffers_(UlcdMethodTreeMgr::METHOD_LEFT_APP_DRAW);
    mgr->drawTopLeft();
    presentTopLeft_();
    mTopFrameBuffer->bind();
    clearFrameBuffers_(UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW);
    mgr->drawTopRight();
    presentTopRight_();
    mBtmFrameBuffer->bind();
    clearFrameBuffers_(DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW);
    mgr->drawBtm();
    presentBtm_();
    mTopFrameBuffer->bind();
}

// 0x0074E5FC slot 0x18 (name is ours)
sead::FrameBuffer* sead::UlcdDoubleCmdGameFrameworkCtrNw4c::getMethodFrameBuffer(s32 methodType) const
{
    // the types of both images of the top screen
    if (methodType >= UlcdMethodTreeMgr::METHOD_LEFT_SYS_DRAW && methodType <= UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW_FINAL) {
        return mTopFrameBuffer;
    }
    return GameFrameworkCtrNw4c::getMethodFrameBuffer(methodType);
}
} // namespace sead

// the type information of the classes below DoubleCmdGameFrameworkCtrNw4c
// 0x0082954C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DoubleCmdGameFrameworkCtrNw4c>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
// the type information of the classes below UlcdDoubleCmdGameFrameworkCtrNw4c (the framework of the game)
// 0x008295C0 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::UlcdDoubleCmdGameFrameworkCtrNw4c>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
