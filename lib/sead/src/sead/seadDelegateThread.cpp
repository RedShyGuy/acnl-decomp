#include "sead/seadDelegateThread.h"

namespace sead {
// 0x00544838 (name is ours)
sead::DelegateThread::DelegateThread(const SafeStringBase<char>& name, IDelegate2<Thread*, MessageQueue::Element>* delegate,
                                     Heap* heap, s32 priority, MessageQueue::BlockType blockType,
                                     MessageQueue::Element quitMessage, s32 stackSize, s32 messageQueueSize)
    : Thread(name, heap, priority, blockType, quitMessage, stackSize, messageQueueSize), mDelegate(delegate)
{
}

// 0x0054488C slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0054487C (deleting dtor)
sead::DelegateThread::~DelegateThread()
{
    // empty (in the original too)
}

// 0x0054481C slot 0x40
void sead::DelegateThread::calc_(MessageQueue::Element message)
{
    mDelegate->invoke(this, message);
}

} // namespace sead
