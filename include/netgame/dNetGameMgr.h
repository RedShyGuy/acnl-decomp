#pragma once

#include "decomp.h"

namespace netgame {
class NetGameMgr
{
public:
    class SingletonDisposer_;
    class Mgr8;
    class SaveDemoMgr;
    class Receiver;
    class Unknown1;
    void FiniNet(); // 0x00617FD4 | libgarden [tier A]
    void SendPacketBatch(netgame::PlayerNo, void const* const*, unsigned int const*, unsigned int, unsigned char, unsigned long, unsigned long, unsigned long); // 0x00618318 | libgarden [tier A]
    void ConnectRandomMatch(); // 0x0061937C | libgarden [tier A]
    void GetPlayerCount() const; // 0x0075EFF8 | libgarden [tier A]
    void GetMyPlayerNo() const; // 0x0075F004 | libgarden [tier A]
    void IsAdmitted(netgame::PlayerNo) const; // 0x0075F02C | libgarden [tier A]
    void IsMultiPlayer() const; // 0x0075F064 | libgarden [tier A]
    void IsInSession(netgame::PlayerNo, bool) const; // 0x00760658 | libgarden [tier A]
};
} // namespace netgame
