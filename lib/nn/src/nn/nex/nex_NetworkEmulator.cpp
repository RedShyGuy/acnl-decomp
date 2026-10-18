#include "nn/nex/nex_NetworkEmulator.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
namespace {
// (inline) a random number from 0 to max
inline f32 GetRandomRatio(f32 max)
{
    RandomNumberGenerator& generator = Platform::GetRandomNumberGenerator();
    if (max <= 0.0f) {
        return 0.0f;
    }
    return generator.GetRandomNumber(0x10000) * (1.0f / 0x10000) * max;
}
} // namespace

// 0x007D4000
// 0x007D3FD0 (deleting dtor)
template <typename T>
TimedQueue<T>::~TimedQueue()
{
    while (m_Entries.GetSize() != 0) {
        PopHead();
    }
}

// 0x007D3C98 | fefates:bytes [tier B]
template <typename T>
void TimedQueue<T>::Queue(const T& value, nn::nex::Time time)
{
    typename qList<Entry>::Node* pPos = m_Entries.m_pHead;
    while (pPos != m_Entries.GetFirst() && time < pPos->m_pPrev->m_Value.m_Time) {
        pPos = pPos->m_pPrev;
    }
    Entry entry(value);
    entry.m_Time = time;
    m_Entries.insert(pPos, entry);
}

// 0x007D3EA4 (name is ours)
template <typename T>
bool TimedQueue<T>::IsHeadReady(nn::nex::Time time)
{
    return m_Entries.GetSize() != 0 && !(time < m_Entries.front().m_Time);
}

// 0x007D3F68 | fefates:callgraph [tier B]
template <typename T>
T TimedQueue<T>::GetHead(nn::nex::Time time)
{
    return m_Entries.front().m_Value;
}

// 0x007D3EE0 (name is ours)
template <typename T>
void TimedQueue<T>::PopHead()
{
    m_Entries.erase(m_Entries.GetFirst());
}

// 0x007D4678
// 0x007D4648 (deleting dtor)
template <typename T>
VirtualModem<T>::~VirtualModem()
{
    // nothing to do
}

// 0x007D45C8 | fefates:callgraph
template <typename T>
void VirtualModem<T>::Queue(const T& value, nn::nex::Time time)
{
    if (m_NextTime < time) {
        m_NextTime = time;
    }
    TimedQueue<T>::Queue(value, m_NextTime);
    if (m_pDevice->m_Bandwidth != -1) {
        m_NextTime = Time(m_NextTime.m_Value + value.GetSize() * 8000 / m_pDevice->m_Bandwidth);
    }
}

// 0x007D70E0
// 0x007D70BC (deleting dtor)
template <typename T>
VirtualInternet<T>::~VirtualInternet()
{
    // nothing to do
}

// 0x007D6F1C | fefates:callgraph
template <typename T>
void VirtualInternet<T>::Queue(const T& value, nn::nex::Time time)
{
    if (GetRandomRatio(1.0f) < m_pDevice->m_LossRate) {
        return;
    }
    Time dueTime = time + m_pDevice->m_Latency;
    if (m_pDevice->m_Jitter != 0) {
        dueTime = dueTime + static_cast<s32>(Platform::GetRandomNumber(m_pDevice->m_Jitter));
    }
    TimedQueue<T>::Queue(value, dueTime);
}

template class TimedQueue<nn::nex::NetworkEmulator::NetworkPacket>;
template class VirtualModem<nn::nex::NetworkEmulator::NetworkPacket>;
template class VirtualInternet<nn::nex::NetworkEmulator::NetworkPacket>;

// 0x0037AD74 | mk7dlp:callseq [tier A]
nn::nex::NetworkEmulator::NetworkEmulator(nn::nex::EmulationDevice* pDevice) : m_Modem(pDevice), m_Internet(pDevice), m_pDevice(pDevice)
{
}

// 0x0037A6AC | fefates:bytes [tier B]
void nn::nex::NetworkEmulator::Queue(nn::nex::Buffer* pBuffer, nn::nex::InetAddress& address)
{
    m_Modem.Queue(NetworkPacket(pBuffer, address), SystemClock::GetTime());
}

// 0x0037A7C4 | fefates:bytes [tier B]
void nn::nex::NetworkEmulator::Dequeue(nn::nex::Buffer* pBuffer)
{
    m_Internet.Dequeue(SystemClock::GetTime());
}

// 0x0037A8AC | fefates:callgraph [tier C]
bool nn::nex::NetworkEmulator::GetHead(nn::nex::Buffer** ppBuffer, nn::nex::InetAddress* pAddress)
{
    // the packets that left the modem go into the internet
    while (m_Modem.IsHeadReady(SystemClock::GetTime())) {
        m_Internet.Queue(m_Modem.GetHead(SystemClock::GetTime()), SystemClock::GetTime());
        m_Modem.Dequeue(SystemClock::GetTime());
    }
    if (!m_Internet.IsHeadReady(SystemClock::GetTime())) {
        return false;
    }
    NetworkPacket packet = m_Internet.GetHead(SystemClock::GetTime());
    if (ppBuffer != nullptr) {
        *ppBuffer = packet.m_pBuffer;
    }
    if (pAddress != nullptr) {
        *pAddress = packet.m_Address;
    }
    return true;
}

} // namespace nex
} // namespace nn
