#include "sead/seadThread.h"
#include "qrdecode/dThread.h"

namespace qrdecode {
// ctor address unknown
qrdecode::Thread::Thread()
{
}

// 0x006CAD0C slot 0x00 | slot vf_0x00 of sead::IDisposer
qrdecode::Thread::~Thread()
{
}

// 0x006CACC4 slot 0x40 | virtual slot, introduced by sead::Thread
void qrdecode::Thread::vf_0x40()
{
}

} // namespace qrdecode
