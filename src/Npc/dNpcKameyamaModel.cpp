#include "Npc/dNpcSimpleModel.h"
#include "Npc/dNpcKameyamaModel.h"

// ctor address unknown
NpcKameyamaModel::NpcKameyamaModel()
{
}

// 0x002BB3E0 slot 0x00 | virtual slot, introduced by HumanModel
bool NpcKameyamaModel::PlayAnim(u16, u32, u32, f32, f32, f32, bool*, u8)
{
}

// 0x002BB4C0 slot 0x1C | slot vf_0x1C of HumanModel
NpcKameyamaModel::~NpcKameyamaModel()
{
}

