#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Message;
class qBuffer;

// The DDL type of qBuffer: a u16 size and the bytes.
class _Type_qBuffer
{
public:
    static void Add(nn::nex::Message* pMessage, const nn::nex::qBuffer& buffer); // 0x0036FAE8 | fefates:bytes [tier B]
    // false if the message is too short for the bytes
    static bool Extract(nn::nex::Message* pMessage, nn::nex::qBuffer* pBuffer); // 0x0036FB38 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
