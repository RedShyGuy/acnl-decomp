// The Fishing Tourney of the town (SvFishingConvention; the classes of the file are in the namespace svfscnve): the
// counterpart of the Bug-Off (dSvInsectConvention.cpp) with fish instead of insects. All names but the class names are
// ours.

#include "Sv/dSvTown.h"

#include <cstring>

#include "Ac/dAcNpc.h"
#include "Bs/dBsNpcMgr.h"
#include "Other/dMailLoadAdapt.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvInsectConventionExtern.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvShared.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"
#include "netgame/dGameFunctions.h"
#include "script/dPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"
#include "sead/seadDelegate1.h"
#include "svfscnve/dUomasaForeachFunction.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

namespace svfscnve {
// the result notice of the Fishing Tourney: the message, the fish of the tourney (0: any fish) and the three places
// (the words of the delegate)
class FishingBbsNotice
{
public:
    FishingBbsNotice() : m_FishId(0)
    {
        for (u32 i = 0; i < 3; i++) {
            netgame::ClearPersonalId(&m_Players[i]);
            Func30836C(&m_Npcs[i]);
        }
    }

    // (inline) the player / villager of a place (not for a place from 3)
    void SetPlayer(u32 place, const SvPersonalId* player)
    {
        if (place < 3) {
            netgame::AssignPersonalId(&m_Players[place], player);
        }
    }

    void SetNpc(u32 place, const SvNpcRef* npc)
    {
        if (place < 3) {
            Func6148BC(&m_Npcs[place], npc);
        }
    }

    // the name of the player or villager of each place is the word 2B4/place, the fish the word 2C0/0
    void SetWords(script::Phrase* phrase); // 0x006CD4F4

    SvBulletinBoardMessage m_Message; // 0x000
    u16 m_FishId;                     // 0x1AC
    SvPersonalId m_Players[3];        // 0x1AE
    SvNpcRef m_Npcs[3];               // 0x238
};
} // namespace svfscnve

namespace {
// (inline) two towns / villager references are the same
DECOMP_ALWAYS_INLINE bool IsSameTown(const SvTownId& a, const SvTownId& b)
{
    return a.tid == b.tid && a.unk_0x14 == b.unk_0x14 && std::memcmp(a.dataTownName, b.dataTownName, 18) == 0;
}

DECOMP_ALWAYS_INLINE bool IsSameNpc(const SvNpcRef& a, const SvNpcRef& b)
{
    return a.villagerId == b.villagerId && IsSameTown(a.townData1, b.townData1) && IsSameTown(a.townData2, b.townData2);
}

// the delegate of the words of the result notice (set while the notice is written)
// 0x00AD5AE4
sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*> s_FishingNoticeDelegate;
// the method of the delegate / none
// 0x0088F694
void (svfscnve::FishingBbsNotice::*const s_FishingNoticeMethod)(script::Phrase*) = &svfscnve::FishingBbsNotice::SetWords;
// 0x0088F69C
void (svfscnve::FishingBbsNotice::*const s_FishingNoNoticeMethod)(script::Phrase*) = NULL;
} // namespace

// (inline) a place is removed: the places below move up, the last place is cleared
DECOMP_ALWAYS_INLINE void RemovePlaceInline(SvFishingConvention* convention, u32 place)
{
    switch (place) {
    case 0:
        netgame::AssignPersonalId(&convention->placePlayers[0], &convention->placePlayers[1]);
        Func6148BC(&convention->placeVillagers[0], &convention->placeVillagers[1]);
        convention->placePoints[0] = convention->placePoints[1];
        convention->placeFish[0] = convention->placeFish[1];
        // fall through
    case 1:
        netgame::AssignPersonalId(&convention->placePlayers[1], &convention->placePlayers[2]);
        Func6148BC(&convention->placeVillagers[1], &convention->placeVillagers[2]);
        convention->placePoints[1] = convention->placePoints[2];
        convention->placeFish[1] = convention->placeFish[2];
        // fall through
    case 2:
        netgame::ClearPersonalId(&convention->placePlayers[2]);
        Func30836C(&convention->placeVillagers[2]);
        convention->placePoints[2] = 0;
        convention->placeFish[2] = SvFgName(SvFgName::s_Empty);
        break;
    }
}

