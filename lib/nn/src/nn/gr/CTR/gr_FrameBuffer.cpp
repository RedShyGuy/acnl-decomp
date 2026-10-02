#include "nn/gr/CTR/gr_FrameBuffer.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x00123B90 | fefates:bytes [tier B]
nn::gr::CTR::FrameBuffer::FrameBuffer()
{
}

// 0x00726A44 | nintendogs:callseq-callee [tier A]
void nn::gr::CTR::FrameBuffer::MakeCommand(unsigned*, unsigned, bool) const
{
}

// 0x00726BB0 | fefates:bytes [tier B]
void nn::gr::CTR::FrameBuffer::MakeClearRequest(unsigned int, bool) const
{
}

} // namespace CTR
} // namespace gr
} // namespace nn
