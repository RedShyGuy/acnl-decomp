#pragma once

#include "decomp.h"
#include "Sv/dSvCommon.h"

struct SvBulletinBoardMessage;
struct SvGardenData;
struct SvGardenDataUnk52BB0;
struct SvMinigameData;
struct SvNpc;
struct SvPlayer;
struct SvPlayerData;
struct SvPlayerHouse;
struct SvPlayerHouseList;
struct SvSharedData;
struct SvTownId;

// Game code that the apply functions of the file of SvProc and the file of the save data in memory call (not decompiled yet; names after the address, the
// parameters after the calls).

// the steps that every apply function runs after the data was taken over (the order of the original)
void Func6A6EE0(); // 0x006A6EE0
void Func2422F0(); // 0x002422F0
void Func5C06A4(); // 0x005C06A4
void Func5D1080(); // 0x005D1080
void Func28554C(); // 0x0028554C
void Func226DA0(); // 0x00226DA0
// (byte 0: 4, bytes 1 and 2: 0)
extern u8 g_Unknown94EDD4[3];

void Func114D18(u32 value); // 0x00114D18 (Thumb)
void Func4E8EF0(); // 0x004E8EF0
void Func51D910(); // 0x0051D910
void Func10CCA8(SvNpc* npcs); // 0x0010CCA8 (Thumb)
bool Func5C58D0(); // 0x005C58D0
void Func24DFF4(); // 0x0024DFF4
void Func1DEDF0(); // 0x001DEDF0
void Func1DEE04(); // 0x001DEE04
void Func304D04(); // 0x00304D04
void Func20DE60(s64* time); // 0x0020DE60
void Func1E08AC(SvSharedData* data); // 0x001E08AC
void Func51D3F4(); // 0x0051D3F4
void Func1082E4(SvBulletinBoardMessage* messages); // 0x001082E4 (Thumb)
void Func10F168(SvNpc* npcs); // 0x0010F168 (Thumb)
void Func116D0C(); // 0x00116D0C (Thumb)
void Func30C97C(SvNpc* npcs, u32 a, u32 b); // 0x0030C97C (Thumb)
void Func30C48C(void* data); // 0x0030C48C
bool Func150260(); // 0x00150260
void Func537AD8(); // 0x00537AD8
void Func537EF8(); // 0x00537EF8
void Func27E6F8(); // 0x0027E6F8
void Func27AB50(SvPlayerData* players); // 0x0027AB50
void* Func5C2320(SvPlayer* player); // 0x005C2320
bool Func76E7C0(SvPlayer* player); // 0x0076E7C0
void* Func3180E4(void* object, u8 index); // 0x003180E4
void Func5D07C0(); // 0x005D07C0
void Func3052C4(); // 0x003052C4
void Func305080(); // 0x00305080
void Func2904BC(); // 0x002904BC
void Func27D8CC(); // 0x0027D8CC
struct SvTownDataUnk5C;
void Func27C884(SvTownDataUnk5C* map, s32 index); // 0x0027C884
void Func10E9A4(SvNpc* npcs); // 0x0010E9A4 (Thumb)

// (the parts of a player that SvProc::ApplyMovePlayerPls copies from mvplayer_pls.dat)
SvPlayerHouse* Func3180C8(SvPlayerHouseList* houses, u32 index); // 0x003180C8
void Func24E16C(SvPlayerHouse* house, const SvPlayerHouse* source); // 0x0024E16C
void* Func1DF90C(SvSharedData* data, u32 index); // 0x001DF90C
void Func6CDAB8(void* part, const void* source); // 0x006CDAB8
void Func2BF890(SvMinigameData* data, const void* source, u32 index); // 0x002BF890
void Func2D49E8(SvGardenDataUnk52BB0* data, const void* source, u32 index); // 0x002D49E8

// (called by the functions of the save data in memory after loads, dSvSaveMemory.cpp)
namespace sead {
class Heap;
} // namespace sead
void Func116CF4(); // 0x00116CF4 (Thumb)
void Func1C2410(void* object, sead::Heap* heap); // 0x001C2410
void Func10E7A8(SvNpc* npcs); // 0x0010E7A8 (Thumb)
void Func240868(); // 0x00240868
void Func2D5E14(); // 0x002D5E14
void Func6FA9E4(u32 value); // 0x006FA9E4
void Func242A28(u32 a, u32 b); // 0x00242A28
void Func108350(SvBulletinBoardMessage* messages); // 0x00108350 (Thumb)
void Func226DE4(); // 0x00226DE4
struct SvFgName;
void Func119298(SvNpc* npcs); // 0x00119298 (Thumb)
void Func2FC8A4(SvFgName* items, const SvFgName* item); // 0x002FC8A4
void Func1BB258(); // 0x001BB258
void Func2FC0D0(); // 0x002FC0D0
void Func2FC0DC(); // 0x002FC0DC
void Func5CE904(SvTime* time); // 0x005CE904

