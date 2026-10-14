#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class Message;
class qResult;

// The DDL type of qResult: its code.
class _Type_qresult
{
public:
    static void Extract(nn::nex::Message* pMessage, nn::nex::qResult* pResult); // 0x0036FBFC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
