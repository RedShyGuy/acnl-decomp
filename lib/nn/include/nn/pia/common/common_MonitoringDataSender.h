#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common20MonitoringDataSenderE @ 0x008CFEAC
// vtable 0x009015C4 (vptr 0x009015CC), offset_to_top 0, 6 entries
//
// Base of the senders of the monitoring data (inet::NexMonitoringDataSender). The meaning of the
// flag and of the slots 0x08 / 0x0C is not known; the member name is ours.
class MonitoringDataSender : public ::nn::pia::common::RootObject
{
public:
    MonitoringDataSender(); // 0x004283BC
    virtual ~MonitoringDataSender(); // 0x004283D8 slot 0x00
    // 0x004283D4 slot 0x04 (deleting dtor)
    virtual void vf_0x08(); // 0x004283B0 slot 0x08, clears the flag
    virtual bool vf_0x0C() const; // 0x00731BC4 slot 0x0C, the flag
    // sends the data (phase as in session::Mesh::MonitoringProcess: 0 begin, 1 end, 2 update;
    // name is ours)
    virtual void Send(u8 phase) = 0; // slot 0x10
    virtual void Trace(u64 flag) const; // 0x00731BCC slot 0x14 (name after StepSequenceJob::Trace)

    bool m_Flag; // 0x04
};
ASSERT_SIZE(MonitoringDataSender, 0x8);
} // namespace common
} // namespace pia
} // namespace nn
