#pragma once

#include "decomp.h"
#include "netgame/dNetGameMgr.h"

class netgame::NetGameMgr::Receiver
{
public:
    void SetWasHandled(unsigned char, bool); // 0x00625A8C | libgarden [tier A]
    void SetIsEnabled(unsigned char, bool); // 0x00625A9C | libgarden [tier A]
    void GetBuffer(unsigned char); // 0x0075FA00 | libgarden [tier A]
    void GetPacket(unsigned char); // 0x0075FA28 | libgarden [tier A]
};
