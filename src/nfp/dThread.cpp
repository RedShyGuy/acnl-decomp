#include "sead/seadThread.h"
#include "nfp/dThread.h"

namespace nfp {
// ctor address unknown
nfp::Thread::Thread()
{
}

// 0x0051BA64 slot 0x00 | slot vf_0x00 of sead::IDisposer
nfp::Thread::~Thread()
{
}

// 0x0051B9E8 slot 0x40 | virtual slot, introduced by sead::Thread
void nfp::Thread::vf_0x40()
{
}

} // namespace nfp
