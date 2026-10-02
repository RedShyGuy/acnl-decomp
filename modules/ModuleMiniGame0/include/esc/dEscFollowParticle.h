#pragma once

#include "decomp.h"
#include "esc/dEscFollowBase.h"

namespace esc {
// vtable +0xDEB90 in ModuleMiniGame0.cro, offset_to_top 0, 5 entries
class EscFollowParticle : public ::esc::EscFollowBase
{
public:
    class FollowParticleData;
    EscFollowParticle(); // ctor address unknown
};
} // namespace esc
