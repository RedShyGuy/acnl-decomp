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

    // (names are ours; only what the stage code uses)
    static NetGameMgr* s_pInstance;
    // a state that the network game resets after a scene change
    struct Wait
    {
        void Reset(bool isForced); // 0x0062C740
    };
    DECOMP_EMIT_COPY static s8 Get54() { return s_pInstance->m_54; } // 0x00625568

    u8 _unknown0[0x54];
    s8 m_54;                // 0x0054 (meaning unknown)
    u8 _unknown55[0x16B];
    bool m_IsSceneChanging; // 0x01C0 set by a scene change in a multiplayer game
    u8 _unknown1C1[0x272B];
    Wait m_Wait;            // 0x28EC
};
} // namespace netgame
