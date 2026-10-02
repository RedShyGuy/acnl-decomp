#pragma once

#include "decomp.h"

namespace sead {
class AudioFxMgr
{
public:
    void append(sead::AudioGlobal::AuxBus, sead::AudioFx*); // 0x0053E2F4 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace sead
