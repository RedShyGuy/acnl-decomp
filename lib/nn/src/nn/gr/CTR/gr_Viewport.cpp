#include "nn/gr/CTR/gr_Viewport.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
using detail::CommandHeader;

// 0x00728B2C | nintendogs:bytes [tier B]
u32* nn::gr::CTR::Viewport::MakeCommand(u32* command) const
{
    *command++ = Float32ToFloat24(static_cast<f32>(m_Width) * 0.5f);
    *command++ = CommandHeader(0x41, 0x7);
    *command++ = detail::Float32ToFloat31(2.0f / static_cast<f32>(m_Width)) << 1;
    *command++ = CommandHeader(0x42);
    *command++ = Float32ToFloat24(static_cast<f32>(m_Height) * 0.5f);
    *command++ = CommandHeader(0x43, 0x7);
    *command++ = detail::Float32ToFloat31(2.0f / static_cast<f32>(m_Height)) << 1;
    *command++ = CommandHeader(0x44);
    *command++ = m_X | (m_Y << 16);
    *command++ = CommandHeader(0x68);
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