// (inline) a villager is picked from the set: first one that may take part, then one of state 1 (else the others are
// removed), then a free villager of the town (-2: none)
DECOMP_ALWAYS_INLINE s32 PickVillager(SvFishingConvention* convention, BitSet32* villagers, bool isFirst, bool check)
{
    BitSet32 set;
    set = *villagers;
    s32 index;
    if (isFirst) {
        for (s32 i = 0; i < 10; i++) {
            if (!convention->CanVillagerTakePart(i, false, false, check)) {
                set.Remove(i);
            }
        }
    }
    if (set.count != 0) {
        index = set.PickRandom();
    } else {
        set = *villagers;
        for (s32 i = 0; i < 10; i++) {
            if (convention->CanVillagerTakePart(i, true, false, false)) {
                set.Add(i);
            } else {
                set.Remove(i);
            }
        }
        if (set.count != 0) {
            index = set.PickRandom();
        } else {
            index = -1;
            for (s32 i = 0; i < 10; i++) {
                SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, i));
                if (npc != NULL && Func309D64(i) < 0 && Func312888(npc) && !Func11A1E0(npc) &&
                    Func118E3C(GetGardenPlus()->data.npcData.npcs, npc)) {
                    index = i;
                    break;
                }
            }
            if (index < 0) {
                return -2;
            }
        }
    }
    *villagers = set;
    return index;
}

// 0x002FFF68 (name is ours)
void SvFishingConvention::UpdateDaily()
{
    bool isReset = false;
    if (flags & 1) {
        const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
        const nn::fnd::DateTimeParameters start = time::SvTimeToParameters(&startTime);
        const nn::fnd::DateTimeParameters end = time::SvTimeToParameters(&endTime);
        nn::fnd::DateTimeParameters day = start;
        time::Func30A5AC(&day, 0, 21, 0, 0);
        if (Func2FB6DC(day, now) && time::SvTimeIsValid(&GetGardenPlus()->data.townData.unk_0x1AE6E)) {
            const nn::fnd::DateTimeParameters last = time::SvTimeToParameters(&GetGardenPlus()->data.townData.unk_0x1AE6E);
            if (!Func2FB6DC(last, now)) {
                return;
            }
            if (Func2FB6DC(now, start) && Func2FB6DC(now, end)) {
                return;
            }
            isReset = true;
        }
    }
    Clear(isReset);
    Start();
    AdvanceTime(true, false, true);
    GivePrizes(0);
    if ((flags & 1) && !(flags & 0x80)) {
        PostResultNotice();
    }
}

// 0x00300658 (name is ours)
void SvFishingConvention::RemovePlace(u32 place)
{
    RemovePlaceInline(this, place);
}

// 0x0030096C (name is ours)
DECOMP_NOINLINE bool SvFishingConvention::EnterVillager(const SvNpcRef* npc, const SvFgName* fish, u16 points)
{
    if (!Func302050(npc)) {
        return false;
    }
    if (points >= 16384) {
        return false;
    }
    if (placePoints[0] == points || placePoints[1] == points || placePoints[2] == points) {
        return false;
    }
    if (points > placePoints[0]) {
        netgame::AssignPersonalId(&placePlayers[2], &placePlayers[1]);
        netgame::AssignPersonalId(&placePlayers[1], &placePlayers[0]);
        netgame::ClearPersonalId(&placePlayers[0]);
        Func6148BC(&placeVillagers[2], &placeVillagers[1]);
        Func6148BC(&placeVillagers[1], &placeVillagers[0]);
        Func6148BC(&placeVillagers[0], npc);
        placePoints[2] = placePoints[1];
        placePoints[1] = placePoints[0];
        placePoints[0] = points;
        placeFish[2] = placeFish[1];
        placeFish[1] = placeFish[0];
        placeFish[0] = *fish;
    } else if (placePoints[1] < points) {
        netgame::AssignPersonalId(&placePlayers[2], &placePlayers[1]);
        netgame::ClearPersonalId(&placePlayers[1]);
        Func6148BC(&placeVillagers[2], &placeVillagers[1]);
        Func6148BC(&placeVillagers[1], npc);
        placePoints[2] = placePoints[1];
        placePoints[1] = points;
        placeFish[2] = placeFish[1];
        placeFish[1] = *fish;
    } else if (placePoints[2] < points) {
        netgame::ClearPersonalId(&placePlayers[2]);
        Func6148BC(&placeVillagers[2], npc);
        placePoints[2] = points;
        placeFish[2] = *fish;
    }
    return true;
}

