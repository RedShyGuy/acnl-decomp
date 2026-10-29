#include "nn/gx/gx_Api.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/svc/svc_Api.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"
#include "sead/seadFrameBuffer.h"
#include "sead/seadGameFramework.h"
#include "sead/seadGameFrameworkCtrNw4c.h"
#include "sead/seadGfxMemoryMgrCtr.h"
#include "sead/seadGraphics.h"
#include "sead/seadTaskMgr.h"
#include "sead/seadThread.h"

namespace sead {
// 0x0097630C
GfxMemoryMgrCtr* GameFrameworkCtrNw4c::s_pGfxMemoryMgr = nullptr;

// 0x0011E9F4 | nintendogs:bytes [tier A]
GLuint sead::GameFrameworkCtrNw4c::createCmdlist_(u32 bufferSize, u32 requestCount)
{
    GLuint name;
    nngxGenCmdlists(1, &name);
    nngxBindCmdlist(name);
    nngxCmdlistStorage(bufferSize, requestCount);
    return name;
}

// 0x0011EA28 | nintendogs:bytes-fuzzy [tier A]
void sead::GameFrameworkCtrNw4c::createFramebuffer_(nn::gr::CTR::FrameBuffer* frameBuffer, int width, int height,
                                                   u32 colorArea, PicaDataColor colorFormat, u32 depthArea,
                                                   PicaDataDepth depthFormat)
{
    // bytes per pixel: RGBA8 4, RGB5A1/RGB565/RGBA4 2
    u32 colorBytes;
    if (colorFormat == 0) {
        colorBytes = 4;
    } else if (colorFormat == 2 || colorFormat == 3 || colorFormat == 4) {
        colorBytes = 2;
    } else {
        colorBytes = 4;
    }
    frameBuffer->m_ColorBuffer.m_VirtualAddr =
        reinterpret_cast<u32>(s_pGfxMemoryMgr->allocate(colorArea, 0x103, 0, colorBytes * width * height));
    frameBuffer->m_ColorBuffer.m_Format = colorFormat;
    frameBuffer->m_ColorBuffer.m_Unknown5 = 0;
    frameBuffer->m_ColorBuffer.m_Width = width;
    frameBuffer->m_ColorBuffer.m_Height = height;
    // D16 2, D24 3, D24S8 4
    u32 depthBytes;
    if (depthFormat == 0) {
        depthBytes = 2;
    } else if (depthFormat == 2) {
        depthBytes = 3;
    } else {
        depthBytes = 4;
    }
    frameBuffer->m_DepthStencilBuffer.m_VirtualAddr =
        reinterpret_cast<u32>(s_pGfxMemoryMgr->allocate(depthArea, 0x103, 0, depthBytes * width * height));
    frameBuffer->m_DepthStencilBuffer.m_Format = depthFormat;
    frameBuffer->m_DepthStencilBuffer.m_Width = width;
    frameBuffer->m_DepthStencilBuffer.m_Height = height;
    frameBuffer->m_Width = width;
    frameBuffer->m_Height = height;
}

// 0x0011EB10 | nintendogs:bytes [tier A]
u32 sead::GameFrameworkCtrNw4c::createDisplaybuffers_(u32* names, u32 num, GLenum display, GLenum format, int width,
                                                     int height, GLenum area)
{
    nngxGenDisplaybuffers(num, names);
    nngxActiveDisplay(display);
    for (u32 i = 0; i < num; i++) {
        nngxBindDisplaybuffer(names[i]);
        nngxDisplaybufferStorage(format, width, height, area);
    }
    return num;
}

// 0x0011EB7C | nintendogs:bytes [tier A]
bool sead::GameFrameworkCtrNw4c::initNngx_(GfxMemoryMgrCtr* mgr)
{
    s_pGfxMemoryMgr = mgr;
    return nngxInitialize(&allocate_, &deallocate_) != 0;
}

// 0x001209F4 (name is ours)
void sead::GameFrameworkCtrNw4c::deallocate_(GLenum area, GLenum kind, GLuint name, void* addr)
{
    s_pGfxMemoryMgr->deallocate(area, kind, name, addr);
}

// 0x00120A2C (name is ours)
void* sead::GameFrameworkCtrNw4c::allocate_(GLenum area, GLenum kind, GLuint name, GLsizei size)
{
    return s_pGfxMemoryMgr->allocate(area, kind, name, size);
}

// 0x00121838 slot 0x70 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::presentTop_()
{
    nngxActiveDisplay(0x400);
    requestTransferRenderImage_(mTopDisplayBuffers[mDisplayBufferIndex], mGrFrameBuffer, 0, 0, 0x500, false);
    nngxActiveDisplay(0x400);
    nngxBindDisplaybuffer(mTopDisplayBuffers[mDisplayBufferIndex]);
}

// (out of line: the derived constructors call it)
// 0x00125BAC (name is ours)
DECOMP_NOINLINE sead::GameFrameworkCtrNw4c::GameFrameworkCtrNw4c(const CreateArg& arg)
    : GameFramework(), mArg(arg), mLastVSyncCount(0), mFrameSpan(0), mLastFrameTick(nn::svc::GetSystemTick()),
      mLastSwapTick(nn::svc::GetSystemTick()), mCmdList(0), mGrFrameBuffer(), mTopFrameBuffer(nullptr),
      mBtmFrameBuffer(nullptr), mDisplayBufferIndex(0)
{
}

// 0x0054AA94 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procFrame_()
{
    Graphics::s_pInstance->lockDrawContext();
    nngxRunCmdlist();
    procDraw_();
    procCalc_();
    procReset_();
    swapBuffer_();
    Graphics::s_pInstance->unlockDrawContext();
    s64 tick = nn::svc::GetSystemTick();
    mFrameSpan = tick - mLastFrameTick;
    mLastFrameTick = nn::svc::GetSystemTick();
    waitForVBlank_();
    if (mDisplayState == 1) {
        nn::gxlow::CTR::StartLcdDisplayEntry();
        mDisplayState = 2;
    }
}

// 0x0054AB44 slot 0x5C
void sead::GameFrameworkCtrNw4c::vf_0x5C()
{
    // empty (in the original too)
}

// 0x0054AB48 slot 0x74 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::presentBtm_()
{
    nngxActiveDisplay(0x401);
    requestTransferRenderImage_(mBtmDisplayBuffers[mDisplayBufferIndex], mGrFrameBuffer, 0, 0, 0x500, false);
    nngxActiveDisplay(0x401);
    nngxBindDisplaybuffer(mBtmDisplayBuffers[mDisplayBufferIndex]);
}

// 0x0054ABAC slot 0x78 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::swapBuffer_()
{
    nngxRunCmdlist();
    GLint isRunning;
    nngxGetCmdlistParameteri(0x201, &isRunning);
    if (isRunning != 0) {
        nngxWaitCmdlistDone();
    }
    nngxStopCmdlist();
    nngxClearCmdlist();
    mDisplayBufferIndex = 1 - mDisplayBufferIndex;
}

// 0x0054ABEC slot 0x84
void sead::GameFrameworkCtrNw4c::vf_0x84()
{
    // empty (in the original too)
}

// 0x0054ABF0 slot 0x50
void sead::GameFrameworkCtrNw4c::vf_0x50()
{
    // empty (in the original too)
}

// 0x0054ABF4 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::waitForVBlank_()
{
    Graphics::s_pInstance->waitForVBlankImpl();
    TickSpan sleepSpan = TickSpan::fromMilliSeconds(10);
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
            mLastSwapTick = nn::svc::GetSystemTick();
            return;
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
}

// 0x0054AD14 slot 0x88
void sead::GameFrameworkCtrNw4c::vf_0x88()
{
    // empty (in the original too)
}

// 0x0054AD18 slot 0x80 | slot vf_0x80 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::clearFrameBuffers_(int methodType)
{
    getMethodFrameBuffer(methodType)->bind();
    Graphics::s_pInstance->clearFrameBuffer(7, mArg.clearColor, 1.0f, 0);
}

// 0x0054AD5C | nintendogs:callseq [tier A]
void sead::GameFrameworkCtrNw4c::requestTransferRenderImage_(u32 displayBuffer,
                                                            const nn::gr::CTR::FrameBuffer& frameBuffer, int x, int y,
                                                            u32 aaMode, bool isFlipped)
{
    nngxBindDisplaybuffer(displayBuffer);
    GLint address;
    GLint format;
    GLint width;
    GLint height;
    nngxGetDisplaybufferParameteri(0x600, &address);
    nngxGetDisplaybufferParameteri(0x601, &format);
    nngxGetDisplaybufferParameteri(0x602, &width);
    nngxGetDisplaybufferParameteri(0x603, &height);
    const nn::gr::CTR::FrameBuffer::ColorBuffer& colorBuffer = frameBuffer.m_ColorBuffer;
    u32 bytes;
    if (colorBuffer.m_Format == 0) {
        bytes = 4;
    } else if (colorBuffer.m_Format == 2 || colorBuffer.m_Format == 3 || colorBuffer.m_Format == 4) {
        bytes = 2;
    } else {
        bytes = 4;
    }
    // the image starts at (x, y) from the bottom of the buffer
    u32 source =
        colorBuffer.m_VirtualAddr + (colorBuffer.m_Width * (colorBuffer.m_Height - height - y) + x) * bytes;
    nngxFlush3DCommand();
    GLsizei blockSize = colorBuffer.m_Unknown5 != 0 ? 32 : 8;
    // GL_RGBA8_OES, GL_RGB5_A1, GL_RGB565, GL_RGBA4
    GLenum sourceFormat;
    if (colorBuffer.m_Format == 0) {
        sourceFormat = 0x8058;
    } else if (colorBuffer.m_Format == 2) {
        sourceFormat = 0x8057;
    } else if (colorBuffer.m_Format == 3) {
        sourceFormat = 0x8D62;
    } else if (colorBuffer.m_Format == 4) {
        sourceFormat = 0x8056;
    } else {
        sourceFormat = 0;
    }
    nngxAddB2LTransferCommand(reinterpret_cast<const GLvoid*>(source), colorBuffer.m_Width, colorBuffer.m_Height,
                              sourceFormat, reinterpret_cast<GLvoid*>(address), width, height, format, aaMode,
                              isFlipped, blockSize);
}

// 0x0054AEA0 slot 0x4C (name is ours)
f32 sead::GameFrameworkCtrNw4c::calcFps() const
{
    return static_cast<f32>(TickSpan::fromSeconds(1).mSpan) / static_cast<f32>(mFrameSpan);
}

// 0x0054AEF0 slot 0x28 (name is ours)
void sead::GameFrameworkCtrNw4c::initRun_(Heap*)
{
    // empty (in the original too)
}

// 0x0054AEF4 slot 0x2C | slot vf_0x2C of sead::Framework
void sead::GameFrameworkCtrNw4c::runImpl_()
{
    waitStartDisplayLoop_();
    mainLoop_();
}

// 0x0054AF20 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::mainLoop_()
{
    mLastVSyncCount = nngxCheckVSync(0x402);
    mLastFrameTick = nn::svc::GetSystemTick();
    mLastSwapTick = nn::svc::GetSystemTick();
    Graphics::s_pInstance->lockDrawContext();
    procCalc_();
    Graphics::s_pInstance->unlockDrawContext();
    for (;;) {
        procFrame_();
    }
}

// 0x0054AF80 slot 0x6C | slot vf_0x6C of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procCalc_()
{
    mTaskMgr->calcCreation_();
    DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr)->calc();
    mTaskMgr->calcDestruction_();
}