// (called by the functions of SvGardenData in dSvSaveMemory.cpp: the parts of the town data; names after the addresses)
struct SvGardenDataUnk52BB0;
struct SvMinigameData;
struct SvNpcData;
struct SvNpcDataUnk1744C;
struct SvPersonalId;
struct SvPlayerData;
struct SvPlayerFlags;
struct SvPwpProject;
struct SvSnowman;
struct SvStrc;
struct SvStrcData;
struct SvTownFlags;
struct SvMuseum;
void Func10D640(SvNpc* npcs, u32 saveNo); // 0x0010D640 (Thumb)
struct SvTownDataUnk1E368;
void Func1B6BB8(SvTownDataUnk1E368* data, u32 saveNo); // 0x001B6BB8
void Func1B66B8(SvTownDataUnk1E368* data, u32 saveNo); // 0x001B66B8
void Func10DF18(SvNpc* npcs, u32 saveNo); // 0x0010DF18 (Thumb)
void Func5BBCF0(SvNpcDataUnk1744C* data, u32 saveNo); // 0x005BBCF0
void Func10DB4C(SvNpc* npcs, u32 saveNo); // 0x0010DB4C (Thumb)
void Func5BD3D0(u8* data, const SvPersonalId* player); // 0x005BD3D0
void Func5BD46C(u8* data, const SvPersonalId* player); // 0x005BD46C
void Func10F040(SvNpc* npcs, SvPlayer* player); // 0x0010F040 (Thumb)
struct SvTownDataUnk1C7A8;
void Func32CEB4(SvTownDataUnk1C7A8* data, u32 saveNo); // 0x0032CEB4
void Func5CE370(SvStrcData* data, u32 saveNo); // 0x005CE370
void Func2EC314(SvPlayerHouseList* houses, u32 saveNo); // 0x002EC314
void Func1147C0(u32 saveNo); // 0x001147C0 (Thumb)
void Func5CE688(SvStrcData* data); // 0x005CE688
void Func2EC334(SvPlayerHouseList* houses); // 0x002EC334
void Func52B20C(SvTime* time); // 0x0052B20C
struct SvTownDataUnkEE28;
void Func1B20B4(SvTownDataUnkEE28* data); // 0x001B20B4

void Func6142B0(SvTownDataUnk5C* map); // 0x006142B0
void Func5CE6C8(SvStrcData* data); // 0x005CE6C8
void Func27B044(SvPlayerData* players); // 0x0027B044
void Func2EC368(SvPlayerHouseList* houses); // 0x002EC368
void Func112550(SvNpcData* data); // 0x00112550 (Thumb)
void Func2BFA34(SvMinigameData* data); // 0x002BFA34
void Func2D4B08(SvGardenDataUnk52BB0* data); // 0x002D4B08
void Func52B260(SvTime* time); // 0x0052B260
void Func1B2740(SvFgName* items); // 0x001B2740
void Func24EFB8(SvPwpProject* project); // 0x0024EFB8
void Func20C0DC(SvStrc* strc); // 0x0020C0DC
void Func1082C4(SvBulletinBoardMessage* messages); // 0x001082C4 (Thumb)
void Func77A53C(SvTownDataUnk1C7A8* data); // 0x0077A53C
struct SvTownDataUnk1CA92;
void Func2EC05C(SvTownDataUnk1CA92* island); // 0x002EC05C
void Func20DE14(SvSnowman* snowmen); // 0x0020DE14
void Func6F485C(SvTownFlags* flags); // 0x006F485C
struct SvTownDataUnk1E448;
void Func24AAE4(SvTownDataUnk1E448* data); // 0x0024AAE4
struct SvMyDesign;
void Func118A8C(SvTime* data); // 0x00118A8C (Thumb)
void Func1DF9E8(SvSharedData* data); // 0x001DF9E8
void Func1B44E0(SvMyDesign* design, u32 value); // 0x001B44E0
