#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_PlayingSession.h"

namespace nn {
namespace nex {
// (0x003C81B0, named PlayingSession::PlayingSession() [ctor?] in symbols.json, is a larger function
// that reads a list of playing sessions)
// RTTI N2nn3nex14PlayingSessionE @ 0x008CE3B8
// vtable 0x008FCC58 (vptr 0x008FCC60), offset_to_top 0, 2 entries
class PlayingSession : public ::nn::nex::_DDL_PlayingSession
{
public:
    // (inline)
    PlayingSession() {}
    virtual ~PlayingSession(); // 0x00373B30 slot 0x00
    // 0x00373AF8 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(PlayingSession, 0x14);
} // namespace nex
} // namespace nn