// 0x00300CAC (name is ours)
void SvFishingConvention::EnterPlayer(const SvPersonalId* player, const SvFgName* fish, u16 points)
{
    if (!player->IsValid() || points >= 16384) {
        return;
    }
    if (placePoints[0] < points) {
        netgame::AssignPersonalId(&placePlayers[2], &placePlayers[1]);
        netgame::AssignPersonalId(&placePlayers[1], &placePlayers[0]);
        netgame::AssignPersonalId(&placePlayers[0], player);
        Func6148BC(&placeVillagers[2], &placeVillagers[1]);
        Func6148BC(&placeVillagers[1], &placeVillagers[0]);
        Func30836C(&placeVillagers[0]);
        placePoints[2] = placePoints[1];
        placePoints[1] = placePoints[0];
        placePoints[0] = points;
        placeFish[2] = placeFish[1];
        placeFish[1] = placeFish[0];
        placeFish[0] = *fish;
    } else if (placePoints[1] < points) {
        netgame::AssignPersonalId(&placePlayers[2], &placePlayers[1]);
        netgame::AssignPersonalId(&placePlayers[1], player);
        Func6148BC(&placeVillagers[2], &placeVillagers[1]);
        Func30836C(&placeVillagers[1]);
        placePoints[2] = placePoints[1];
        placePoints[1] = points;
        placeFish[2] = placeFish[1];
        placeFish[1] = *fish;
    } else if (placePoints[2] < points) {
        netgame::AssignPersonalId(&placePlayers[2], player);
        Func30836C(&placeVillagers[2]);
        placePoints[2] = points;
        placeFish[2] = *fish;
    }
}

// 0x00300E90 (name is ours)
DECOMP_NOINLINE bool SvFishingConvention::VillagerCatch(const SvNpcRef* npc)
{
    if (npc == NULL) {
        return false;
    }
    if (!Func302050(npc)) {
        return false;
    }
    u8 kind = 0;
    if (GetRandom(100) >= 50) {
        kind = 1;
    }
    SvFgName fish;
    f32 factor;
    if (targetFish.IsEmpty()) {
        fish = Func6175B0(&factor, kind, -3, -1, time::GetCurrentDate());
    } else {
        const nn::fnd::DateTimeParameters start = time::SvTimeToParameters(&startTime);
        fish = Func61749C(&factor, &kind, &targetFish, &start);
        if (factor > 1.0f) {
            return false;
        }
    }
    const u16 points = Func76981C(fish);
    if (placePoints[0] == points || placePoints[1] == points || placePoints[2] == points) {
        return false;
    }
    bool isEntered = true;
    for (s32 i = 0; i < 3; i++) {
        if (IsSameNpc(*npc, placeVillagers[i])) {
            if (placePoints[i] >= points) {
                isEntered = false;
            } else {
                RemovePlaceInline(this, i);
            }
        }
    }
    if (isEntered) {
        isEntered = EnterVillager(npc, &fish, points);
    }
    return isEntered;
}

// 0x00301BDC (name is ours)
DECOMP_NOINLINE void SvFishingConvention::VillagerCatchAtTime(const SvNpcRef* npc, const nn::fnd::DateTimeParameters* date, bool isLimited)
{
    if (npc == NULL) {
        return;
    }
    if (!Func302050(npc)) {
        return;
    }
    const u32 random = GetRandom(1000);
    u8 kind = 0;
    if (random < 10) {
        kind = 3;
    } else if (random - 10 < 10) {
        kind = (random & 1) ? 2 : 3;
    } else if (random - 20 < 80) {
        kind = (random & 1) ? 1 : 2;
    } else if (random - 100 < 200) {
        if (!(random & 1)) {
            kind = 1;
        }
    } else {
        return;
    }
    if (isLimited && kind != 0) {
        kind = 1;
    }
    SvFgName fish;
    f32 factor;
    if (targetFish.IsEmpty()) {
        if (isLimited) {
            fish = Func6175B0(&factor, kind, -3, date->hour < 12 ? -1 : 1, date);
        } else {
            fish = Func617798(&factor, kind, date);
        }
    } else {
        fish = Func61749C(&factor, &kind, &targetFish, date);
        if (isLimited) {
            const s32 direction = date->hour < 12 ? -1 : 1;
            if (factor > 1.0f + static_cast<f32>(direction) * 0.1f) {
                return;
            }
        }
    }
    const u16 points = Func76981C(fish);
    if (placePoints[0] == points || placePoints[1] == points || placePoints[2] == points) {
        return;
    }
    bool isEntered = true;
    for (s32 i = 0; i < 3; i++) {
        if (IsSameNpc(*npc, placeVillagers[i])) {
            if (placePoints[i] >= points) {
                isEntered = false;
            } else {
                RemovePlaceInline(this, i);
            }
        }
    }
    if (isEntered && EnterVillager(npc, &fish, points)) {
        flags |= 8;
    }
}

