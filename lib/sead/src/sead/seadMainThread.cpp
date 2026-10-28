#include "sead/seadMainThread.h"

namespace sead {
// 0x0053F224 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0053F1F8 (deleting dtor)
sead::MainThread::~MainThread()
{
    mState = STATE_TERMINATED;
}

// 0x0053F1F0 slot 0x08
void sead::MainThread::destroy()
{
    // empty (in the original too)
}

// 0x0053F1E8 slot 0x1C
void sead::MainThread::quit(bool)
{
    // empty (in the original too)
}

// 0x0053F1F4 slot 0x20
void sead::MainThread::waitDone()
{
    // empty (in the original too)
}

// 0x0053F1E4 slot 0x24
void sead::MainThread::quitAndDestroySingleThread(bool)
{
    // empty (in the original too)
}

// 0x0053F1E0 slot 0x2C
void sead::MainThread::setPriority(s32)
{
    // empty (in the original too)
}

// 0x0053F1EC slot 0x40
void sead::MainThread::calc_(MessageQueue::Element)
{
    // empty (in the original too)
}

} // namespace sead
