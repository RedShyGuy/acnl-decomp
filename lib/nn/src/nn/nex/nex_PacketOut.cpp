#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_StreamSettings.h"

namespace nn {
namespace nex {
// 0x003D7580 | fefates:bytes [tier B]
void nn::nex::PacketOut::Initialize(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value, nn::nex::Timeout* pTimeout)
{
    m_TypeFlags |= (type & TYPE_MASK) | (flags & ~TYPE_MASK);
    m_Unknown0x120 = 0;
    m_Unknown0x122 = 0;
    m_Unknown0x128 = 0;
    m_pEndPoint = pEndPoint;
    m_Unknown0x12C = 0;
    m_Unknown0x140 = 0;
    m_Unknown0x148 = value;
    if (pPayload != nullptr) {
        pPayload->AcquireRef();
        if (m_pPayload != nullptr) {
            m_pPayload->ReleaseRef();
        }
        m_pPayload = pPayload;
    }
    m_pTimeout = pTimeout;
    m_Unknown0x130 = 0;
    m_Unknown0x134 = 0;
    m_Unknown0x131 = 0;
    m_Unknown0x138 = 0;
}

// 0x003D7670 (name is ours)
bool nn::nex::PacketOut::IsBundlingTimeReached()
{
    if (GetType() == TYPE_RAW) {
        return true;
    }
    Stream::Type type = static_cast<Stream::Type>(GetStreamType());
    if (m_Unknown0x130 != 0 || Stream::GetSettings(type)->m_Bundling.m_IsEnabled == 0) {
        return true;
    }
    u64 queueTime = m_Unknown0x108.m_Value;
    StreamBundling& bundling = Stream::GetSettings(type)->m_Bundling;
    Core* pCore = Core::GetInstance();
    ScopedCS lock((pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock);
    {
        // (the update of the time locks again)
        pCore = Core::GetInstance();
        ScopedCS innerLock((pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock);
        Time now = SystemClock::GetTime();
        if (static_cast<s64>(now.m_Value - bundling.m_LastTime) > bundling.m_Interval) {
            bundling.m_LastTime = now.m_Value;
        }
    }
    return bundling.m_LastTime >= queueTime;
}

// 0x003D7780 | fefates:bytes [tier B]
nn::nex::PacketOut::PacketOut(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value)
    : m_Unknown0x128(0), m_Unknown0x12C(0)
{
    Timeout* pTimeout = nullptr;
    if ((flags & FLAG_NEED_ACK) != 0) {
        pTimeout = new Timeout();
    }
    Initialize(pEndPoint, type, flags, pPayload, value, pTimeout);
}

// 0x003D7884 slot 0x00 | fefates:bytes
// 0x003D7850 (deleting dtor)
nn::nex::PacketOut::~PacketOut()
{
    if (m_pTimeout != nullptr) {
        delete m_pTimeout;
        m_pTimeout = nullptr;
    }
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00834AC4
template <>
DECOMP_NOINLINE void vector<nn::nex::PacketOut*, nn::nex::MemAllocator<nn::nex::PacketOut*> >::_C_insert_aux(nn::nex::PacketOut** pos, nn::nex::PacketOut* const& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
