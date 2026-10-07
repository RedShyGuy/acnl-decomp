#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/transport/transport_NetworkRttManager.h"

// The round trip times measured to one station: a ring of 16 slots, each for 256 ms (the time in
// ms >> 8). The class name and UpdateTime are from the fefates symbols; the rest is ours.
class nn::pia::transport::NetworkRttManager::MeasurementData
{
public:
    static const u32 SLOT_NUM = 16;

    // the samples of one slot (name is ours)
    struct Slot
    {
        Slot(); // 0x00454D24

        void Clear()
        {
            m_Last = 0;
            m_Sum = 0;
            m_SquareSum = 0;
            m_Count = 0;
        }

        u32 m_Last;      // 0x0
        u32 m_Sum;       // 0x4
        u32 m_SquareSum; // 0x8
        u32 m_Count;     // 0xC
    };

    MeasurementData() : m_ReceiveTime(0) { Clear(); }

    void Clear()
    {
        for (u32 i = 0; i < SLOT_NUM; i++) {
            m_Slots[i].Clear();
        }
        m_LastTime = 0;
    }
    // (for NetworkRttManager::Table)
    void Initialize(const common::StationAddress& address, u32 now)
    {
        m_StationAddress = address;
        m_ReceiveTime = now;
        Clear();
    }
    void Update(u32 now) { UpdateTime(now); }

    // moves to the slot of the time (ms), the slots in between are cleared; the time >> 8
    DECOMP_NOINLINE u32 UpdateTime(unsigned int now); // 0x00454B2C | fefates:bytes [tier B]

    common::StationAddress m_StationAddress; // 0x000
    u32 m_ReceiveTime;                       // 0x010, when it was added (ms since Initialize)
    Slot m_Slots[SLOT_NUM];                  // 0x014
    u32 m_LastTime;                          // 0x114, the time of the last UpdateTime
};
ASSERT_SIZE(nn::pia::transport::NetworkRttManager::MeasurementData, 0x118);
