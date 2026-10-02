#pragma once

#include "decomp.h"

namespace nn {
namespace gxlow {
namespace CTR {
class CmdReqQueueTx
{
public:
    void Initialize(void*); // 0x00136E1C | fefates:bytes [tier B]
    void TryEnqueue(const nn::gxlow::CTR::detail::CmdReq*); // 0x0013B5C0 | nintendogs:callgraph [tier A]
};
} // namespace CTR
} // namespace gxlow
} // namespace nn
