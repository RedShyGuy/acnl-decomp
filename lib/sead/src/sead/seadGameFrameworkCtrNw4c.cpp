#include "sead/seadGameFramework.h"
#include "sead/seadGameFrameworkCtrNw4c.h"

namespace sead {
// ctor candidate(s) 0x00125BAC (unverified)
sead::GameFrameworkCtrNw4c::GameFrameworkCtrNw4c()
{
}

// 0x0074D388 slot 0x00 | virtual slot, introduced by sead::Framework
void sead::GameFrameworkCtrNw4c::vf_0x00()
{
}

// 0x0074D204 slot 0x04 | virtual slot, introduced by sead::Framework
void sead::GameFrameworkCtrNw4c::vf_0x04()
{
}

// 0x0054B11C slot 0x08 | slot vf_0x08 of sead::Framework
sead::GameFrameworkCtrNw4c::~GameFrameworkCtrNw4c()
{
}

// 0x0074D250 slot 0x18 | virtual slot, introduced by sead::Framework
void sead::GameFrameworkCtrNw4c::vf_0x18()
{
}

// 0x0054AEF0 slot 0x28 | virtual slot, introduced by sead::Framework
void sead::GameFrameworkCtrNw4c::vf_0x28()
{
}

// 0x0054AEF4 slot 0x2C | slot vf_0x2C of sead::Framework
void sead::GameFrameworkCtrNw4c::runImpl_()
{
}

// 0x0054CFE0 slot 0x30 | slot vf_0x30 of sead::Framework
void sead::GameFrameworkCtrNw4c::createMethodTreeMgr_(sead::Heap*)
{
}

// 0x0054AEA0 slot 0x4C | virtual slot, introduced by sead::GameFramework
void sead::GameFrameworkCtrNw4c::vf_0x4C()
{
}

// 0x0054ABF0 slot 0x50 | virtual slot, introduced by sead::GameFramework
void sead::GameFrameworkCtrNw4c::vf_0x50()
{
}

// 0x0074D1FC slot 0x54 | virtual slot, introduced by sead::GameFramework
void sead::GameFrameworkCtrNw4c::vf_0x54()
{
}

// 0x0054AB44 slot 0x5C | virtual slot, introduced by sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::vf_0x5C()
{
}

// 0x0054AF20 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::mainLoop_()
{
}

// 0x0054AA94 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procFrame_()
{
}

// 0x0054B010 slot 0x68 | slot vf_0x68 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procDraw_()
{
}

// 0x0054AF80 slot 0x6C | slot vf_0x6C of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::procCalc_()
{
}

// 0x00121838 slot 0x70 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::presentTop_()
{
}

// 0x0054AB48 slot 0x74 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::presentBtm_()
{
}

// 0x0054ABAC slot 0x78 | nintendogs:callseq
void sead::GameFrameworkCtrNw4c::swapBuffer_()
{
}

// 0x0054ABF4 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::waitForVBlank_()
{
}

// 0x0054AD18 slot 0x80 | slot vf_0x80 of sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::clearFrameBuffers_(int)
{
}

// 0x0054ABEC slot 0x84 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::vf_0x84()
{
}

// 0x0054AD14 slot 0x88 | virtual slot, introduced by sead::GameFrameworkCtrNw4c
void sead::GameFrameworkCtrNw4c::vf_0x88()
{
}

// 0x0011E9F4 | nintendogs:bytes [tier A]
void sead::GameFrameworkCtrNw4c::createCmdlist_(unsigned, unsigned)
{
}

// 0x0011EA28 | nintendogs:bytes-fuzzy [tier A]
void sead::GameFrameworkCtrNw4c::createFramebuffer_(nn::gr::CTR::FrameBuffer*, int, int, unsigned, PicaDataColor, unsigned, PicaDataDepth)
{
}

// 0x0011EB10 | nintendogs:bytes [tier A]
void sead::GameFrameworkCtrNw4c::createDisplaybuffers_(unsigned*, unsigned, unsigned, unsigned, int, int, unsigned)
{
}

// 0x0011EB7C | nintendogs:bytes [tier A]
void sead::GameFrameworkCtrNw4c::initNngx_(sead::GfxMemoryMgrCtr*)
{
}

// 0x00120A2C | nintendogs:bytes [tier A]
void sead::GameFrameworkCtrNw4c::deallocate(unsigned, unsigned, unsigned, void*)
{
}

// 0x0054AD5C | nintendogs:callseq [tier A]
void sead::GameFrameworkCtrNw4c::requestTransferRenderImage_(unsigned, const nn::gr::CTR::FrameBuffer&, int, int, unsigned, bool)
{
}

} // namespace sead
