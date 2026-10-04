#include "sead/seadMessageQueue.h"
#include "pead/peadINamable.h"
#include "pead/peadIDisposer.h"
#include "pead/hostio/peadReflexible.h"
#include "pead/peadThread.h"

namespace pead {
// ctor address unknown
pead::Thread::Thread()
{
}

// 0x0053C574 slot 0x00 | nintendogs:callgraph
pead::Thread::~Thread()
{
}

// 0x0053C2D0 slot 0x08 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x08()
{
}

// 0x005383C4 slot 0x0C | nintendogs:bytes
void pead::Thread::sendMessage(int, sead::MessageQueue::BlockType)
{
}

// 0x0053BFA0 slot 0x10 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x10()
{
}

// 0x0053C1C0 slot 0x14 | nintendogs:bytes
void pead::Thread::start()
{
}

// 0x0053C0F0 slot 0x18 | virtual slot, introduced by pead::Thread
void pead::Thread::quit(bool)
{
}

// 0x0053C2DC slot 0x1C | virtual slot, introduced by pead::Thread
void pead::Thread::waitDone()
{
}

// 0x0053C0B8 slot 0x20 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x20()
{
}

// 0x0053C0C4 slot 0x24 | nintendogs:bytes
void pead::Thread::quitAndWaitDoneSingleThread(bool)
{
}

// 0x0053BFBC slot 0x28 | nintendogs:bytes
void pead::Thread::setPriority(int)
{
}

// 0x00749534 slot 0x2C | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x2C()
{
}

// 0x0074953C slot 0x30 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x30()
{
}

// 0x00749544 slot 0x34 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x34()
{
}

// 0x0074954C slot 0x38 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x38()
{
}

// 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
void pead::Thread::vf_0x3C()
{
}

// 0x007495BC slot 0x40 | virtual slot, introduced by pead::Thread
void pead::Thread::vf_0x40()
{
}

} // namespace pead
