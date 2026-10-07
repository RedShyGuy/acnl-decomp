#include "nn/pia/transport/transport_NetworkRttManager.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_NetworkRttManager_MeasurementData.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// an echo older than this (ms) is not sent back
const u32 ECHO_TIME_LIMIT = 10000;
} // namespace

// 0x0097E454
nn::pia::transport::NetworkRttManager* nn::pia::transport::NetworkRttManager::s_pInstance;

// 0x00454354 (name is ours)
template int nn::pia::transport::NetworkRttManager::Table<nn::pia::transport::NetworkRttManager::TimeStampData>::Compare(const void*, const void*);
// 0x00454358 (name is ours)
template int nn::pia::transport::NetworkRttManager::Table<nn::pia::transport::NetworkRttManager::MeasurementData>::Compare(const void*, const void*);

// 0x0045421C | fefates:bytes [tier B]
u32 nn::pia::transport::NetworkRttManager::GetAverage(const nn::pia::common::StationAddress& address)
{
    m_MeasurementCriticalSection.Lock();
    MeasurementData* pData = m_MeasurementTable.Find(address);
    if (pData == nullptr) {
        m_MeasurementCriticalSection.Unlock();
        return 0;
    }
    pData->UpdateTime(GetCurrentMSec());
    u32 sum = 0;
    u32 count = 0;
    for (u32 i = 0; i < MeasurementData::SLOT_NUM; i++) {
        sum += pData->m_Slots[i].m_Sum;
        count += pData->m_Slots[i].m_Count;
    }
    u32 average = 0;
    if (count != 0) {
        average = sum / count;
    }
    m_MeasurementCriticalSection.Unlock();
    return average;
}

// 0x0045435C | fefates:bytes [tier B]
void nn::pia::transport::NetworkRttManager::WriteToPacket(nn::pia::common::Packet* pPacket)
{
    u32 now = GetCurrentMSec();
    if (!m_MeasurementTable.IsInitialized()) {
        pPacket->m_RttTimeStamp = 0;
    } else {
        pPacket->m_RttTimeStamp = __builtin_bswap16(static_cast<u16>(now));
    }
    u16 echo = 0;
    if (pPacket->m_DestinationStationAddress.IsValid()) {
        m_TimeStampCriticalSection.Lock();
        TimeStampData* pData = m_TimeStampTable.Find(pPacket->m_DestinationStationAddress);
        if (pData != nullptr) {
            u32 heldTime = now - pData->m_ReceiveTime;
            if (heldTime < ECHO_TIME_LIMIT) {
                echo = static_cast<u16>(pData->m_TimeStamp + heldTime);
                // 0 means no echo
                if (echo == 0) {
                    echo = 0xFFFF;
                }
            }
        }
        m_TimeStampCriticalSection.Unlock();
    }
    pPacket->m_RttEcho = __builtin_bswap16(echo);
}

// 0x00454490 | fefates:bytes [tier B]
nn::Result nn::pia::transport::NetworkRttManager::CreateInstance()
{
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new NetworkRttManager;
    return nn::Result();
}

// 0x00454524
void nn::pia::transport::NetworkRttManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00454550 | fefates:bytes [tier B]
nn::Result nn::pia::transport::NetworkRttManager::InitializeImpl(unsigned int measurementNum, unsigned int timeStampNum)
{
    if (m_MeasurementTable.IsInitialized() || m_TimeStampTable.IsInitialized()) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    pead::Heap* pHeap = common::HeapManager::GetHeap();
    m_MeasurementTable.Initialize(measurementNum, pHeap);
    m_TimeStampTable.Initialize(timeStampNum, pHeap);
    m_StartTime.SetNow();
    return nn::Result();
}

// 0x004546D4 | fefates:bytes [tier B]
void nn::pia::transport::NetworkRttManager::ReadFromPacket(const nn::pia::common::Packet* pPacket)
{
    const common::StationAddress& address = pPacket->m_SourceStationAddress;
    if (!address.IsValid()) {
        return;
    }
    u32 now = GetCurrentMSec();
    if (__builtin_bswap16(pPacket->m_RttTimeStamp) != 0) {
        m_TimeStampCriticalSection.Lock();
        if (m_TimeStampTable.IsInitialized()) {
            TimeStampData* pData = m_TimeStampTable.Update(address, now);
            if (pData != nullptr) {
                pData->m_TimeStamp = __builtin_bswap16(pPacket->m_RttTimeStamp);
            }
        }
        m_TimeStampCriticalSection.Unlock();
    }
    u16 echo = __builtin_bswap16(pPacket->m_RttEcho);
    if (echo == 0) {
        return;
    }
    u16 rtt = static_cast<u16>(now - echo);
    m_MeasurementCriticalSection.Lock();
    if (m_MeasurementTable.IsInitialized()) {
        MeasurementData* pData = m_MeasurementTable.Update(address, now);
        if (pData != nullptr) {
            MeasurementData::Slot& slot = pData->m_Slots[pData->UpdateTime(now) % MeasurementData::SLOT_NUM];
            u32 squareSum = slot.m_SquareSum + rtt * rtt;
            // not when the sum of the squares overflows
            if (slot.m_SquareSum <= squareSum) {
                slot.m_Last = rtt;
                slot.m_Sum += rtt;
                slot.m_SquareSum = squareSum;
                slot.m_Count++;
            }
        }
    }
    m_MeasurementCriticalSection.Unlock();
}

// 0x00454B2C | fefates:bytes [tier B]
u32 nn::pia::transport::NetworkRttManager::MeasurementData::UpdateTime(unsigned int now)
{
    u32 time = now >> 8;
    u32 lastTime = m_LastTime >> 8;
    if (time != lastTime) {
        if (time - lastTime >= SLOT_NUM) {
            Clear();
        } else {
            u32 index = time % SLOT_NUM;
            u32 lastIndex = lastTime % SLOT_NUM;
            if (index < lastIndex) {
                for (u32 i = 0; i <= index; i++) {
                    m_Slots[i].Clear();
                }
                for (u32 i = lastIndex + 1; i < SLOT_NUM; i++) {
                    m_Slots[i].Clear();
                }
            } else {
                for (u32 i = lastIndex + 1; i <= index; i++) {
                    m_Slots[i].Clear();
                }
            }
        }
    }
    m_LastTime = now;
    return time;
}

// 0x00454C20 | fefates:bytes [tier B]
void nn::pia::transport::NetworkRttManager::Finalize()
{
    m_TimeStampTable.Finalize();
    m_MeasurementTable.Finalize();
}

// 0x00454D24 (name after C++)
nn::pia::transport::NetworkRttManager::MeasurementData::Slot::Slot() : m_Last(0), m_Sum(0), m_SquareSum(0), m_Count(0)
{
}

// 0x00454D3C (name after C++)
nn::pia::transport::NetworkRttManager::~NetworkRttManager()
{
    Finalize();
}

// 0x007356D8 slot 0x00
void nn::pia::transport::NetworkRttManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