// 0x0054B010 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procDraw_()
{
    DualScreenMethodTreeMgr* mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
    clearFrameBuffers_(DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW);
    mTopFrameBuffer->bind();
    mgr->drawTop();
    presentTop_();
    clearFrameBuffers_(DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW);
    mBtmFrameBuffer->bind();
    mgr->drawBtm();
    presentBtm_();
    mTopFrameBuffer->bind();
}

// 0x0054B11C slot 0x08
// 0x0054B100 (deleting dtor)
sead::GameFrameworkCtrNw4c::~GameFrameworkCtrNw4c()
{
    // empty (in the original too)
}

// 0x0054CFE0 slot 0x30
sead::MethodTreeMgr* sead::GameFrameworkCtrNw4c::createMethodTreeMgr_(Heap* heap)
{
    return new (heap, 4) DualScreenMethodTreeMgr();
}

// 0x0074D250 slot 0x18 (name is ours)
sead::FrameBuffer* sead::GameFrameworkCtrNw4c::getMethodFrameBuffer(s32 methodType) const
{
    switch (methodType) {
    case DualScreenMethodTreeMgr::METHOD_SYS_DRAW: {
        DualScreenMethodTreeMgr* mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
        return mgr->mSysDrawScreen != DualScreenMethodTreeMgr::SCREEN_TOP ? mBtmFrameBuffer : mTopFrameBuffer;
    }
    case DualScreenMethodTreeMgr::METHOD_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_APP_DRAW_FINAL: {
        DualScreenMethodTreeMgr* mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
        return mgr->mAppDrawScreen != DualScreenMethodTreeMgr::SCREEN_TOP ? mBtmFrameBuffer : mTopFrameBuffer;
    }
    case DualScreenMethodTreeMgr::METHOD_TOP_SYS_DRAW:
    case DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW_FINAL:
        return mTopFrameBuffer;
    case DualScreenMethodTreeMgr::METHOD_BTM_SYS_DRAW:
    case DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW_FINAL:
        return mBtmFrameBuffer;
    default:
        return nullptr;
    }
}
} // namespace sead

// the type information of the classes below GameFramework
// 0x00828CB4 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::GameFramework>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
