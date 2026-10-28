#include "sead/seadMessageQueue.h"
#include "sead/seadNew.h"

namespace sead {
// 0x00138C2C (name is ours)
sead::MessageQueue::MessageQueue() : mBuffer(nullptr)
{
}

// 0x00540FE4 | nintendogs:bytes [tier A]
sead::MessageQueue::~MessageQueue()
{
    free();
}

// 0x00138BF0 | nintendogs:bytes [tier A]
void sead::MessageQueue::allocate(s32 size, Heap* heap)
{
    if (size <= 0) {
        return;
    }
    mBuffer = new (heap, 4) uptr[size];
    mQueue.Initialize(mBuffer, size);
}

// 0x00540F60 | nintendogs:bytes [tier A]
void sead::MessageQueue::free()
{
    mQueue.Finalize();
    if (mBuffer != nullptr) {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
}

// 0x00540FBC (name is ours)
bool sead::MessageQueue::push(Element message, BlockType blockType)
{
    if (blockType != BLOCK_TYPE_BLOCK) {
        return mQueue.TryEnqueue(message);
    }
    mQueue.Enqueue(message);
    return true;
}

// 0x00540F08 (name is ours)
bool sead::MessageQueue::jam(Element message, BlockType blockType)
{
    if (blockType != BLOCK_TYPE_BLOCK) {
        return mQueue.TryJam(message);
    }
    mQueue.Jam(message);
    return true;
}

// 0x00540F30 (name after pead::MessageQueue::pop)
sead::MessageQueue::Element sead::MessageQueue::pop(BlockType blockType)
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

} // namespace sead
