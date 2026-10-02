#pragma once

#include "decomp.h"

namespace sead {
class MessageQueue
{
public:
    struct BlockType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void allocate(int, sead::Heap*); // 0x00138BF0 | nintendogs:bytes [tier A]
    void pop(sead::MessageQueue::BlockType); // 0x001428C4 | nintendogs:bytes [tier A]
    void free(); // 0x00540F60 | nintendogs:bytes [tier A]
    ~MessageQueue(); // 0x00540FE4 | nintendogs:bytes [tier A]
};
} // namespace sead
