#include "Sv/dSvSaveMemory.h"

#include <cstring>

#include "Other/dHeapInit.h"
#include "Other/dSaveProcess.h"
#include "Other/dVisitorLocalInfo.h"
#include "Sound/dSoundMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvChecksum.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvProcExtern.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"
#include "sead/seadGlobalRandom.h"
#include "netgame/dGameFunctions.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"
#include "nn/fs/fs_Api.h"
#include "sead/seadExpHeap.h"

// 0x005CB0A8 (name is ours)
bool SvIsCrcValid(const void* base, const void* data, u32* checksum, u32 size, bool isLogged)
{
    (void)isLogged;
    const u32 expected = *checksum;
    *checksum = 0;
    const bool isValid = SvCalcCrc32C(data, size - (static_cast<const u8*>(data) - static_cast<const u8*>(base)),
                                      0xFFFFFFFF, 0xFFFFFFFF) == expected;
    *checksum = expected;
    return isValid;
}

// 0x005CB0EC (name is ours)
SvSaveMemoryUnk8* SvGetSaveMemoryUnk8()
{
    return g_SvSaveMemory.unk_0x8;
}

// 0x005CB0FC (name is ours)
bool SvCheckSecureValue()
{
    return nn::fs::CheckSaveDataSecureValue(g_SvSaveMemory.pData->secureValue.secureValue);
}

// 0x005CB110 (name is ours)
u16 SvGetSaveVerifier()
{
    return 158;
}

// 0x005CB118 (name is ours)
void SvIncrementSecureValue()
{
    u64& secureValue = g_SvSaveMemory.pData->secureValue.secureValue;
    secureValue++;
    if (secureValue == 0) {
        secureValue++;
    }
}

// 0x005CB158 (name is ours)
void* SvPrepareSaveWrite(const void* data, u32 size, u32 offset)
{
    if (static_cast<u64>(size) + offset > sizeof(SvGardenPlus) || g_SvSaveMemory.pCopy == NULL) {
        return const_cast<void*>(data);
    }
    u8* part = reinterpret_cast<u8*>(g_SvSaveMemory.pCopy) + offset;
    std::memcpy(part, data, size);
    SvGardenPlus* copy = g_SvSaveMemory.pCopy;
    copy->data.UpdateChecksums();
    copy->sharedData.UpdateChecksums();
    return part;
}

// (a removed call, falls through into SvGetExhibition)
// 0x005CB1C8 (name is ours)
SvExhibition* SvGetExhibitionEntry()
{
    return SvGetExhibition();
}

// 0x005CB1CC (name is ours)
SvExhibition* SvGetExhibition()
{
    return SvExhibitionHolder::GetData();
}

// 0x005CB1D8 (name is ours)
void SvCreateSaveCopy()
{
    if (g_SvSaveMemory.pCopy != NULL) {
        return;
    }
    g_SvSaveMemory.pCopy = static_cast<SvGardenPlus*>(HeapInit::s_pStageHeap->tryAlloc(sizeof(SvGardenPlus), 128));
    if (g_SvSaveMemory.pCopy != NULL) {
        std::memcpy(static_cast<void*>(g_SvSaveMemory.pCopy), g_SvSaveMemory.pData, sizeof(SvGardenPlus));
    }
}

// 0x005CB238 (name is ours)
void SvDestroySaveCopy()
{
    if (g_SvSaveMemory.pCopy != NULL) {
        HeapInit::s_pStageHeap->free(g_SvSaveMemory.pCopy);
        g_SvSaveMemory.pCopy = NULL;
    }
}

// 0x005CB490 (name is ours)
u32 SvGetMinigameDataOffset()
{
    return 0x5033C;
}

// 0x005CB788 (name is ours)
SvExhibition* SvGetExhibitionAlways()
{
    return g_pSvExhibition;
}

// 0x005CB798 (name is ours)
u32 SvGetUnk52BB0Offset()
{
    return 0x52C30;
}

// 0x005CB7EC (name is ours)
void SvClearSaveData(SvGardenPlus* data)
{
    std::memset(static_cast<void*>(data), 0, sizeof(SvGardenPlus));
}

