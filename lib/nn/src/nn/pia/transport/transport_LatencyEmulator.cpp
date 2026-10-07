#include "nn/pia/transport/transport_LatencyEmulator.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00450174 | fefates:callseq [tier C]
nn::Result nn::pia::transport::LatencyEmulator::readDispatch()
{
    common::Time now;
    now.SetNow();
    nn::Result result;
    while (!m_Ring.IsFull()) {
        result = m_pInput->Read(&m_ReadEntry.m_Packet);
        if (result.IsFailure()) {
            if (result == common::RESULT_NO_DATA) {
                return nn::Result();
            }
            return result;
        }
        m_ReadEntry.m_Time = GetPassTime(now);
        m_Ring.PushBack(m_ReadEntry);
    }
    return result;
}

// 0x004502AC | fefates:bytes [tier B]
nn::Result nn::pia::transport::LatencyEmulator::writeDispatch()
{
    common::Time now;
    now.SetNow();
    nn::Result result;
    while (m_Ring.GetNum() != 0) {
        if (m_Ring.Front().m_Time >= now) {
            return result;
        }
        result = m_pOutput->Write(m_Ring.Front().m_Packet);
        m_Ring.PopFront();
        if (result.IsFailure()) {
            if (result == common::RESULT_BUFFER_IS_FULL) {
                return nn::Result();
            }
            return result;
        }
    }
    return result;
}

// 0x004503C8 (name is ours)
nn::Result nn::pia::transport::LatencyEmulator::Read(nn::pia::common::Packet* pPacket)
{
    if (!common::IsValidPointer(pPacket)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!common::IsValidPointer(m_pInput)) {
        return common::RESULT_INVALID_STATE;
    }
    if (m_Ring.GetNum() == 0) {
        return common::RESULT_NO_DATA;
    }
    common::Time now;
    now.SetNow();
    if (m_Ring.Front().m_Time >= now) {
        return common::RESULT_NO_DATA;
    }
    *pPacket = m_Ring.Front().m_Packet;
    m_Ring.PopFront();
    return nn::Result();
}

// 0x004504FC | fefates:bytes [tier B]
void nn::pia::transport::LatencyEmulator::init(unsigned int packetNum)
{
    m_pEntries = nullptr;
    m_LatencyMin = 0;
    m_LatencyMax = 0;
    if (packetNum == 0) {
        return;
    }
    m_pEntries = common::NewArray<Entry>(packetNum);
    if (common::IsValidPointer(m_pEntries)) {
        m_Ring.SetBuffer(packetNum, m_pEntries);
    }
}

// 0x004505C0 | fefates:callseq [tier C]
void nn::pia::transport::LatencyEmulator::Clear()
{
    m_Ring.Clear();
}

// 0x004505D0 | fefates:callseq [tier C]
nn::Result nn::pia::transport::LatencyEmulator::Write(const nn::pia::common::Packet& packet)
{
    common::Time now;
    now.SetNow();
    common::Time time = GetPassTime(now);
    Entry entry;
    entry.m_Time = time;
    entry.m_Packet = packet;
    if (!m_Ring.PushBack(entry)) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    return nn::Result();
}

// 0x00450708 | fefates:bytes [tier B]
nn::Result nn::pia::transport::LatencyEmulator::Dispatch()
{
    if (!common::IsValidPointer(m_pInput) && common::IsValidPointer(m_pOutput)) {
        return writeDispatch();
    }
    if (common::IsValidPointer(m_pInput) && !common::IsValidPointer(m_pOutput)) {
        return readDispatch();
    }
    return common::RESULT_INVALID_STATE;
}

// 0x0045075C | fefates:bytes [tier B]
nn::pia::transport::LatencyEmulator::LatencyEmulator(unsigned int packetNum)
    : m_pInput(nullptr), m_pOutput(nullptr), m_CriticalSection(-1)
{
    init(packetNum);
}

// 0x004507C8 | fefates:bytes [tier B]
nn::pia::transport::LatencyEmulator::~LatencyEmulator()
{
    if (m_pEntries != nullptr) {
        common::DeleteArray(m_pEntries);
        m_pEntries = nullptr;
    }
}

} // namespace transport
} // namespace pia
} // namespace nn
