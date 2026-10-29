#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/svc/svc_Api.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDoubleCmdGameFrameworkCtrNw4c.h"
#include "sead/seadGameFrameworkCtrNw4c.h"
#include "sead/seadGraphics.h"
#include "sead/seadThread.h"

namespace sead {
// 0x00976308
DoubleCmdGameFrameworkCtrNw4c* DoubleCmdGameFrameworkCtrNw4c::s_pInstance = nullptr;

// 0x0011EC1C (name is ours)
void sead::DoubleCmdGameFrameworkCtrNw4c::cmdListCallback_(s32 id)
{
    s_pInstance->onCmdListDone_(id);
}

// (out of line: the derived constructors call it)
// 0x00120A64 (name is ours)
DECOMP_NOINLINE sead::DoubleCmdGameFrameworkCtrNw4c::DoubleCmdGameFrameworkCtrNw4c(const CreateArg& arg)
    : GameFrameworkCtrNw4c(arg), mCmdListIndex(0), mFrameCount(0), mCmdListDoneTick(nn::svc::GetSystemTick()),
      mVBlankDone(false)
{
    s_pInstance = this;
}

// 0x0054DAF0 slot 0x64
void sead::DoubleCmdGameFrameworkCtrNw4c::procFrame_()
{
    Graphics::s_pInstance->lockDrawContext();
    nngxBindCmdlist(mCmdLists[mCmdListIndex]);
    procCalc_();
    procDraw_();
    swapBuffer_();
    Graphics::s_pInstance->unlockDrawContext();
    if (mVBlankDone) {
        mVBlankDone = false;
    }
    s64 tick = nn::svc::GetSystemTick();
    mFrameSpan = tick - mLastFrameTick;
    mLastFrameTick = nn::svc::GetSystemTick();
    waitForVBlank_();
    Graphics::s_pInstance->lockDrawContext();
    if (!mVBlankDone) {
        mVBlankDone = true;
    }
    nngxBindCmdlist(mCmdLists[mCmdListIndex]);
    nngxRunCmdlist();
    mCmdListIndex = 1 - mCmdListIndex;
    nngxBindCmdlist(mCmdLists[mCmdListIndex]);
    mDisplayBufferIndex = (mDisplayBufferIndex + 1) % 3;
    Graphics::s_pInstance->unlockDrawContext();
    if (mDisplayState == 1 && mFrameCount == 2) {
        nn::gxlow::CTR::StartLcdDisplayEntry();
        mDisplayState = 2;
    }
    if (mFrameCount <= 2) {
        mFrameCount++;
    }
}

// 0x0054DC34 slot 0x74
void sead::DoubleCmdGameFrameworkCtrNw4c::presentBtm_()
{
    requestTransferRenderImage_(mBtmDisplayBuffers3[mDisplayBufferIndex], mGrFrameBuffer, mArg.btmTransferX,
                                mArg.btmTransferY, 0x500, false);
}

// 0x0054DC6C slot 0x70
void sead::DoubleCmdGameFrameworkCtrNw4c::presentTop_()
{
    requestTransferRenderImage_(mTopDisplayBuffers3[mDisplayBufferIndex], mGrFrameBuffer, mArg.topTransferX,
                                mArg.topTransferY, 0x500, false);
}

// 0x0054DCA4 slot 0x78 | nintendogs:callseq
void sead::DoubleCmdGameFrameworkCtrNw4c::swapBuffer_()
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
    nngxActiveDisplay(0x401);
    nngxBindDisplaybuffer(mBtmDisplayBuffers3[index]);
    nngxBindCmdlist(mCmdLists[mCmdListIndex]);
}

// 0x0054DD50 slot 0x7C
void sead::DoubleCmdGameFrameworkCtrNw4c::waitForVBlank_()
{
    TickSpan sleepSpan = TickSpan::fromMilliSeconds(10);
    if (mFrameCount > 0) {
        for (;;) {
            s32 vsyncCount = nngxCheckVSync(0x402);
            u32 count;
            if (mLastVSyncCount > vsyncCount) {
                count = 0x7FFFFFFE - mLastVSyncCount + vsyncCount;
            } else {
                count = vsyncCount - mLastVSyncCount;
            }
            if (count >= mArg.waitVBlankCount) {
                Graphics::s_pInstance->lockDrawContext();
                nngxSwapBuffers(0x402);
                Graphics::s_pInstance->unlockDrawContext();
                mLastVSyncCount = vsyncCount;
                break;
            }
            if (count + 1 == mArg.waitVBlankCount) {
                Graphics::s_pInstance->lockDrawContext();
                nngxSwapBuffers(0x402);
                Graphics::s_pInstance->unlockDrawContext();
                Graphics::s_pInstance->waitForVBlankImpl();
            } else {
                Thread::sleep(sleepSpan);
            }
        }
    } else {
        s32 vsyncCount = nngxCheckVSync(0x402);
        u32 count;
        if (mLastVSyncCount > vsyncCount) {
            count = 0x7FFFFFFE - mLastVSyncCount + vsyncCount;
        } else {
            count = vsyncCount - mLastVSyncCount;
        }
        for (; count < mArg.waitVBlankCount; count++) {
            Graphics::s_pInstance->waitForVBlankImpl();
        }
        mLastVSyncCount = nngxCheckVSync(0x402);
    }
    mLastSwapTick = nn::svc::GetSystemTick();
}

// 0x0054DEC8 slot 0x88
void sead::DoubleCmdGameFrameworkCtrNw4c::vf_0x88()
{
    // empty (in the original too)
}

// 0x0054DECC slot 0x8C (name is ours)
void sead::DoubleCmdGameFrameworkCtrNw4c::onCmdListDone_(s32)
{
    mCmdListDoneTick = nn::svc::GetSystemTick();
}

// 0x0054DEE4 slot 0x60
void sead::DoubleCmdGameFrameworkCtrNw4c::mainLoop_()
{
    mLastVSyncCount = nngxCheckVSync(0x402);
    mLastFrameTick = nn::svc::GetSystemTick();
    mLastSwapTick = nn::svc::GetSystemTick();
    for (;;) {
        procFrame_();
    }
}

// 0x0054DF60 slot 0x08
// 0x0054DF1C (deleting dtor)
sead::DoubleCmdGameFrameworkCtrNw4c::~DoubleCmdGameFrameworkCtrNw4c()
{
    if (s_pInstance != nullptr) {
        s_pInstance = nullptr;
    }
}
} // namespace sead

// the type information of the classes below GameFrameworkCtrNw4c
// 0x00829298 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::GameFrameworkCtrNw4c>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
