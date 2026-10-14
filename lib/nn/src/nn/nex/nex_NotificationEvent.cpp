#include "nn/nex/nex_NotificationEvent.h"

namespace nn {
namespace nex {
// 0x003873E0 slot 0x00
// 0x003873C0 (deleting dtor)
nn::nex::NotificationEvent::~NotificationEvent()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D9EE0
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::NotificationEvent& event)
{
    u64 param = (static_cast<u64>(event.m_uiParam1) << 32) | event.m_uiParam2;
    stream << L"NotificationEvent (" << L" PIDSource=" << event.m_pidSource << L" Type=" << event.m_uiType / 1000
           << L" SubType=" << event.m_uiType % 1000 << L" Param1=" << event.m_uiParam1 << L" Param2=" << event.m_uiParam2
           << L" Param3=" << event.m_uiParam3 << L" UInt64Param1=";
    stream.StreamNumber(param) << L" StrParam=" << event.m_strParam << L") ";
    return stream;
}

} // namespace nex
} // namespace nn
