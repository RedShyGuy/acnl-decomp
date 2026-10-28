#pragma once

#include "decomp.h"
#include "nn/os/detail/os_BlockingQueueBase.h"

namespace sead {
class Heap;

// A queue of messages (words) in a buffer from a heap: the messages of a Thread. allocate/free/pop
// are the names of the reference symbols; the other names are ours.
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

    // a queue that finalizes itself before the base destructor does it again (as in nw::snd)
    class Queue : public nn::os::detail::BlockingQueueBase<nn::os::CriticalSection>
    {
    public:
        ~Queue() { Finalize(); }
    };

    MessageQueue(); // 0x00138C2C (name is ours)
    ~MessageQueue(); // 0x00540FE4 | nintendogs:bytes [tier A]

    // a buffer of size messages from heap
    void allocate(s32 size, Heap* heap); // 0x00138BF0 | nintendogs:bytes [tier A]
    void free(); // 0x00540F60 | nintendogs:bytes [tier A]
    // adds the message at the back / the front; false if there is no room and blockType is NO_BLOCK
    bool push(Element message, BlockType blockType); // 0x00540FBC (name is ours)
    bool jam(Element message, BlockType blockType); // 0x00540F08 (name is ours)
    // the first message; 0 if there is none and blockType is NO_BLOCK
    Element pop(BlockType blockType); // 0x00540F30 (name after pead::MessageQueue::pop)

    uptr* mBuffer; // 0x00
    Queue mQueue;  // 0x04
};
ASSERT_SIZE(MessageQueue, 0x38);
} // namespace sead
