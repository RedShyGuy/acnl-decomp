#include "fgobj/fgobj_Globals.h"
#include "fgobj/fgobj_Extern.h"
#include "fgobj/dObjectBase.h"

namespace fgobj {
// 0x0094FE5C
u8 s_Unk94FE5C;
// 0x0094FE5D
u8 s_Unk94FE5D;
// 0x0094FE5E
bool s_IsIndoor;
// 0x0094FE5F
bool s_IsOutdoor;
// 0x0094FE60
u8 s_Unk94FE60;
// 0x0094FE61
u8 s_Unk94FE61;
// 0x0094FE62
u8 s_Unk94FE62;
// 0x0094FE64
s16 s_Unk94FE64;
// 0x0094FE68
MyDesignTextures* s_pMyDesignTextures;
// 0x0094FE6C
SeadParticleHandle* s_pParticleHandles;
// 0x0094FE70
Proc* s_pProc;
// 0x0094FE74
ObjectBase* s_pMagicLampObject;
// 0x0094FE78
f32 s_Unk94FE78;
// 0x0094FE7C
f32 s_Unk94FE7C;
// 0x0094FE94
s32 s_LastModelKey;
// 0x0094FE98
s32 s_LastMaterialKey;
// 0x0094FE9C
const void* s_pLastMesh;
// 0x0094FEA4
u32 s_TreeTextureAddresses[2];
// 0x0094FEAC
const char* s_SettingModelNames[2] = {"setting_t_outside", "setting_t_outside_2"};
// 0x0094FEC4
const char* s_SoldOutTextureNames[REGION_TEXTURE_NUM] = {
    "t_fg_soldout_JP", "t_fg_soldout_US_ENG",
    "t_fg_soldout_US_SPA", "t_fg_soldout_US_FRA",
    "t_fg_soldout_EU_ENG", "t_fg_soldout_EU_SPA",
    "t_fg_soldout_EU_FRA", "t_fg_soldout_ITA",
    "t_fg_soldout_GER", "t_fg_soldout_KR",
};
// 0x0094FEEC
const char* s_GraceSoldOutTextureNames[REGION_TEXTURE_NUM] = {
    "t_fg_grace_soldout_JP", "t_fg_grace_soldout_US_ENG",
    "t_fg_grace_soldout_US_SPA", "t_fg_grace_soldout_US_FRA",
    "t_fg_grace_soldout_EU_ENG", "t_fg_grace_soldout_EU_SPA",
    "t_fg_grace_soldout_EU_FRA", "t_fg_grace_soldout_ITA",
    "t_fg_grace_soldout_GER", "t_fg_grace_soldout_KR",
};
// 0x0094FF14
const char* s_WallSoldOutTextureNames[REGION_TEXTURE_NUM] = {
    "t_fg_wall_soldout_JP", "t_fg_wall_soldout_US_ENG",
    "t_fg_wall_soldout_US_SPA", "t_fg_wall_soldout_US_FRA",
    "t_fg_wall_soldout_EU_ENG", "t_fg_wall_soldout_EU_SPA",
    "t_fg_wall_soldout_EU_FRA", "t_fg_wall_soldout_ITA",
    "t_fg_wall_soldout_GER", "t_fg_wall_soldout_KR",
};
// 0x0094FF3C
const char* s_WallGraceSoldOutTextureNames[REGION_TEXTURE_NUM] = {
    "t_fg_wall_g_soldout_JP", "t_fg_wall_g_soldout_US_ENG",
    "t_fg_wall_g_soldout_US_SPA", "t_fg_wall_g_soldout_US_FRA",
    "t_fg_wall_g_soldout_EU_ENG", "t_fg_wall_g_soldout_EU_SPA",
    "t_fg_wall_g_soldout_EU_FRA", "t_fg_wall_g_soldout_ITA",
    "t_fg_wall_g_soldout_GER", "t_fg_wall_g_soldout_KR",
};
// 0x0094FF64
const char* s_FogModelNames[FOG_MODEL_NUM] = {"fg_soldout", "fg_wall_ftrS", "fg_wall_ftrL"};

// 0x0094FF90
DrawMethod s_DrawMethods[2] = {&ObjectBase::DrawCached, &ObjectBase::DrawAll};

// 0x0098B47C
sead::Heap* s_SmallHeaps[SMALL_HEAP_NUM];
// 0x0098BD14
sead::Heap* s_LargeHeaps[LARGE_HEAP_NUM];

// 0x00847624
extern const f32 s_ScatterOffsets[4][4][2] = {
    {{6.0f, -4.0f}, {0.0f, 0.0f}, {-4.0f, 4.0f}, {4.0f, 0.0f}},
    {{-4.0f, -4.0f}, {-6.0f, 2.0f}, {2.0f, 0.0f}, {-2.0f, 4.0f}},
    {{6.0f, -2.0f}, {8.0f, 0.0f}, {-2.0f, -6.0f}, {4.0f, 4.0f}},
    {{-2.0f, 6.0f}, {4.0f, -6.0f}, {6.0f, -2.0f}, {-2.0f, -4.0f}},
};
} // namespace fgobj

// (owner unknown, see fgobj_Extern.h)
// 0x00954F88
u8 g_Unk954F88 = 10;
// (owner unknown, see fgobj_Extern.h)
// 0x00950530
bool s_Unk950530;
// 0x00950531
bool s_Unk950531;
// (owner unknown, see fgobj_Extern.h)
// 0x00AAF736
UnitBitsAAF736 g_UnitBitsAAF736;
