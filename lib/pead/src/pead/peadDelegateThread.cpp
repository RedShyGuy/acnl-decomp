#include "pead/peadThread.h"
#include "pead/peadDelegateThread.h"

namespace pead {
// 0x00538788 (name is ours)
pead::DelegateThread::DelegateThread(const SafeStringBase<char>&, IDelegate2<Thread*, int>*, Heap*, int, int, int, int, int)
{
}

// 0x0053C570 slot 0x00 | slot vf_0x00 of pead::IDisposer
pead::DelegateThread::~DelegateThread()
{
}

// 0x0053876C slot 0x3C | virtual slot, introduced by pead::Thread
void pead::DelegateThread::vf_0x3C()
{
}

} // namespace pead