// 0x003022F8 (name is ours)
DECOMP_NOINLINE void SvFishingConvention::AdvanceTime(bool a, bool b, bool c)
{
    if (!(flags & 1) || (flags & 2)) {
        return;
    }
    const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
    if (!time::SvTimeIsValid(&progressTime)) {
        return;
    }
    nn::fnd::DateTimeParameters step = time::SvTimeToParameters(&progressTime);
    if (!Func2FB6DC(now, step)) {
        return;
    }
    flags &= ~0xC;
    if (!a) {
        nn::fnd::DateTimeParameters first = step;
        Func312848(&first, 0, 0, 30, 0);
        if (now.hour < 18 && !Func2FB6DC(now, first)) {
            return;
        }
        a = true;
    }
    Func312848(&step, 0, 0, 3, 0);
    if (!time::SvTimeIsValid(&endTime)) {
        return;
    }
    const nn::fnd::DateTimeParameters end = time::SvTimeToParameters(&endTime);
    flags |= 4;
    BitSet32 villagers;
    villagers.Init(10, true);
    bool isNext = false;
    while (Func2FB6DC(now, step) && !Func2FB6DC(step, end)) {
        if (a) {
            s32 index = PickVillager(this, &villagers, !isNext, b);
            if (index == -2) {
                index = -1;
            }
            SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, index));
            if (npc != NULL) {
                VillagerCatchAtTime(&npc->npc, &step, c);
            }
            isNext = true;
        }
        Func312848(&step, 0, 0, 3, 0);
    }
    Func5CE904(&progressTime);
}

// 0x00303000 (name is ours)
void SvFishingConvention::Clear(bool isReset)
{
    flags &= 0x70;
    Func10F514(GetGardenData()->GetNpcData()->npcs);
    time::SvTimeClear(&progressTime);
    time::SvTimeClear(&startTime);
    time::SvTimeClear(&endTime);
    netgame::ClearPersonalId(&placePlayers[0]);
    netgame::ClearPersonalId(&placePlayers[1]);
    netgame::ClearPersonalId(&placePlayers[2]);
    Func30836C(&placeVillagers[0]);
    Func30836C(&placeVillagers[1]);
    Func30836C(&placeVillagers[2]);
    placePoints[0] = 0;
    placePoints[1] = 0;
    placePoints[2] = 0;
    targetFish = SvFgName::s_Empty;
    placeFish[0] = SvFgName::s_Empty;
    placeFish[1] = SvFgName::s_Empty;
    placeFish[2] = SvFgName::s_Empty;
    SvPlayerData* players = &GetGardenData()->players;
    for (u32 i = 0; i < 4; i++) {
        SvPlayer* player = players->Get(static_cast<u8>(i));
        if (player != NULL) {
            player->unk_0x8A3C.unk_0x3E = 0;
            if (isReset) {
                reinterpret_cast<u8&>(player->unk_0x8A3C.chipLetter) &= ~7;
            }
            reinterpret_cast<u8&>(player->unk_0x8A3C.chipLetter) &= ~8;
        }
    }
}

