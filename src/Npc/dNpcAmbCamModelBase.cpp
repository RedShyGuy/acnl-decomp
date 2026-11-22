#include "Npc/dNpcAmbCamModelBase.h"

#include "Npc/dNpcModel.h"
#include "Npc/dNpcTopsChangeModel.h"
#include "Player/dPlayerModel.h"

// the instances of the human models of the amiibo camera (the vtables and the slots in the template area)

// 0x002E5FEC slot 0x1C
// 0x002E5FDC (deleting dtor)
template NpcAmbCamModelBase<NpcModel>::~NpcAmbCamModelBase();
// 0x007D277C slot 0x24
template void NpcAmbCamModelBase<NpcModel>::SetMtx(const nn::math::MTX34* mtx);

// 0x002E5FD0 slot 0x1C
// 0x002E5FC0 (deleting dtor)
template NpcAmbCamModelBase<NpcTopsChangeModel>::~NpcAmbCamModelBase();
// 0x007D271C slot 0x24
template void NpcAmbCamModelBase<NpcTopsChangeModel>::SetMtx(const nn::math::MTX34* mtx);

// 0x002E5EBC slot 0x1C
// 0x002E5EAC (deleting dtor)
template NpcAmbCamModelBase<PlayerModel>::~NpcAmbCamModelBase();
// 0x007D26BC slot 0x24
template void NpcAmbCamModelBase<PlayerModel>::SetMtx(const nn::math::MTX34* mtx);
