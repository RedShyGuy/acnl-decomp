#pragma once

#include "decomp.h"
#include "nn/pia/common/common_MonitoringDataSender.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexMonitoringDataSenderE @ 0x008CF9C0
// vtable 0x009004D8 (vptr 0x009004E0), offset_to_top 0, 6 entries
class NexMonitoringDataSender : public ::nn::pia::common::MonitoringDataSender
{
public:
    NexMonitoringDataSender(); // ctor candidate(s) 0x00404E3C (unverified)
    virtual void vf_0x00(); // 0x00404FB8 slot 0x00 | virtual slot, introduced by nn::pia::common::MonitoringDataSender
    virtual void vf_0x04(); // 0x00404F24 slot 0x04 | virtual slot, introduced by nn::pia::common::MonitoringDataSender
    virtual void vf_0x10(); // 0x00404AC4 slot 0x10 | virtual slot, introduced by nn::pia::common::MonitoringDataSender
    virtual void vf_0x14(); // 0x0072F52C slot 0x14 | virtual slot, introduced by nn::pia::common::MonitoringDataSender
};
} // namespace inet
} // namespace pia
} // namespace nn
