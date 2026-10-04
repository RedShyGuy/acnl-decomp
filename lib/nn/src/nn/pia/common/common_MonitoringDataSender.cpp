#include "nn/pia/common/common_MonitoringDataSender.h"

namespace nn {
namespace pia {
namespace common {
// 0x004283B0
void nn::pia::common::MonitoringDataSender::vf_0x08()
{
    m_Flag = false;
}

// 0x004283BC
nn::pia::common::MonitoringDataSender::MonitoringDataSender() : m_Flag(false)
{
}

// 0x004283D8
// 0x004283D4 (deleting dtor)
nn::pia::common::MonitoringDataSender::~MonitoringDataSender()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00731BC4
bool nn::pia::common::MonitoringDataSender::vf_0x0C() const
{
    return m_Flag;
}

// 0x00731BCC (name after StepSequenceJob::Trace)
void nn::pia::common::MonitoringDataSender::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
