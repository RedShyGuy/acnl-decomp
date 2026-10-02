#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
class MessageId
{
public:
    MessageId(const unsigned char*); // 0x00350A84 | nintendogs:callgraph [tier A]
    MessageId(unsigned char*); // 0x00350AAC | nintendogs:callgraph [tier A]
    MessageId(); // 0x00350AD4 | nintendogs:callgraph [tier A]
    void IsEmpty() const; // 0x007295B0 | nintendogs:bytes [tier A]
    void IsEqual(const unsigned char*) const; // 0x007295EC | nintendogs:callgraph [tier A]
    void GetBinary(unsigned char*) const; // 0x00729618 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace cec
} // namespace nn
