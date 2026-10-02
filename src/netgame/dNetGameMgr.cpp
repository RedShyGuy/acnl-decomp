#include "netgame/dNetGameMgr.h"

namespace netgame {
// 0x00617FD4 | libgarden [tier A]
void netgame::NetGameMgr::FiniNet()
{
}

// 0x00618318 | libgarden [tier A]
void netgame::NetGameMgr::SendPacketBatch(netgame::PlayerNo, void const* const*, unsigned int const*, unsigned int, unsigned char, unsigned long, unsigned long, unsigned long)
{
}

// 0x0061937C | libgarden [tier A]
void netgame::NetGameMgr::ConnectRandomMatch()
{
}

// 0x0075EFF8 | libgarden [tier A]
void netgame::NetGameMgr::GetPlayerCount() const
{
}

// 0x0075F004 | libgarden [tier A]
void netgame::NetGameMgr::GetMyPlayerNo() const
{
}

// 0x0075F02C | libgarden [tier A]
void netgame::NetGameMgr::IsAdmitted(netgame::PlayerNo) const
{
}

// 0x0075F064 | libgarden [tier A]
void netgame::NetGameMgr::IsMultiPlayer() const
{
}

// 0x00760658 | libgarden [tier A]
void netgame::NetGameMgr::IsInSession(netgame::PlayerNo, bool) const
{
}

} // namespace netgame