// 0x0030329C (name is ours)
void SvFishingConvention::Start()
{
    if (!Func2FDE58(event::EVENT_ID_FISHING_TOURNEY)) {
        return;
    }
    flags |= 1;
    nn::fnd::DateTimeParameters start;
    event::GetStartDate(event::EVENT_ID_FISHING_TOURNEY, &start);
    nn::fnd::DateTimeParameters end;
    event::GetEndDate(event::EVENT_ID_FISHING_TOURNEY, &end);
    Func312848(&start, 0, 3, 0, 0);
    Func312848(&end, 0, -4, 0, 0);
    time::SvTimeSet(&progressTime, start);
    time::SvTimeSet(&startTime, start);
    time::SvTimeSet(&endTime, end);
    if ((flags >> 4) == 0) {
        const u32 random = GetRandom(2) + 1;
        flags = (flags & 0x8F) | ((random << 4) & 0x70);
    }
    // the tourneys of January, March, May and November have a fish, the others take any fish
    switch (start.month) {
    case 1:
    case 3:
    case 5:
    case 11: {
        f32 factor;
        targetFish = Func6173D4(&factor, -3, 0, &start);
        break;
    }
    case 2:
    case 4:
    case 10:
    case 12:
        targetFish = SvFgName(SvFgName::s_Empty);
        break;
    }
    BitSet32 villagers;
    villagers.Init(10, true);
    bool isNext = false;
    s32 failures = 0;
    while (!Func302050(&placeVillagers[2])) {
        const s32 index = PickVillager(this, &villagers, !isNext, false);
        if (index < 0 || failures > 45) {
            return;
        }
        villagers.Remove(index);
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, index));
        if (npc != NULL && !VillagerCatch(&npc->npc)) {
            villagers.Add(index);
            failures++;
        }
        isNext = true;
    }
}

// 0x003039D0 (name is ours)
DECOMP_NOINLINE bool SvFishingConvention::CanVillagerTakePart(s32 index, bool a, bool b, bool c)
{
    (void)b;
    if (static_cast<u32>(index) >= 10) {
        return false;
    }
    SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, index));
    if (npc == NULL) {
        return false;
    }
    if (a) {
        if (npc->unk_0x250E != 1 || Func309D64(index) >= 0 || npc->unk_0x250F == 3) {
            return false;
        }
    } else if (npc->unk_0x250E != 0 || npc->unk_0x250F == 3) {
        return false;
    }
    if (Func118E3C(GetGardenPlus()->data.npcData.npcs, npc) || Func11A1E0(npc) || !Func312888(npc)) {
        return false;
    }
    if (!c) {
        return true;
    }
    svfscnve::UomasaForeachFunction func;
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    // (unlike the Bug-Off: without the manager the villager may take part)
    if (mgr == NULL) {
        return true;
    }
    for (AcNpc** actor = mgr->m_pNpcsBegin; actor != mgr->m_pNpcsEnd; actor++) {
        if (*actor != NULL) {
            static_cast<BsNpcMgr::NpcForeachFunction&>(func).Invoke(*actor);
        }
    }
    for (s32 i = 0; i < func.m_Num; i++) {
        if (i < 10 && func.m_IsMarked[i]) {
            if (static_cast<u32>(i) >= mgr->GetNpcNum()) {
                return true;
            }
            AcNpc* actor = mgr->m_pNpcsBegin[i];
            if (actor == NULL) {
                return true;
            }
            s32 actorIndex;
            std::memcpy(&actorIndex, reinterpret_cast<const u8*>(actor) + 0x1030, sizeof(actorIndex));
            if (actorIndex == index) {
                return true;
            }
        }
    }
    return false;
}

