#pragma once

#include "decomp.h"

namespace nn {
namespace gr {
namespace CTR {
class FrameBuffer
{
public:
    FrameBuffer(); // 0x00123B90 | fefates:bytes [tier B]
    void MakeCommand(unsigned*, unsigned, bool) const; // 0x00726A44 | nintendogs:callseq-callee [tier A]
    void MakeClearRequest(unsigned int, bool) const; // 0x00726BB0 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace gr
} // namespace nn
