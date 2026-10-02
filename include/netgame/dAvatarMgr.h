#pragma once

#include "decomp.h"
#include "net/dSystemCallback.h"

namespace netgame {
// RTTI N7netgame9AvatarMgrE @ 0x008D3F70
// vtable 0x0090BD4C (vptr 0x0090BD54), offset_to_top 0, 8 entries
class AvatarMgr : public ::net::SystemCallback
{
public:
    AvatarMgr(); // ctor candidate(s) 0x0062B728 (unverified)
    virtual void vf_0x00(); // 0x0062B7F8 slot 0x00 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x04(); // 0x0062B7A8 slot 0x04 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x08(); // 0x0062ABDC slot 0x08 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x0C(); // 0x0062B2BC slot 0x0C | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x10(); // 0x0062A3E8 slot 0x10 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x14(); // 0x0062B4F4 slot 0x14 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x18(); // 0x0062AC3C slot 0x18 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x1C(); // 0x0062AFF4 slot 0x1C | virtual slot, introduced by net::SystemCallback
    void GetPlayerNo(nn::pia::StationId const&) const; // 0x007603E8 | libgarden [tier A]
    void GetAvatarInfo(netgame::PlayerNo) const; // 0x007604F0 | libgarden [tier A]
};
} // namespace netgame
