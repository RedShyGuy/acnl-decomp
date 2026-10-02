#include "sead/seadMessageQueue.h"

namespace sead {
// 0x00138BF0 | nintendogs:bytes [tier A]
void sead::MessageQueue::allocate(int, sead::Heap*)
{
}

// 0x001428C4 | nintendogs:bytes [tier A]
void sead::MessageQueue::pop(sead::MessageQueue::BlockType)
{
}

// 0x00540F60 | nintendogs:bytes [tier A]
void sead::MessageQueue::free()
{
}

// 0x00540FE4 | nintendogs:bytes [tier A]
sead::MessageQueue::~MessageQueue()
{
}

} // namespace sead
