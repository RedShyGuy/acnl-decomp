#include "pead/peadDelegateThread.h"

namespace pead {
// 0x0053876C slot 0x3C
void pead::DelegateThread::calc_(MessageQueue::Element message)
{
    mDelegate->invoke(this, message);
}

// 0x00538788 (name after sead)
pead::DelegateThread::DelegateThread(const SafeStringBase<char>& name,
                                     IDelegate2<Thread*, MessageQueue::Element>* delegate, Heap* heap, s32 priority,
                                     MessageQueue::BlockType blockType, MessageQueue::Element quitMessage,
                                     s32 stackSize, s32 messageQueueSize)
    : Thread(name, heap, priority, blockType, quitMessage, stackSize, messageQueueSize), mDelegate(delegate)
{
}

// 0x0053C570 slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x005387C4 (deleting dtor)
pead::DelegateThread::~DelegateThread()
{
    // empty (in the original too)
}
} // namespace pead
