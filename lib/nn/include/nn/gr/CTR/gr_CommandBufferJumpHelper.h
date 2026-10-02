#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class CommandBufferJumpHelper
{
public:
    CommandBufferJumpHelper(); // TODO: default ctor added so derived stubs compile - may not exist
    void FinalizeJump(unsigned int*); // 0x00349D64 | fefates:bytes [tier B]
    CommandBufferJumpHelper(unsigned int*); // 0x00349DA0 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace gr
} // namespace nn
