#pragma once

#include "decomp.h"
#include "nn/gr/CTR/gr_FrameBuffer.h"
#include "nn/gx/gx_Types.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadColor4f.h"
#include "sead/seadGameFramework.h"

namespace sead {
class GfxMemoryMgrCtr;

// RTTI N4sead20GameFrameworkCtrNw4cE @ 0x008D1E4C
// vtable 0x00906488 (vptr 0x00906490), offset_to_top 0, 35 entries
// The game framework of the 3DS: one command list, two display buffers per screen, the main loop with
// calc, draw (top, then bottom screen), the transfer of the render image and the wait for the VBlank.
// The game sets up the buffers in its initRun_ with the static helpers. Names of the slots after the
// reference symbols where marked, the members and the other slots are ours.
class GameFrameworkCtrNw4c : public ::sead::GameFramework
{
public:
    // what the framework is made with (names are ours, after the game's values)
    struct CreateArg
    {
        u32 topFrameBufferWidth;   // 0x00 (256)
        u32 topFrameBufferHeight;  // 0x04 (416)
        u32 btmFrameBufferWidth;   // 0x08 (256)
        u32 btmFrameBufferHeight;  // 0x0C (320)
        s32 topTransferX;          // 0x10, where the top screen image starts in the frame buffer
        s32 topTransferY;          // 0x14
        s32 btmTransferX;          // 0x18
        s32 btmTransferY;          // 0x1C
        u32 waitVBlankCount;       // 0x20, VBlanks per frame
        Color4f clearColor;        // 0x24
        u32 cmdListBufferSize;     // 0x34
        u32 cmdListRequestCount;   // 0x38
        u32 field_0x3C;            // 0x3C
        u32 field_0x40;            // 0x40
        u32 field_0x44;            // 0x44
        u32 field_0x48;            // 0x48
        u32 field_0x4C;            // 0x4C
    };

    explicit GameFrameworkCtrNw4c(const CreateArg& arg); // 0x00125BAC
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D388 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return GameFramework::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D204 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983504 (guard 0x00983500)
        static const RuntimeTypeInfo::Derive<GameFramework> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~GameFrameworkCtrNw4c(); // 0x0054B11C slot 0x08
    // 0x0054B100 slot 0x0C (deleting dtor)
    virtual FrameBuffer* getMethodFrameBuffer(s32 methodType) const; // 0x0074D250 slot 0x18
    virtual void initRun_(Heap* heap); // 0x0054AEF0 slot 0x28
    virtual void runImpl_(); // 0x0054AEF4 slot 0x2C | slot vf_0x2C of sead::Framework
    virtual MethodTreeMgr* createMethodTreeMgr_(Heap* heap); // 0x0054CFE0 slot 0x30
    virtual f32 calcFps() const; // 0x0054AEA0 slot 0x4C
    virtual void vf_0x50(); // 0x0054ABF0 slot 0x50
    virtual u32 vf_0x54() const { return 0; } // 0x0074D1FC slot 0x54
    virtual void vf_0x5C(); // 0x0054AB44 slot 0x5C
    virtual void mainLoop_(); // 0x0054AF20 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
    virtual void procFrame_(); // 0x0054AA94 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
    virtual void procDraw_(); // 0x0054B010 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
    virtual void procCalc_(); // 0x0054AF80 slot 0x6C | slot vf_0x6C of sead::GameFrameworkCtrNw4c
    virtual void presentTop_(); // 0x00121838 slot 0x70 | nintendogs:callseq
    virtual void presentBtm_(); // 0x0054AB48 slot 0x74 | nintendogs:callseq
    virtual void swapBuffer_(); // 0x0054ABAC slot 0x78 | nintendogs:callseq
    virtual void waitForVBlank_(); // 0x0054ABF4 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
    // binds the frame buffer of the method type and clears it
    virtual void clearFrameBuffers_(int methodType); // 0x0054AD18 slot 0x80 | slot vf_0x80 of sead::GameFrameworkCtrNw4c
    virtual void vf_0x84(); // 0x0054ABEC slot 0x84
    virtual void vf_0x88(); // 0x0054AD14 slot 0x88

    // the command list (returns its name)
    // GameFramework::initialize (name is ours; a removed call before it)
    static void initialize(const InitializeArg& arg); // 0x0011EC14
    static GLuint createCmdlist_(u32 bufferSize, u32 requestCount); // 0x0011E9F4 | nintendogs:bytes [tier A]
    // a frame buffer with color and depth buffers from the graphics memory manager
    static void createFramebuffer_(nn::gr::CTR::FrameBuffer* frameBuffer, int width, int height, u32 colorArea,
                                   PicaDataColor colorFormat, u32 depthArea, PicaDataDepth depthFormat); // 0x0011EA28 | nintendogs:bytes-fuzzy [tier A]
    static u32 createDisplaybuffers_(u32* names, u32 num, GLenum display, GLenum format, int width, int height,
                                     GLenum area); // 0x0011EB10 | nintendogs:bytes [tier A]
    // nngx with the memory of mgr
    static bool initNngx_(GfxMemoryMgrCtr* mgr); // 0x0011EB7C | nintendogs:bytes [tier A]
    // the allocator functions of nngx (symbols.json calls 0x00120A2C deallocate: it allocates through
    // slot 0x08 of the manager; names are ours)
    static void* allocate_(GLenum area, GLenum kind, GLuint name, GLsizei size); // 0x00120A2C
    static void deallocate_(GLenum area, GLenum kind, GLuint name, void* addr); // 0x001209F4
    // copies the render image of frameBuffer into the display buffer
    static void requestTransferRenderImage_(u32 displayBuffer, const nn::gr::CTR::FrameBuffer& frameBuffer, int x,
                                            int y, u32 aaMode, bool isFlipped); // 0x0054AD5C | nintendogs:callseq [tier A]

    // the graphics memory manager of nngx (name is ours)
    static GfxMemoryMgrCtr* s_pGfxMemoryMgr;

    CreateArg mArg;                          // 0x050
    s32 mLastVSyncCount;                     // 0x0A0
    s64 mFrameSpan;                          // 0x0A8, ticks of the last frame
    s64 mLastFrameTick;                      // 0x0B0
    s64 mLastSwapTick;                       // 0x0B8
    GLuint mCmdList;                         // 0x0C0
    GLuint mTopDisplayBuffers[2];            // 0x0C4
    GLuint mBtmDisplayBuffers[2];            // 0x0CC
    nn::gr::CTR::FrameBuffer mGrFrameBuffer; // 0x0D4, the render target of both screens
    FrameBuffer* mTopFrameBuffer;            // 0x11C
    FrameBuffer* mBtmFrameBuffer;            // 0x120
    s32 mDisplayBufferIndex;                 // 0x124
};
ASSERT_SIZE(GameFrameworkCtrNw4c, 0x128);
} // namespace sead
