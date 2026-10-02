#pragma once

#include "decomp.h"
#include "Npc/dNpcModel.h"

namespace esc {
// vtable +0xDEB18 in ModuleMiniGame0.cro, offset_to_top 0, 20 entries
class NpcEscapeModel : public ::NpcModel
{
public:
    NpcEscapeModel(); // ctor address unknown
    virtual ~NpcEscapeModel(); // ModuleMiniGame0.cro +0x011928 slot 0x08
};
} // namespace esc
