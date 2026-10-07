#include "nn/pia/transport/transport_RttCalculator.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097FA34
s32 nn::pia::transport::RttCalculator::s_TimeOutMSec[2] = {2000, 200};

// 0x0044EDB0 (name is ours)
void nn::pia::transport::RttCalculator::SetSendTime(const common::Time& time)
{
    m_SendTime = time;
}

// 0x0044EDC0 | fefates:bytes [tier B]
void nn::pia::transport::RttCalculator::Update(int rtt)
{
    m_LastRtt = rtt;
    m_Rtts.Add(rtt);
}

// 0x0044EDFC | fefates:bytes [tier B]
void nn::pia::transport::RttCalculator::Cleanup()
{
    if (m_IsActive) {
        m_IsActive = false;
        m_Rtts.Clear();
        m_LastRtt = -1;
        m_SendTime = common::Time();
    }
}

// 0x0044EE2C | fefates:bytes [tier B]
void nn::pia::transport::RttCalculator::Startup()
{
    m_IsActive = true;
    m_LastRtt = -1;
    m_SendTime = common::Time();
    m_Rtts.Clear();
}

// 0x0044EE54 | fefates:bytes [tier B]
nn::pia::transport::RttCalculator::RttCalculator() : m_IsActive(false), m_LastRtt(-1), m_SendTime()
{
    m_Rtts.Clear();
}

// 0x0044EE80 (name is ours)
nn::pia::transport::RttCalculator::~RttCalculator()
{
    // empty (in the original too)
}

// 0x00734D58 | fefates:bytes [tier B]
int nn::pia::transport::RttCalculator::GetRtt(unsigned int num) const
{
    if (!m_IsActive || m_Rtts.GetCount() == 0) {
        return -1;
    }
    return m_Rtts.GetMedian(num);
}

// 0x00734D80 | fefates:bytes [tier B]
int nn::pia::transport::RttCalculator::GetRtt() const
{
    if (!m_IsActive || m_Rtts.GetCount() == 0) {
        return -1;
    }
    return m_Rtts.GetMedian(16);
}

// 0x00734DAC | fefates:bytes [tier B]
bool nn::pia::transport::RttCalculator::IsTimeOut() const
{
    s32 timeout = m_Rtts.GetCount() == 16 ? s_TimeOutMSec[0] : s_TimeOutMSec[1];
    common::TimeSpan elapsed = Transport::s_pInstance->GetDispatchTime() - m_SendTime;
    return elapsed.GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick() >= timeout;
}

} // namespace transport
} // namespace pia
} // namespace nn
