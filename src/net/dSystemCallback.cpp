#include "net/dSystemCallback.h"

namespace net {
// 0x0050CFF4 (name is ours)
void net::SystemCallback::OnStationLeft(const nn::pia::StationId& stationId, u32 frame)
{
    // empty (in the original too)
}

// 0x0050CFF8 (name is ours)
bool net::SystemCallback::OnJoinApproval(const char16* name, u32 length)
{
    return true;
}

// 0x0050D000 (name is ours)
void net::SystemCallback::OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification)
{
    // empty (in the original too)
}

// 0x0050D004 (name is ours)
void net::SystemCallback::OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                         bool isFromHandler)
{
    // empty (in the original too)
}

// 0x0050D008 (name is ours)
void net::SystemCallback::OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame)
{
    // empty (in the original too)
}

// 0x0050D00C (name is ours)
void net::SystemCallback::OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size)
{
    // empty (in the original too)
}

// 0x0050D010 (name after the RTTI)
net::SystemCallback::SystemCallback()
{
    // (only the vptr is set)
}

// 0x0050D028 | libgarden
// 0x0050D020 (deleting dtor)
net::SystemCallback::~SystemCallback()
{
    // empty (in the original too)
}
} // namespace net
