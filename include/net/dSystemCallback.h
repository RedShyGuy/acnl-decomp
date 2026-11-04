#pragma once

#include "decomp.h"
#include "nn/pia/pia_Types.h"

namespace net {
// RTTI N3net14SystemCallbackE @ 0x008D0FC8
// vtable 0x0090465C (vptr 0x00904664), offset_to_top 0, 8 entries
//
// The events of the network layer for the game (the class name and the destructor after libgarden,
// the slot names are ours; frame is the frame counter of the network thread)
class SystemCallback
{
public:
    SystemCallback(); // 0x0050D010
    virtual ~SystemCallback(); // 0x0050D028 slot 0x00
    // whether a station with the player name may join the session that we created
    virtual bool OnJoinApproval(const char16* name, u32 length); // 0x0050CFF8 slot 0x08
    virtual void OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame); // 0x0050D008 slot 0x0C
    virtual void OnStationLeft(const nn::pia::StationId& stationId, u32 frame); // 0x0050CFF4 slot 0x10
    virtual void OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size); // 0x0050D00C slot 0x14
    virtual void OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification); // 0x0050D000 slot 0x18
    virtual void OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                bool isFromHandler); // 0x0050D004 slot 0x1C
};
} // namespace net
