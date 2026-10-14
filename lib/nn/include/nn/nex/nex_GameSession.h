#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_GameSession.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11GameSessionE @ 0x008CE00C
// vtable 0x008FC280 (vptr 0x008FC288), offset_to_top 0, 9 entries
class GameSession : public ::nn::nex::_DDL_GameSession
{
public:
    // (inline)
    GameSession() {}
    virtual ~GameSession(); // 0x003576DC slot 0x00
    // 0x003576BC slot 0x04 (deleting dtor)
};
ASSERT_SIZE(GameSession, 0x34);
} // namespace nex
} // namespace nn