// 0x005CB9C4 (name is ours)
void SvUpdateCrc(const void* base, const void* data, u32* checksum, u32 size)
{
    *checksum = 0;
    *checksum = SvCalcCrc32C(data, size - (static_cast<const u8*>(data) - static_cast<const u8*>(base)), 0xFFFFFFFF,
                             0xFFFFFFFF);
}

// 0x005CB7DC (name is ours)
SvGardenPlus* SvGetSaveMemory()
{
    return g_SvSaveMemory.pData;
}

namespace {
// (inline) the garden data after the checks of the save I/O (GetGardenData, embedded here)
DECOMP_ALWAYS_INLINE SvGardenData* GetGardenDataInline()
{
    IsSaveIoOnMainData();
    IsSaveIoOnWholeFile();
    return &g_SvSaveMemory.pData->data;
}

// (inline) the object 0x003044DC takes the stage heap
DECOMP_ALWAYS_INLINE void AttachStageHeap()
{
    sead::Heap* heap = HeapInit::s_pStageHeap;
    if (heap != NULL) {
        Func1C2410(Func3044DC(), heap);
    }
}
} // namespace

// 0x005CB274 (name is ours)
void Func5CB274(SvGardenPlus* data)
{
    (void)data;
    SvPlayer* player = SvGetCurrentPlayer();
    player->playerFlags.resetPending = 0;
    player->unk_0x8F38 = 0;
    Func116CF4();
    if (player->unk_0x8F77.data[4] & 0x40) {
        GetGardenDataInline()->townData.townFlags.unk_0x1_5 = 1;
    }
}

// 0x005CB2EC (name is ours)
void Func5CB2EC()
{
    if (time::SvTimeIsValid(&g_SvSaveMemory.pData->data.townData.unk_0x1E43C)) {
        time::SetCurrentDate(time::SvTimeToParameters(&GetGardenDataInline()->townData.unk_0x1E43C));
    }
    VisitorLocalInfo::s_pInstance->Func2C2B34();
    AttachStageHeap();
    Func6A6EE0();
    Func139580(Func3044DC());
    Func10E7A8(GetGardenDataInline()->GetNpcData()->npcs);
    Func240868();
    Func2D5E14();
    Func6FA9E4(1);
    Func242A28(1, 1);
    Func2422F0();
    SoundMgr::SUpdateTownMelody();
    Func108350(GetGardenDataInline()->townData.unk_0x1AE94.bBoardMessages);
    Func226DE4();
    SvGardenData* garden = GetGardenDataInline();
    for (u32 i = 0; i < 16; i++) {
        Func6BA474(&garden->townData.unk_0xEDBE.lostAndFoundItems[i]);
    }
}

// 0x005CB49C (name is ours)
void Func5CB49C()
{
    Func119298(g_SvSaveMemory.pData->data.npcData.npcs);
}

// 0x005CB4B4 (name is ours)
void Func5CB4B4(SvGardenPlus* data)
{
    (void)data;
    VisitorLocalInfo::s_pInstance->Func2C2B04();
    VisitorLocalInfo::s_pInstance->Func2C2CDC();
    Func2932C4();
    AttachStageHeap();
    Func6A6EE0();
    Func2422F0();
    Func139580(Func3044DC());
    time::LoadOffset();
    nn::fnd::DateTimeParameters date = time::SvTimeToParameters(&g_SvSaveMemory.pData->data.townData.unk_0xEE0E);
    event::Update(&date, false, false);
    netgame::Func582F4C();
    netgame::Func30048C(1, 1);
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    Func2FC8A4(GetGardenDataInline()->townData.unk_0xEE2C.nooksHomeItems, &player->unk_0x8F0A);
    SvGetCurrentPlayer()->unk_0x8F0A = SvFgName::s_Empty;
    SoundMgr::SUpdateTownMelody();
    SvTownData& town = GetGardenDataInline()->townData;
    if ((town.unk_0x1DAC6 & 0xF0) >> 4 < 10) {
        town.unk_0x1DAC6 += 0x10;
    }
    const u8 value = town.unk_0x1CA8C[4];
    if ((value & 8) && !(value & 0x40)) {
        GetGardenDataInline()->townData.unk_0x1CA8C[4] |= 0x40;
    }
    Func1BB258();
}

