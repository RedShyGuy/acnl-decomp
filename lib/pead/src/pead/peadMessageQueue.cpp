#include "pead/peadHeapMgr.h"
#include "pead/peadMessageQueue.h"

namespace pead {
// 0x00538364 (name after sead)
bool pead::MessageQueue::jam(Element message, BlockType blockType)
{
    if (blockType != BLOCK_TYPE_BLOCK) {
        return mQueue.TryJam(message);
    }
    mQueue.Jam(message);
    return true;
}

// 0x0053838C (name after sead)
void pead::MessageQueue::free()
{
    mQueue.Finalize();
    if (mBuffer != nullptr) {
        DeleteArray(mBuffer);
        mBuffer = nullptr;
    }
}

// 0x005383F4 (name after sead)
bool pead::MessageQueue::push(Element message, BlockType blockType)
{
    if (blockType != BLOCK_TYPE_BLOCK) {
        return mQueue.TryEnqueue(message);
    }
    mQueue.Enqueue(message);
    return true;
}

// 0x0053841C (name after sead)
void pead::MessageQueue::allocate(s32 size, Heap* heap)
{
    if (size <= 0) {
        return;
    }
    mBuffer = NewArray<uptr>(size, heap);
    mQueue.Initialize(mBuffer, size);
}

// 0x00538484 (name after sead)
pead::MessageQueue::MessageQueue() : mBuffer(nullptr)
{
}

// 0x005384B8 (name after sead)
pead::MessageQueue::~MessageQueue()
{
    free();
}

// 0x001428C4 | nintendogs:bytes
pead::MessageQueue::Element pead::MessageQueue::pop(BlockType blockType)
{
    if (blockType != BLOCK_TYPE_BLOCK) {
        uptr message;
        if (mQueue.TryDequeue(&message)) {
            return message;
        }
        return 0;
    }
    return mQueue.Dequeue();
}
} // namespace pead
