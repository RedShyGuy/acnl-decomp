#include "nn/gr/CTR/gr_Combiner.h"
#include "nn/gr/CTR/detail/gr_Command.h"

namespace nn {
namespace gr {
namespace CTR {
// 0x0034B548 | fefates:bytes-fuzzy [tier B]
nn::gr::CTR::Combiner::Combiner() : m_BufferColorR(0), m_BufferColorG(0), m_BufferColorB(0), m_BufferColorA(0)
{
    for (int i = 0; i < STAGE_COUNT; ++i) {
        m_Stages[i] = Stage(i);
    }
}

// 0x00728A80 | fefates:bytes [tier B]
u32* nn::gr::CTR::Combiner::Stage::MakeCommand(u32* command) const
{
    *command++ = m_Rgb.m_Source[0] | (m_Rgb.m_Source[1] << 4) | (m_Rgb.m_Source[2] << 8) | (m_Alpha.m_Source[0] << 16)
        | (m_Alpha.m_Source[1] << 20) | (m_Alpha.m_Source[2] << 24);
    *command++ = detail::CommandHeader(m_Register, 0xF, 4, true);
    *command++ = m_Rgb.m_Operand[0] | (m_Rgb.m_Operand[1] << 4) | (m_Rgb.m_Operand[2] << 8) | (m_Alpha.m_Operand[0] << 12)
        | (m_Alpha.m_Operand[1] << 16) | (m_Alpha.m_Operand[2] << 20);
    *command++ = m_Rgb.m_Combine | (m_Alpha.m_Combine << 16);
    *command++ = detail::PackColor(m_ColorR, m_ColorG, m_ColorB, m_ColorA);
    *command++ = m_Rgb.m_Scale | (m_Alpha.m_Scale << 16);
    return command;
}

// 0x0072893C | fefates:bytes [tier B]
u32* nn::gr::CTR::Combiner::MakeCommand(u32* command) const
{
    for (int i = 0; i < STAGE_COUNT; ++i) {
        command = m_Stages[i].MakeCommand(command);
    }
    // which of stages 1 to 4 write their result into the combiner buffer
    *command++ = (m_Stages[1].m_Rgb.m_BufferInput << 8) | (m_Stages[1].m_Alpha.m_BufferInput << 12)
        | ((m_Stages[2].m_Rgb.m_BufferInput << 9) | (m_Stages[2].m_Alpha.m_BufferInput << 13))
        | ((m_Stages[3].m_Rgb.m_BufferInput << 10) | (m_Stages[3].m_Alpha.m_BufferInput << 14))
        | ((m_Stages[4].m_Rgb.m_BufferInput << 11) | (m_Stages[4].m_Alpha.m_BufferInput << 15));
    *command++ = detail::CommandHeader(0xE0, 0x2);
    *command++ = detail::PackColor(m_BufferColorR, m_BufferColorG, m_BufferColorB, m_BufferColorA);
    *command++ = detail::CommandHeader(0xFD);
    return command;
}

} // namespace CTR
} // namespace gr
} // namespace nn
