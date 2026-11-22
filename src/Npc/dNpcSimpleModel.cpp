#include "Npc/dNpcModel.h"
#include "Npc/dNpcSimpleModel.h"

// ctor address unknown
// (the special NPCs build it inline with NpcModel(false, false, false), e.g. 0x001F1F28)
NpcSimpleModel::NpcSimpleModel() : NpcModel(false, false, false)
{
}

// 0x00270B0C slot 0x1C | slot vf_0x1C of HumanModel
NpcSimpleModel::~NpcSimpleModel()
{
}

