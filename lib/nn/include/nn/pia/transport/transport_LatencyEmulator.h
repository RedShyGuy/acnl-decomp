#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "pead/peadRandom.h"

namespace nn {
namespace pia {
namespace common {
class IPacketInput;
class IPacketOutput;
} // namespace common
namespace transport {
// Delays the packets of a stream by a random time between m_LatencyMin and m_LatencyMax (ms):
// it reads from m_pInput (readDispatch) or writes to m_pOutput (writeDispatch) through a ring of
// packets with their time. The class and the functions are from the fefates symbols; the layout
// is from the constructor and init, the member names and the names marked so are ours. Made with
// new (RootObject) by TransportThreadStream::InitializeCore.
class LatencyEmulator : public ::nn::pia::common::RootObject
{
public:
    // a delayed packet (name is ours)
    struct Entry
    {
        Entry() {}

        common::Packet m_Packet; // 0x000
        common::Time m_Time;     // 0x5F0, when it is passed on
    };

    // a ring of T in a buffer from outside (name is ours)
    template <typename T>
    class RingBuffer
    {
    public:
        RingBuffer() : m_pBuffer(nullptr), m_Size(0), m_Head(0), m_Num(0) {}

        void SetBuffer(s32 size, T* pBuffer)
        {
            if (size > 0 && pBuffer != nullptr) {
                m_pBuffer = pBuffer;
                m_Num = 0;
                m_Size = size;
                m_Head = 0;
            }
        }
        void Clear()
        {
            m_Num = 0;
            m_Head = 0;
        }
        s32 GetNum() const { return m_Num; }
        bool IsFull() const { return m_Num >= m_Size; }

        T& Front()
        {
            if (m_Num > 0) {
                s32 index = m_Head;
                if (index >= m_Size) {
                    index -= m_Size;
                }
                return m_pBuffer[index];
            }
            return m_pBuffer[0];
        }
        bool PushBack(const T& value)
        {
            if (m_Num >= m_Size) {
                return false;
            }
            s32 index = m_Num++ + m_Head;
            if (index >= m_Size) {
                index -= m_Size;
            }
            m_pBuffer[index] = value;
            return true;
        }
        void PopFront()
        {
            if (m_Num > 0) {
                m_Num--;
                m_Head++;
                if (m_Head >= m_Size) {
                    m_Head = 0;
                }
            }
        }

        T* m_pBuffer; // 0x0
        s32 m_Size;   // 0x4
        s32 m_Head;   // 0x8
        s32 m_Num;    // 0xC
    };

    explicit LatencyEmulator(unsigned int packetNum); // 0x0045075C | fefates:bytes [tier B]
    ~LatencyEmulator(); // 0x004507C8 | fefates:bytes [tier B]

    void init(unsigned int packetNum); // 0x004504FC | fefates:bytes [tier B]
    void Clear(); // 0x004505C0 | fefates:callseq [tier C]

    // passes on the packets whose time has come (m_pInput or m_pOutput must be set, not both)
    nn::Result Dispatch(); // 0x00450708 | fefates:bytes [tier B]
    // the input: reads all packets of m_pInput into the ring
    nn::Result readDispatch(); // 0x00450174 | fefates:callseq [tier C]
    // the output: writes the packets whose time has come to m_pOutput
    nn::Result writeDispatch(); // 0x004502AC | fefates:bytes [tier B]
    // the input: the next packet whose time has come (name is ours)
    nn::Result Read(nn::pia::common::Packet* pPacket); // 0x004503C8
    // the output: puts the packet into the ring
    nn::Result Write(const nn::pia::common::Packet& packet); // 0x004505D0 | fefates:callseq [tier C]

    // when a packet that comes now is passed on
    common::Time GetPassTime(const common::Time& now)
    {
        m_CriticalSection.Lock();
        u32 latencyMin = m_LatencyMin;
        u32 latencyMax = m_LatencyMax;
        m_CriticalSection.Unlock();
        s32 latency = latencyMin + m_Random.getU32(latencyMax - latencyMin);
        return now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().GetTick() * latency);
    }

    RingBuffer<Entry> m_Ring;                  // 0x00
    Entry* m_pEntries;                         // 0x10
    common::IPacketInput* m_pInput;            // 0x14
    common::IPacketOutput* m_pOutput;          // 0x18
    u32 m_LatencyMin;                          // 0x1C
    u32 m_LatencyMax;                          // 0x20
    common::CriticalSection m_CriticalSection; // 0x24
    pead::Random m_Random;                     // 0x30
    Entry m_ReadEntry;                         // 0x40
};
ASSERT_SIZE(LatencyEmulator::Entry, 0x5F8);
ASSERT_OFFSET(LatencyEmulator, m_ReadEntry, 0x40);
ASSERT_SIZE(LatencyEmulator, 0x638);
} // namespace transport
} // namespace pia
} // namespace nn
