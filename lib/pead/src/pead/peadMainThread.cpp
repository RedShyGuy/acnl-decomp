#include "pead/peadMainThread.h"

namespace pead {
// 0x00537FD4 slot 0x28
void pead::MainThread::setPriority(s32)
{
    // empty (in the original too)
}

// 0x00537FD8 slot 0x20
void pead::MainThread::quitAndDestroySingleThread(bool)
{
    // empty (in the original too)
}

// 0x00537FDC slot 0x18
void pead::MainThread::quit(bool)
{
    // empty (in the original too)
}

// 0x00537FE0 slot 0x3C
void pead::MainThread::calc_(MessageQueue::Element)
{
    // empty (in the original too)
}

// 0x00537FE4 slot 0x08
void pead::MainThread::destroy()
{
    // empty (in the original too)
}

// 0x00537FE8 slot 0x1C
void pead::MainThread::waitDone()
{
    // empty (in the original too)
}

// 0x00538014 slot 0x00 | nintendogs:bytes
// 0x00537FEC (deleting dtor)
pead::MainThread::~MainThread()
{
    mState = STATE_TERMINATED;
}
} // namespace pead
