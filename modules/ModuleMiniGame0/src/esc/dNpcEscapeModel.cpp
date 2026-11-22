#include "Npc/dNpcModel.h"
#include "esc/dNpcEscapeModel.h"

namespace esc {
// ctor address unknown
// (built inline with NpcModel(true, true, true) in AcNpcEscape, ModuleMiniGame0.cro +0x00E838)
esc::NpcEscapeModel::NpcEscapeModel() : NpcModel(true, true, true)
{
}

// ModuleMiniGame0.cro +0x011928 slot 0x08
esc::NpcEscapeModel::~NpcEscapeModel()
{
}

} // namespace esc
