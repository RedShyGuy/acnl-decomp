#pragma once

#include "decomp.h"
#include "sead/seadGameFramework.h"

namespace sead {
// RTTI N4sead20GameFrameworkCtrNw4cE @ 0x008D1E4C
// vtable 0x00906488 (vptr 0x00906490), offset_to_top 0, 35 entries
class GameFrameworkCtrNw4c : public ::sead::GameFramework
{
public:
    GameFrameworkCtrNw4c(); // ctor candidate(s) 0x00125BAC (unverified)
    virtual void vf_0x00(); // 0x0074D388 slot 0x00 | virtual slot, introduced by sead::Framework
    virtual void vf_0x04(); // 0x0074D204 slot 0x04 | virtual slot, introduced by sead::Framework
    virtual ~GameFrameworkCtrNw4c(); // 0x0054B11C slot 0x08 | slot vf_0x08 of sead::Framework
    // 0x0054B100 slot 0x0C | slot vf_0x0C of sead::Framework (deleting dtor)
    virtual void vf_0x18(); // 0x0074D250 slot 0x18 | virtual slot, introduced by sead::Framework
    virtual void vf_0x28(); // 0x0054AEF0 slot 0x28 | virtual slot, introduced by sead::Framework
    virtual void runImpl_(); // 0x0054AEF4 slot 0x2C | slot vf_0x2C of sead::Framework
    virtual void createMethodTreeMgr_(sead::Heap*); // 0x0054CFE0 slot 0x30 | slot vf_0x30 of sead::Framework
    virtual void vf_0x4C(); // 0x0054AEA0 slot 0x4C | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x50(); // 0x0054ABF0 slot 0x50 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x54(); // 0x0074D1FC slot 0x54 | virtual slot, introduced by sead::GameFramework
    virtual void vf_0x5C(); // 0x0054AB44 slot 0x5C | virtual slot, introduced by sead::GameFrameworkCtrNw4c
    virtual void mainLoop_(); // 0x0054AF20 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
    virtual void procFrame_(); // 0x0054AA94 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
    virtual void procDraw_(); // 0x0054B010 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
    virtual void procCalc_(); // 0x0054AF80 slot 0x6C | slot vf_0x6C of sead::GameFrameworkCtrNw4c
    virtual void presentTop_(); // 0x00121838 slot 0x70 | nintendogs:callseq
    virtual void presentBtm_(); // 0x0054AB48 slot 0x74 | nintendogs:callseq
    virtual void swapBuffer_(); // 0x0054ABAC slot 0x78 | nintendogs:callseq
    virtual void waitForVBlank_(); // 0x0054ABF4 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
    virtual void clearFrameBuffers_(int); // 0x0054AD18 slot 0x80 | slot vf_0x80 of sead::GameFrameworkCtrNw4c
    virtual void vf_0x84(); // 0x0054ABEC slot 0x84 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
    virtual void vf_0x88(); // 0x0054AD14 slot 0x88 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
    void createCmdlist_(unsigned, unsigned); // 0x0011E9F4 | nintendogs:bytes [tier A]
    void createFramebuffer_(nn::gr::CTR::FrameBuffer*, int, int, unsigned, PicaDataColor, unsigned, PicaDataDepth); // 0x0011EA28 | nintendogs:bytes-fuzzy [tier A]
    void createDisplaybuffers_(unsigned*, unsigned, unsigned, unsigned, int, int, unsigned); // 0x0011EB10 | nintendogs:bytes [tier A]
    void initNngx_(sead::GfxMemoryMgrCtr*); // 0x0011EB7C | nintendogs:bytes [tier A]
    void deallocate(unsigned, unsigned, unsigned, void*); // 0x00120A2C | nintendogs:bytes [tier A]
    void requestTransferRenderImage_(unsigned, const nn::gr::CTR::FrameBuffer&, int, int, unsigned, bool); // 0x0054AD5C | nintendogs:callseq [tier A]
};
} // namespace sead