// 0x005CB690 (name is ours)
void Func5CB690()
{
    Func2932C4();
    AttachStageHeap();
    Func6A6EE0();
    Func2422F0();
    Func139580(Func3044DC());
    nn::fnd::DateTimeParameters date = time::SvTimeToParameters(&g_SvSaveMemory.pData->data.townData.unk_0xEE0E);
    event::Update(&date, false, false);
    Func2422F0();
    SoundMgr::SUpdateTownMelody();
    Func2FC0D0();
    Func2FC0DC();
    VisitorLocalInfo::s_pInstance->Func2C2B10();
    VisitorLocalInfo::s_pInstance->Func2C2ADC();
    Func1BB258();
}

// 0x005CB7A4 (name is ours)
void Func5CB7A4()
{
    Func5CE904(&GetGardenDataInline()->townData.unk_0x1E43C);
}

// 0x005CB7CC (name is ours)
void Func5CB7CC()
{
    VisitorLocalInfo::s_pInstance->Func2C2AA8();
}

namespace {
// (inline) a new secure value, everything cleared
DECOMP_ALWAYS_INLINE void ResetSaveData(SvGardenPlus* data)
{
    sead::GlobalRandom* random = sead::GlobalRandom::s_pInstance;
    const u32 high = random->getU32();
    const u32 low = random->getU32(0xFFFFFFFF) + 1;
    data->secureValue.saveInitialized = 0;
    data->secureValue.secureValue = static_cast<u64>(high) << 32 | low;
    data->data.Clear();
    Func1DF9E8(&data->sharedData);
}
} // namespace

// 0x005CB7F8 (name is ours)
void Func5CB7F8(SvGardenPlus* data)
{
    ResetSaveData(data);
}

// 0x005CB84C (name is ours)
void SvAllocateSaveMemory()
{
    if (g_SvSaveMemory.pData != NULL) {
        return;
    }
    g_SvSaveMemory.pData =
        static_cast<SvGardenPlus*>(g_SvSaveMemory.unk_0x8->pHeap->tryAlloc(sizeof(SvGardenPlus), 128));
    std::memset(static_cast<void*>(g_SvSaveMemory.pData), 0, sizeof(SvGardenPlus));
    SvGardenPlus* data = g_SvSaveMemory.pData;
    if (data != NULL) {
        new (data) SvGardenPlus();
    }
    ResetSaveData(g_SvSaveMemory.pData);
}

// 0x005CBF6C (name is ours)
SvGardenData::SvGardenData()
{
    // (only the members are built)
}

// 0x005CB9F8 (name is ours)
void SetSvDate(SvDate* date, u16 year, u8 month, u8 day)
{
    date->month = month;
    date->day = day;
    date->year = year;
}

// 0x005CBA08 (name is ours)
SvDate::SvDate(u16 year, u8 month, u8 day) : year(0), month(0), day(0)
{
    SetSvDate(this, year, month, day);
}

// 0x005CBA34 (name is ours)
void SvSaveHeader::Initialize()
{
    saveVerifier1 = 158;
    saveVerifier2 = 0;
}

// 0x005CBA50 (name is ours)
void SvGardenData::AddPlaytime(s64 time)
{
    const u64 sum = static_cast<u64>(townData.playtime) + static_cast<u64>(time);
    if (sum <= 0x7FFFFFFFFFFFFFFFull) {
        townData.playtime = sum;
    } else {
        townData.playtime = 0x7FFFFFFFFFFFFFFFll;
    }
}

// (falls through into SvGardenData::Func5CBAB8)
// 0x005CBAA0 (name is ours)
void Func5CBAA0(SvGardenPlus* data)
{
    Func17D478();
    data->data.Func5CBAB8();
}

// 0x005CBAB8 (name is ours)
void SvGardenData::Func5CBAB8()
{
    time::SvTimeSet(&townData.unk_0x1AE6E, *time::GetCurrentDate());
}

// 0x005CBADC (name is ours)
void SvGardenData::Func5CBADC(u32 saveNo)
{
    if (!SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(saveNo))) {
        return;
    }
    IsSaveIoOnNpcData();
    Func10D640(npcData.npcs, saveNo);
    if (saveNo != 0) {
        Func1B6BB8(&townData.unk_0x1E368, saveNo);
    }
}

