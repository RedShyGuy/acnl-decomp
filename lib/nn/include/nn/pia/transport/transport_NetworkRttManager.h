#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Time.h"
#include "pead/peadPtrArray.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
}
namespace transport {
// RTTI N2nn3pia9transport17NetworkRttManagerE @ 0x008D01E8
// vtable 0x00901E9C (vptr 0x00901EA4), offset_to_top 0, 1 entries
//
// The round trip times of the packets: every packet carries the milliseconds since Initialize
// (header +8) and echoes the last time stamp of its destination plus the time it was held here
// (header +10). The echo gives a sample of the round trip time of the source; the samples go into
// a ring of 16 slots of 256 ms per station (MeasurementData). Layout from CreateInstance and
// InitializeImpl; the member names and the names marked so are ours. The destructor is not virtual
// (the vtable only has Trace).
class NetworkRttManager : public ::nn::pia::common::RootObject
{
public:
    class MeasurementData;

    // the last time stamp received from a station (name is ours)
    struct TimeStampData
    {
        TimeStampData() : m_ReceiveTime(0), m_TimeStamp(0) {}

        void Update(u32 now) { m_ReceiveTime = now; }
        void Initialize(const common::StationAddress& address, u32 now)
        {
            m_StationAddress = address;
            m_ReceiveTime = now;
            m_TimeStamp = 0;
        }

        common::StationAddress m_StationAddress; // 0x00
        u32 m_ReceiveTime;                       // 0x10, ms since Initialize
        u16 m_TimeStamp;                         // 0x14, ms since the Initialize of the station
    };

    // the entries of the stations in a pead::PtrArray sorted by the station address, their memory
    // in one heap block; a new station replaces the one that has been there longest when it is
    // full (name is ours)
    template <typename T>
    class Table
    {
    public:
        Table() : m_pBuffer(nullptr) {}

        // binarySearch: an entry and the station address of the key
        static int Compare(const void* a, const void* b)
        {
            return common::StationAddress::Compare(static_cast<const T*>(a)->m_StationAddress,
                                                   *static_cast<const common::StationAddress*>(b));
        }

        bool IsInitialized() const { return m_Array.mPtrs != nullptr; }

        void Initialize(u32 num, pead::Heap* pHeap)
        {
            if (num == 0) {
                m_pBuffer = nullptr;
                return;
            }
            m_pBuffer = common::NewArray<T>(num);
            m_Array.allocBuffer(num, pHeap, 4);
        }
        void Finalize()
        {
            if (m_pBuffer != nullptr) {
                m_Array.freeBuffer();
                if (m_pBuffer != nullptr) {
                    common::DeleteArray(m_pBuffer);
                }
                m_pBuffer = nullptr;
            }
        }

        T* Find(const common::StationAddress& address) const
        {
            if (IsInitialized()) {
                int index = m_Array.binarySearch(&address, Compare);
                if (index >= 0) {
                    return m_Array.at(index);
                }
            }
            return nullptr;
        }

        // the entry of the station (a new one if there is none)
        DECOMP_ALWAYS_INLINE T* Update(const common::StationAddress& address, u32 now)
        {
            int index = m_Array.binarySearch(&address, Compare);
            T* p;
            if (index >= 0) {
                p = m_Array.at(index);
                p->Update(now);
                return p;
            }
            if (m_Array.size() >= m_Array.mPtrNumMax) {
                s32 oldest = 0;
                u32 oldestTime = now - static_cast<T*>(m_Array.mPtrs[0])->m_ReceiveTime;
                for (s32 i = 1; i < m_Array.size(); i++) {
                    u32 time = now - m_Array.at(i)->m_ReceiveTime;
                    if (time > oldestTime) {
                        oldest = i;
                        oldestTime = time;
                    }
                }
                p = m_Array.at(oldest);
                m_Array.erase(oldest, 1);
            } else {
                p = &m_pBuffer[m_Array.size()];
            }
            p->Initialize(address, now);
            for (s32 i = 0; i < m_Array.size(); i++) {
                if (address < m_Array.at(i)->m_StationAddress) {
                    m_Array.insert(i, p);
                    return p;
                }
            }
            m_Array.pushBack(p);
            return p;
        }

        pead::PtrArray<T> m_Array; // 0x0
        T* m_pBuffer;              // 0xC
    };

    // (inline in CreateInstance)
    NetworkRttManager() : m_TimeStampCriticalSection(-1), m_MeasurementCriticalSection(-1) {}
    // the destructor; symbols.json has nn::nex::DOClassesTable::~DOClassesTable here
    DECOMP_NOINLINE ~NetworkRttManager(); // 0x00454D3C
    virtual void Trace(u64 flag) const; // 0x007356D8 slot 0x00

    static nn::Result CreateInstance(); // 0x00454490 | fefates:bytes [tier B]
    // symbols.json has nn::nex::DOClassesTable::TearDownDOClassesTable here
    static void DestroyInstance(); // 0x00454524
    nn::Result InitializeImpl(unsigned int measurementNum, unsigned int timeStampNum); // 0x00454550 | fefates:bytes [tier B]
    void Finalize(); // 0x00454C20 | fefates:bytes [tier B]

    // the average round trip time to the station in ms (0 if it is not known)
    u32 GetAverage(const nn::pia::common::StationAddress& address); // 0x0045421C | fefates:bytes [tier B]
    void WriteToPacket(nn::pia::common::Packet* pPacket); // 0x0045435C | fefates:bytes [tier B]
    void ReadFromPacket(const nn::pia::common::Packet* pPacket); // 0x004546D4 | fefates:bytes [tier B]

    // ms since Initialize
    u32 GetCurrentMSec() const
    {
        common::Time now;
        now.SetNow();
        return static_cast<u32>((now - m_StartTime).GetTick() / common::TimeSpan::GetTicksPerMSec().GetTick());
    }

    static NetworkRttManager* s_pInstance;

    Table<TimeStampData> m_TimeStampTable;               // 0x04
    common::CriticalSection m_TimeStampCriticalSection;   // 0x14
    Table<MeasurementData> m_MeasurementTable;           // 0x20
    common::CriticalSection m_MeasurementCriticalSection; // 0x30
    common::Time m_StartTime;                            // 0x40
};
ASSERT_SIZE(NetworkRttManager::TimeStampData, 0x18);
ASSERT_OFFSET(NetworkRttManager, m_MeasurementTable, 0x20);
ASSERT_OFFSET(NetworkRttManager, m_StartTime, 0x40);
ASSERT_SIZE(NetworkRttManager, 0x48);
} // namespace transport
} // namespace pia
} // namespace nn