// 0x00303C6C (name is ours)
DECOMP_NOINLINE void SvFishingConvention::GivePrizes(s32 value)
{
    const nn::fnd::DateTimeParameters today = time::GetGameDate();
    if (time::SvTimeIsValid(&startTime)) {
        const nn::fnd::DateTimeParameters start = time::SvTimeToParameters(&startTime);
        if (!event::Func314104(start, today)) {
            for (u32 i = 0; i < 3; i++) {
                if (placePlayers[i].IsValid()) {
                    SvPlayerData* players = &GetGardenPlus()->data.players;
                    const u32 index = Func71AC00(players, &placePlayers[i]);
                    SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(Func30A1FC(players, index)));
                    if (!(flags & 2) && player != NULL) {
                        reinterpret_cast<u8&>(player->unk_0x8A3C.chipLetter) |= 1 << i;
                    }
                }
            }
        }
    }
    if (value <= 0) {
        return;
    }
    SvPlayerData* players = &GetGardenData()->players;
    for (u32 p = 0; p < 4; p++) {
        SvPlayer* player = players->Get(static_cast<u8>(p));
        if (player == NULL) {
            continue;
        }
        for (u32 i = 0; i < 3; i++) {
            const u8 bit = 1 << i;
            if (!(reinterpret_cast<const u8&>(player->unk_0x8A3C.chipLetter) & bit)) {
                continue;
            }
            SvMail mail;
            SvFgName paper = SvFgName::MakeOfKind(item::KIND_PAPER, 16);
            SvFgName::Name prize = SvFgName::NAME_EMPTY;
            if (i == 0) {
                prize = SvFgName::NAME_GOLD_FISH_TROPHY;
            } else if (i == 1) {
                prize = SvFgName::NAME_SILVER_FISH_TROPHY;
            } else if (i == 2) {
                prize = SvFgName::NAME_BRONZE_FISH_TROPHY;
            }
            if (mail.Func306374("Mail_SP_Uomasa", i + 1, &player->playerInfo, &paper, 16, 0)) {
                mail.Func769154(SvFgName(static_cast<SvFgName::ID>(prize)), 0);
                if (mail.Func2FF418(1, 1, 0)) {
                    reinterpret_cast<u8&>(player->unk_0x8A3C.chipLetter) &= ~bit;
                }
            }
        }
    }
}

// 0x0030454C (name is ours)
DECOMP_NOINLINE void SvFishingConvention::PostResultNotice()
{
    if (!time::SvTimeIsValid(&endTime)) {
        return;
    }
    const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
    nn::fnd::DateTimeParameters date = time::SvTimeToParameters(&endTime);
    time::Func30A5AC(&date, 0, 3, 0, 0);
    if (Func2FB6DC(date, now)) {
        return;
    }
    flags |= 0x80;
    // the result of a tourney with a fish / of a tourney with any fish
    const s32 label = targetFish.IsEmpty() ? 61 : 62;
    SvTownDataUnk1AE94* board = &GetGardenData()->townData.unk_0x1AE94;
    svfscnve::FishingBbsNotice notice;
    if (!targetFish.IsEmpty()) {
        notice.m_FishId = GetFgNameId(&targetFish);
    }
    for (u32 i = 0; i < 3; i++) {
        if (placePlayers[i].IsValid()) {
            notice.SetPlayer(i, &placePlayers[i]);
        } else if (Func302050(&placeVillagers[i])) {
            notice.SetNpc(i, &placeVillagers[i]);
        }
    }
    s_FishingNoticeDelegate.mObject = &notice;
    s_FishingNoticeDelegate.mMethod = s_FishingNoticeMethod;
    if (Func3020CC(&notice.m_Message, "BBS_Event", label, &date, NULL, NULL, &s_FishingNoticeDelegate)) {
        board->Func302120(&notice.m_Message);
    }
    s_FishingNoticeDelegate.mObject = NULL;
    s_FishingNoticeDelegate.mMethod = s_FishingNoNoticeMethod;
}

namespace svfscnve {
// 0x006CD4F4 (name is ours)
void svfscnve::FishingBbsNotice::SetWords(script::Phrase* phrase)
{
    for (u32 i = 0; i < 3; i++) {
        u32 index = 0;
        if (i == 0) {
            index = 0;
        } else if (i == 1) {
            index = 1;
        } else if (i == 2) {
            index = 2;
        }
        if (m_Players[i].IsValid()) {
            script::WordFix<9> name;
            MakePlayerName(&name, m_Players[i].playerData.playerName, m_Players[i].playerData.gender & 1);
            phrase->SetWord2B4(name, index);
        } else {
            script::WordFix<9> name;
            Func56E310(&name, &m_Npcs[i]);
            phrase->SetWord2B4(name, index);
        }
    }
    if (m_FishId != 0) {
        SvFgName fish(static_cast<SvFgName::ID>(m_FishId));
        phrase->SetWord2C0(fish.GetName(NULL), 0);
    }
}
} // namespace svfscnve

// the instances of the delegate of FishingBbsNotice
// 0x00814824 slot 0x00
template void sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A2F0 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<svfscnve::FishingBbsNotice, script::Phrase*>::clone(sead::Heap*) const;
