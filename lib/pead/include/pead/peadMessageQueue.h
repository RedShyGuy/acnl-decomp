#pragma once

#include "decomp.h"
#include "nn/os/detail/os_BlockingQueueBase.h"

namespace pead {
class Heap;

// pia's copy of sead::MessageQueue: a queue of messages (words) in a buffer from a heap, the messages of a
// Thread. pop is the name of the reference symbols; the other names are those of our sead.
class MessageQueue
{
public:
    typedef s32 Element;

    // whether push/jam/pop wait for room or for a message (names are ours)
    enum BlockType
    {
        BLOCK_TYPE_BLOCK = 0,
        BLOCK_TYPE_NO_BLOCK = 1,
    };

    // a queue that finalizes itself before the base destructor does it again (as in sead)
    class Queue : public nn::os::detail::BlockingQueueBase<nn::os::CriticalSection>
    {
    public:
        ~Queue() { Finalize(); }
    };

    MessageQueue(); // 0x00538484 (name after sead)
    ~MessageQueue(); // 0x005384B8 (name after sead)

    // a buffer of size messages from heap
    void allocate(s32 size, Heap* heap); // 0x0053841C (name after sead)
    void free(); // 0x0053838C (name after sead)
    // adds the message at the back / the front; false if there is no room and blockType is NO_BLOCK
    bool push(Element message, BlockType blockType); // 0x005383F4 (name after sead)
    bool jam(Element message, BlockType blockType); // 0x00538364 (name after sead)
    // the first message; 0 if there is none and blockType is NO_BLOCK
    Element pop(BlockType blockType); // 0x001428C4 | nintendogs:bytes

    uptr* mBuffer; // 0x00
    Queue mQueue;  // 0x04
};
ASSERT_SIZE(MessageQueue, 0x38);
} // namespace pead
