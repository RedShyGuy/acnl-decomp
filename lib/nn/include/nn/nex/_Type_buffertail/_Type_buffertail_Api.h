#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Buffer;
class Message;

// The DDL type of a buffer at the end of a message: the rest of the content.
class _Type_buffertail
{
public:
    static void Add(nn::nex::Message* pMessage, const nn::nex::Buffer& buffer); // 0x003846F0 | fefates:bytes [tier B]
    static void Extract(nn::nex::Message* pMessage, nn::nex::Buffer* pBuffer); // 0x00384728 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
