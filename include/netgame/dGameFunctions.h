#pragma once

#include "Sv/dSvPersonalId.h"
#include "decomp.h"
#include "math/dAngle16.h"
#include "netgame/dMachineBitTable.h"
#include "netgame/netgame_Api.h"
#include "nn/math/math_Vector3.h"
#include "stage/stage_Api.h"

struct SvPlayer;
struct SvGardenData;
namespace applet {
class ErrEulaMgr;
} // namespace applet
namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// Functions that netgame calls: game code that is not decompiled yet and helpers of netgame without their
// own header (names are ours; FuncXXXXXX = named after the address while its purpose is unknown).
void Func10DEA0(void* data); // 0x0010DEA0 (Thumb)
void Func114828(); // 0x00114828 (Thumb)
void Func1148DC(); // 0x001148DC (Thumb)
void Func1AD28C(void* object, u32 param); // 0x001AD28C
void BuildSaveBlocks(NetGameMgr* mgr); // 0x001AE4AC
void AssignPersonalId(SvPersonalId* id, const SvPersonalId* other); // 0x001B5048
void MakePersonalId(SvPersonalId* id, const char16* name, u16 playerId, u32 gender, const SvTownId* town); // 0x001B5050
void Func215508(void* object, u32 player); // 0x00215508
bool Func23C4CC(); // 0x0023C4CC
bool Func23CB00(); // 0x0023CB00
bool Func23CBC0(); // 0x0023CBC0
bool Func23CBF0(); // 0x0023CBF0
bool Func23CC18(); // 0x0023CC18
bool GetStructurePosition(nn::math::VEC3_* position, u32 structure); // 0x00240574
void AssignPlayerId(SvPlayerId* id, const SvPlayerId* other); // 0x0027A9E8
void ClearPlayerId(SvPlayerId* id); // 0x0027AA48
void PrepareSendGarden(SvGardenData* garden); // 0x0027AA74
void FinishSendGarden(SvGardenData* garden); // 0x0027AB14
void Func283200(u32 player, bool flag); // 0x00283200
void Func283234(); // 0x00283234
void Func283328(); // 0x00283328
void Func29D318(void* object); // 0x0029D318
void Func29D3CC(void* object); // 0x0029D3CC
void Func29D43C(void* object); // 0x0029D43C
void Func2C2644(void* object); // 0x002C2644
void Func2D5DC0(); // 0x002D5DC0
void Func2FA1FC(void* object); // 0x002FA1FC
void* Func2FD00C(); // 0x002FD00C
u32 Func2FF63C(u32 max); // 0x002FF63C
void Func30048C(u32 a, u32 b); // 0x0030048C
u32 Func3037DC(void* data); // 0x003037DC
SvPlayer* Func3038EC(u32 saveNo); // 0x003038EC
void SetTimeNow(void* time); // 0x00303EEC
void Func3043F4(void* object); // 0x003043F4
void ClearPersonalId(SvPersonalId* id); // 0x003053D4
void Func30804C(); // 0x0030804C
void OnPlayerAssigned(); // 0x003080CC
void Func32D4E0(); // 0x0032D4E0
bool Func350BA8(); // 0x00350BA8
bool Func350D40(); // 0x00350D40
void Func4E83C0(u32 player, u32 param); // 0x004E83C0
void Func51DD6C(void* info, u32 no); // 0x0051DD6C
void OnPlayerLeft(PlayerNo player); // 0x0051DE9C
void CloseSession(); // 0x0051E178
f32 Func56671C(f32 range); // 0x0056671C
u64 GetClock(); // 0x0056C010
s64 GetCurrentTime(); // 0x0056C448
void ResetStageData(); // 0x0056C7EC
void UpdateClock(); // 0x0056D2B0
math::Angle16 Func57C4F0(const nn::math::VEC3_* from, const nn::math::VEC3_* to); // 0x0057C4F0
void Func582F4C(); // 0x00582F4C
void Func5C401C(); // 0x005C401C
void Func5C4D04(void* object); // 0x005C4D04
void Func5C5874(); // 0x005C5874
SvPlayer* GetSvPlayer(PlayerNo player); // 0x005C592C
void Func5C660C(u32 from, u32 to); // 0x005C660C
void Func5C664C(u32 from, u32 to); // 0x005C664C
void Func5C6688(); // 0x005C6688
void Func5C69C4(); // 0x005C69C4
void Func5C6C4C(); // 0x005C6C4C
void Func5C7050(); // 0x005C7050
void Func5C7284(u32 from, u32 to); // 0x005C7284
void Func5C7AE0(u32 from, u32 to); // 0x005C7AE0
bool Func5C8B84(u32 param, PlayerNo player); // 0x005C8B84
void SetTime(void* time, s64 value); // 0x005CEA18
void ShowError(u32 code, bool unknown); // 0x005CEEA0
u32 GetDefaultRegion(); // 0x005D51AC
bool IsGardenBusy(); // 0x006086B4
void Func608F44(); // 0x00608F44
void Func608F60(); // 0x00608F60
void Func61E708(); // 0x0061E708
void Func61F5F8(); // 0x0061F5F8
void ResetMgr8(); // 0x00625514
s8 GetMgr8Flag(); // 0x0062552C
bool IsLeaveConfirmed(); // 0x00626270
bool IsLeaveSaved(); // 0x00626C90
void SetJoinFinished(); // 0x00626D44
void SetLockUnlocked(); // 0x00626DC8
bool IsJoinDone(); // 0x00626DE0
bool CanSendToAll(PlayerNo player); // 0x00626E00
void ClearLockSource(); // 0x00626E84
void StartEntry(); // 0x00627564
bool AreJoinFlagsSet(); // 0x0062766C
bool IsJoinStarted(); // 0x00627680
void LoadEntry(); // 0x006276A0
void SetJoinAck(); // 0x0062777C
void Func6A7498(void* tour); // 0x006A7498
void Func6A7620(); // 0x006A7620
void Func6A76AC(); // 0x006A76AC
void Func6A7C4C(); // 0x006A7C4C
void Func6A7CC4(); // 0x006A7CC4
bool Func6A7EE8(); // 0x006A7EE8
bool Func6A7F04(u32 time); // 0x006A7F04
void Func6B7ED0(void* shared, u32 param); // 0x006B7ED0
void Func6B9808(void* shared, u32 param); // 0x006B9808
void MakeTownId(SvTownId* town, const char16* name, u32 region); // 0x006BC944
void MakeTownId(SvTownId* town, const char16* name, u16 townId, u32 region); // 0x006BCADC
void PrepareSendPlayer(SvPlayer* player); // 0x006BD500
void FinishSendPlayer(SvPlayer* player); // 0x006BD940
u32 Func6BDA24(SvPlayer* player); // 0x006BDA24
stage::Name Func6C4514(s8 kind); // 0x006C4514
void Func6C45A8(nn::math::VEC3_* center); // 0x006C45A8
stage::Name Func6C4734(s8 kind); // 0x006C4734
void Func6C4750(nn::math::VEC3_* position, u32 no); // 0x006C4750
void Func6C499C(void* tour); // 0x006C499C
void Func6C4A60(void* tour); // 0x006C4A60
void Func6C4D50(void* tour); // 0x006C4D50
s8 Func6C4DD0(s32 tour); // 0x006C4DD0
void Func6C53FC(void* data); // 0x006C53FC
void Func6C547C(void* data, s32 tourName); // 0x006C547C
math::Angle16 Func6DB524(); // 0x006DB524
void CloseCurrentMenu(); // 0x006DD7E4
bool Func710100(); // 0x00710100
bool IsErrorReady(applet::ErrEulaMgr* mgr); // 0x0075A918
} // namespace netgame
