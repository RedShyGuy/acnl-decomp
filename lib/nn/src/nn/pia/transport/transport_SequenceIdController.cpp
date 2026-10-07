#include "nn/pia/transport/transport_SequenceIdController.h"

namespace nn {
namespace pia {
namespace transport {
// 0x004588EC | fefates:bytes [tier B]
u16 nn::pia::transport::SequenceIdController::GetNextSendSequenceId()
{
    m_SendSequenceId++;
    if (m_SendSequenceId == 0) {
        m_SendSequenceId = 1;
    }
    return m_SendSequenceId;
}

// 0x00458910 | fefates:bytes [tier B]
bool nn::pia::transport::SequenceIdController::CheckReceivedSequenceId(unsigned short sequenceId)
{
    if (sequenceId == 0) {
        if (m_IsCounting) {
            if (m_NoSequenceIdNum < 0xFFFF) {
                m_NoSequenceIdNum++;
            } else {
                m_NoSequenceIdNum = 0xFFFF;
                m_IsCounting = false;
            }
        }
        return true;
    }
    s16 diff = static_cast<s16>(sequenceId - m_ReceivedSequenceId);
    if (diff <= 0) {
        return false;
    }
    // the ids wrapped around: 0 lies between them, but it is never sent
    u16 untilWrap = static_cast<u16>(-m_ReceivedSequenceId);
    if (untilWrap != 0 && diff > untilWrap) {
        diff--;
    }
    if (m_IsCounting) {
        if (m_ReceivedNum + diff < 0xFFFF) {
            m_ReceivedNum += diff;
            m_LostNum += diff - 1;
        } else {
            m_IsCounting = false;
            m_LostNum += 0xFFFF - m_ReceivedNum;
            m_ReceivedNum = 0xFFFF;
        }
    }
    u32 totalReceivedNum = m_TotalReceivedNum + diff;
    if (totalReceivedNum >= m_TotalReceivedNum) {
        m_TotalLostNum += diff - 1;
        m_TotalReceivedNum = totalReceivedNum;
    } else {
        m_TotalLostNum += 0xFFFFFFFF - m_TotalReceivedNum;
        m_TotalReceivedNum = 0xFFFFFFFF;
    }
    m_ReceivedSequenceId = sequenceId;
    return true;
}

// 0x00458A1C | fefates:bytes [tier B]
nn::Result nn::pia::transport::SequenceIdController::Startup()
{
    m_SendSequenceId = 0;
    m_ReceivedSequenceId = 0;
    m_ReceivedNum = 0;
    m_LostNum = 0;
    m_NoSequenceIdNum = 0;
    m_IsCounting = false;
    m_TotalReceivedNum = 0;
    m_TotalLostNum = 0;
    return nn::Result();
}

// 0x00458A48 | fefates:bytes [tier B]
nn::pia::transport::SequenceIdController::SequenceIdController()
{
    // only the vptr (the members are set by Startup)
}

// 0x00458A5C
// 0x00458A58 (deleting dtor)
nn::pia::transport::SequenceIdController::~SequenceIdController()
{
    // empty (in the original too)
}

// 0x007360AC
void nn::pia::transport::SequenceIdController::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
