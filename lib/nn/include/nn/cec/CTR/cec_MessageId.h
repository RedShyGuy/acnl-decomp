#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
// The id of a message: 8 bytes, all zero for a message without an id (the member name is ours).
class MessageId
{
public:
    static const size_t SIZE = 8;

    // a copy of the 8 bytes at p (all zero for NULL)
    MessageId(const u8* p); // 0x00350A84 | nintendogs:callgraph [tier A]
    MessageId(u8* p); // 0x00350AAC | nintendogs:callgraph [tier A]
    MessageId(); // 0x00350AD4 | nintendogs:callgraph [tier A]
    bool IsEmpty() const; // 0x007295B0 | nintendogs:bytes [tier A]
    bool IsEqual(const u8* p) const; // 0x007295EC | nintendogs:callgraph [tier A]
    void GetBinary(u8* p) const; // 0x00729618 | nintendogs:callgraph [tier A]

private:
    u8 m_Id[SIZE]; // 0x0
};
ASSERT_SIZE(MessageId, 8);
} // namespace CTR
} // namespace cec
} // namespace nn