// 0x005CBB30 (name is ours)
void SvGardenData::Func5CBB30(u32 saveNo)
{
    if (!SvIsValidPlayerSaveNo(static_cast<PlayerSaveNumber>(saveNo))) {
        return;
    }
    townData.unk_0xEE2C.Func20D640(saveNo);
    Func6BCCBC(&townData.museum, saveNo);
    IsSaveIoOnNpcData();
    SvNpc* npcs = npcData.npcs;
    Func10DF18(npcs, saveNo);
    Func5BBCF0(npcData.unk_0x1744C, saveNo);
    Func10DB4C(npcs, saveNo);
    SvPlayer* player = players.Get(saveNo);
    if (player != NULL) {
        Func5BD3D0(npcData.unk_0x172F8, &player->playerInfo);
        Func5BD46C(npcData.unk_0x172F8, &player->playerInfo);
    }
    player = players.Get(saveNo);
    if (player != NULL) {
        Func10F040(npcs, player);
    }
    Func32CEB4(&townData.unk_0x1C7A8, saveNo);
    Func5CE370(&strcData, saveNo);
    Func2EC314(&townData.playerHouses, saveNo);
    Func1147C0(saveNo);
    if (saveNo != 0) {
        Func1B66B8(&townData.unk_0x1E368, saveNo);
    }
    AttachStageHeap();
    Func6A6EE0();
    Func2422F0();
    Func139580(Func3044DC());
}

// 0x005CBCAC (name is ours)
void SvGardenData::Func5CBCAC()
{
    header.Initialize();
    Func5CE688(&strcData);
    Func2EC334(&townData.playerHouses);
    Func52B20C(&townData.unk_0xEE0E);
    Func1B20B4(&townData.unk_0xEE28);
    townData.unk_0xEE2C.Func10A2C8();
    Func6BD0F0(&townData.museum);
    Func118A7C(&townData.unk_0x1CA84);
    Func1B44E0(&townData.townFlag, 22);
}

// 0x005CBD3C (name is ours)
void SvGardenData::Clear()
{
    Func6142B0(&townData.unk_0x5C);
    Func5CE6C8(&strcData);
    Func27B044(&players);
    Func2EC368(&townData.playerHouses);
    townData.unk_0xEE28.Init();
    townData.unk_0xEE2C.Func5CE224();
    Func6BD128(&townData.museum);
    Func118A8C(&townData.unk_0x1CA84);
    Func112550(&npcData);
    Func2BFA34(&minigameData);
    Func2D4B08(&unk_0x52BB0);
    Func52B260(&townData.unk_0xEE0E);
    event::Func6140F8(&townData.unk_0x1AE50);
    Func1B2740(townData.unk_0xEDBE.lostAndFoundItems);
    time::SvTimeClear(&townData.unk_0x1AE6E);
    Func6BCB9C(townData.unk_0x1AE76);
    Func24EFB8(&townData.nextPwpToBuild);
    Func20C0DC(&townData.unk_0x1AE90);
    Func1082C4(townData.unk_0x1AE94.bBoardMessages);
    Func77A53C(&townData.unk_0x1C7A8);
    townData.unk_0x1C7A8.unk_0x1C.Func303000(true);
    townData.unk_0x1C7A8.unk_0x1C.unk_0x0 = 0;
    townData.unk_0x1C90E.Func2EB558(true);
    townData.unk_0x1C90E.unk_0x0 = 0;
    Func2EC05C(&townData.unk_0x1CA92);
    Func20DE14(townData.snowmen);
    townData.unk_0x1CA8C[4] = 0;
    townData.unk_0x1DABC = 0;
    time::SvTimeClear(&townData.unk_0x1DABE);
    townData.unk_0x1DAC6 = 0;
    netgame::ClearPersonalId(&townData.unk_0x1DACA);
    townData.unk_0x1DAC7 = 0;
    townData.unk_0x1DAC8 = 0;
    Func6F485C(&townData.townFlags);
    townData.townFlag.Clear();
    townData.unk_0x1E444.year = 0;
    townData.unk_0x1E444.month = 0;
    townData.unk_0x1E444.day = 0;
    Func24AAE4(&townData.unk_0x1E448);
    for (u32 i = 0; i < 6; i++) {
        townData.unk_0x1E448.unk_0x14[i] = 0;
    }
    townData.playtime = 0;
}
