// The Bug-Off of the town (SvInsectConvention) and its record in the shared data: the original dSvInsectConvention.cpp.
// All names but the class names are ours.

#include "Sv/dSvTown.h"

#include <cstring>

#include "Ac/dAcNpc.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgr_NpcForeachFunction.h"
#include "Other/dMailLoadAdapt.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvInsectConventionExtern.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvShared.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"
#include "netgame/dGameFunctions.h"
#include "script/dPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"
#include "sead/seadDelegate1.h"
#include "stage/stage_Extern.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

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

// (inline) the points of a caught insect: its size times the factor times a random factor
DECOMP_ALWAYS_INLINE u16 GetPoints(u8 size, f32 factor, f32 random)
{
    return static_cast<u32>(static_cast<f32>(size) * factor * random);
}
} // namespace

// (inline) a place is removed: the places below move up, the last place is cleared
DECOMP_ALWAYS_INLINE void RemovePlace(SvInsectConvention* convention, u32 place)
{
    switch (place) {
    case 0:
        netgame::AssignPersonalId(&convention->unk_0x1A[0], &convention->unk_0x1A[1]);
        Func6148BC(&convention->unk_0xA4[0], &convention->unk_0xA4[1]);
        convention->unk_0x134[0] = convention->unk_0x134[1];
        convention->unk_0x13A[0] = convention->unk_0x13A[1];
        // fall through
    case 1:
        netgame::AssignPersonalId(&convention->unk_0x1A[1], &convention->unk_0x1A[2]);
        Func6148BC(&convention->unk_0xA4[1], &convention->unk_0xA4[2]);
        convention->unk_0x134[1] = convention->unk_0x134[2];
        convention->unk_0x13A[1] = convention->unk_0x13A[2];
        // fall through
    case 2:
        netgame::ClearPersonalId(&convention->unk_0x1A[2]);
        Func30836C(&convention->unk_0xA4[2]);
        convention->unk_0x134[2] = 0;
        convention->unk_0x13A[2] = SvFgName(SvFgName::s_Empty);
        break;
    }
}

// 0x002EA430 (name is ours)
void SvInsectConvention::Func2EA430()
{
    bool isReset = false;
    if (unk_0x0 & 1) {
        const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
        const nn::fnd::DateTimeParameters start = time::SvTimeToParameters(&unk_0x9);
        const nn::fnd::DateTimeParameters end = time::SvTimeToParameters(&unk_0x11);
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
    Func2EB558(isReset);
    Func2EB680();
    Func2EB0CC(true, false, true);
    Func2EBB9C(0);
    if ((unk_0x0 & 1) && !(unk_0x0 & 0x80)) {
        Func2EBDFC();
    }
}

// 0x002EA5B0 (name is ours)
void SvInsectConvention::Func2EA5B0(u32 place)
{
    RemovePlace(this, place);
}

// 0x002EA68C (name is ours)
void SvInsectConvention::Func2EA68C(u32 value)
{
    Func2EB0CC(1, 0, 0);
    Func2EBB9C(value);
    if ((unk_0x0 & 1) && !(unk_0x0 & 0x80)) {
        Func2EBDFC();
    }
}

// 0x002EA6D8 (name is ours)
DECOMP_NOINLINE bool SvInsectConvention::Func2EA6D8(const SvNpcRef* npc, const SvFgName* item, u16 points)
{
    if (!Func302050(npc)) {
        return false;
    }
    if (points >= 1024) {
        return false;
    }
    if (unk_0x134[0] == points || unk_0x134[1] == points || unk_0x134[2] == points) {
        return false;
    }
    if (points > unk_0x134[0]) {
        netgame::AssignPersonalId(&unk_0x1A[2], &unk_0x1A[1]);
        netgame::AssignPersonalId(&unk_0x1A[1], &unk_0x1A[0]);
        netgame::ClearPersonalId(&unk_0x1A[0]);
        Func6148BC(&unk_0xA4[2], &unk_0xA4[1]);
        Func6148BC(&unk_0xA4[1], &unk_0xA4[0]);
        Func6148BC(&unk_0xA4[0], npc);
        unk_0x134[2] = unk_0x134[1];
        unk_0x134[1] = unk_0x134[0];
        unk_0x134[0] = points;
        unk_0x13A[2] = unk_0x13A[1];
        unk_0x13A[1] = unk_0x13A[0];
        unk_0x13A[0] = *item;
    } else if (unk_0x134[1] < points) {
        netgame::AssignPersonalId(&unk_0x1A[2], &unk_0x1A[1]);
        netgame::ClearPersonalId(&unk_0x1A[1]);
        Func6148BC(&unk_0xA4[2], &unk_0xA4[1]);
        Func6148BC(&unk_0xA4[1], npc);
        unk_0x134[2] = unk_0x134[1];
        unk_0x134[1] = points;
        unk_0x13A[2] = unk_0x13A[1];
        unk_0x13A[1] = *item;
    } else if (unk_0x134[2] < points) {
        netgame::ClearPersonalId(&unk_0x1A[2]);
        Func6148BC(&unk_0xA4[2], npc);
        unk_0x134[2] = points;
        unk_0x13A[2] = *item;
    }
    return true;
}

// 0x002EA860 (name is ours)
SvNpcRef* SvInsectConvention::Func2EA860(s32 place)
{
    return &unk_0xA4[place < 3 ? place : 0];
}

// 0x002EA874 (name is ours)
SvFgName* SvInsectConvention::Func2EA874(s32 place)
{
    return &unk_0x13A[place < 3 ? place : 0];
}

// 0x002EA888 (name is ours)
void SvInsectConvention::Func2EA888(const SvPersonalId* player, const SvFgName* item, u16 points)
{
    if (!player->IsValid() || points >= 1024) {
        return;
    }
    if (unk_0x134[0] < points) {
        netgame::AssignPersonalId(&unk_0x1A[2], &unk_0x1A[1]);
        netgame::AssignPersonalId(&unk_0x1A[1], &unk_0x1A[0]);
        netgame::AssignPersonalId(&unk_0x1A[0], player);
        Func6148BC(&unk_0xA4[2], &unk_0xA4[1]);
        Func6148BC(&unk_0xA4[1], &unk_0xA4[0]);
        Func30836C(&unk_0xA4[0]);
        unk_0x134[2] = unk_0x134[1];
        unk_0x134[1] = unk_0x134[0];
        unk_0x134[0] = points;
        unk_0x13A[2] = unk_0x13A[1];
        unk_0x13A[1] = unk_0x13A[0];
        unk_0x13A[0] = *item;
    } else if (unk_0x134[1] < points) {
        netgame::AssignPersonalId(&unk_0x1A[2], &unk_0x1A[1]);
        netgame::AssignPersonalId(&unk_0x1A[1], player);
        Func6148BC(&unk_0xA4[2], &unk_0xA4[1]);
        Func30836C(&unk_0xA4[1]);
        unk_0x134[2] = unk_0x134[1];
        unk_0x134[1] = points;
        unk_0x13A[2] = unk_0x13A[1];
        unk_0x13A[1] = *item;
    } else if (unk_0x134[2] < points) {
        netgame::AssignPersonalId(&unk_0x1A[2], player);
        Func30836C(&unk_0xA4[2]);
        unk_0x134[2] = points;
        unk_0x13A[2] = *item;
    }
}

// 0x002EA9FC (name is ours)
SvPersonalId* SvInsectConvention::Func2EA9FC(s32 place)
{
    return &unk_0x1A[place < 3 ? place : 0];
}

// 0x002EAA1C (name is ours)
DECOMP_NOINLINE bool SvInsectConvention::Func2EAA1C(const SvNpcRef* npc)
{
    if (npc == NULL) {
        return false;
    }
    if (!Func302050(npc)) {
        return false;
    }
    const u32 random = GetRandom(100);
    u32 kind = 0;
    f32 factor = GetRandomF(0.5f) + 0.7f;
    if (random >= 50) {
        kind = 1;
        factor = GetRandomF(0.5f) + 0.7f;
    }
    SvFgName item;
    u8 size;
    f32 sizeFactor;
    Func701518(&item, &size, &sizeFactor, kind, time::GetCurrentDate());
    if (size > 30) {
        return false;
    }
    const u16 points = GetPoints(size, sizeFactor, factor);
    if (unk_0x134[0] == points || unk_0x134[1] == points || unk_0x134[2] == points) {
        return false;
    }
    bool isEntered = true;
    for (s32 i = 0; i < 3; i++) {
        if (IsSameNpc(*npc, unk_0xA4[i])) {
            if (unk_0x134[i] >= points) {
                isEntered = false;
            } else {
                RemovePlace(this, i);
            }
        }
    }
    if (isEntered) {
        isEntered = Func2EA6D8(npc, &item, points);
    }
    return isEntered;
}

// 0x002EACF4 (name is ours)
DECOMP_NOINLINE void SvInsectConvention::Func2EACF4(const SvNpcRef* npc, const nn::fnd::DateTimeParameters* date, bool isLimited)
{
    if (npc == NULL) {
        return;
    }
    if (!Func302050(npc)) {
        return;
    }
    const u32 random = GetRandom(1000);
    u32 kind = 0;
    f32 factor = 0.7f;
    if (unk_0x134[0] < 80) {
        if (random < 10) {
            kind = 3;
        } else if (random - 10 < 10) {
            if (random & 1) {
                kind = 2;
                factor = GetRandomF(0.45f) + factor;
            } else {
                kind = 3;
                factor = GetRandomF(0.4f) + factor;
            }
        } else if (random - 20 < 80) {
            if (random & 1) {
                kind = 1;
                factor = GetRandomF(0.5f) + factor;
            } else {
                kind = 2;
                factor = GetRandomF(0.45f) + factor;
            }
        } else if (random - 100 < 200) {
            if (!(random & 1)) {
                kind = 1;
            }
            factor = GetRandomF(0.5f) + factor;
        } else {
            return;
        }
    } else {
        if (random < 10) {
            kind = (random & 1) ? 5 : 4;
            factor = GetRandomF(0.4f) + factor;
        } else if (random - 10 < 10) {
            kind = 3;
            factor = GetRandomF(0.4f) + factor;
        } else if (random - 20 < 80) {
            if (random & 1) {
                kind = 1;
                factor = GetRandomF(0.5f) + factor;
            } else {
                kind = 2;
                factor = GetRandomF(0.45f) + factor;
            }
        } else if (random - 100 < 200) {
            if (!(random & 1)) {
                kind = 1;
            }
            factor = GetRandomF(0.5f) + factor;
        } else {
            return;
        }
    }
    SvFgName item;
    u8 size;
    f32 sizeFactor;
    Func701518(&item, &size, &sizeFactor, kind, date);
    const u16 points = GetPoints(size, sizeFactor, factor);
    if (isLimited) {
        u32 limit = 60;
        if (date->hour < 12) {
            limit = 30;
        } else if (date->hour < 16) {
            limit = 40;
        }
        if (points > limit) {
            return;
        }
    }
    if (unk_0x134[0] == points || unk_0x134[1] == points || unk_0x134[2] == points) {
        return;
    }
    bool isEntered = true;
    for (s32 i = 0; i < 3; i++) {
        if (IsSameNpc(*npc, unk_0xA4[i])) {
            if (unk_0x134[i] >= points) {
                isEntered = false;
            } else {
                RemovePlace(this, i);
            }
        }
    }
    if (isEntered && Func2EA6D8(npc, &item, points)) {
        unk_0x0 |= 8;
    }
}

namespace {
// (the fields of an NPC actor that the Bug-Off reads; AcNpc has no layout yet)
DECOMP_ALWAYS_INLINE u8 GetActorByte9A8(const AcNpc* npc)
{
    return reinterpret_cast<const u8*>(npc)[0x9A8];
}

DECOMP_ALWAYS_INLINE const void* GetActorObject868(const AcNpc* npc)
{
    return reinterpret_cast<const u8*>(npc) + 0x868;
}

DECOMP_ALWAYS_INLINE s32 GetActorNpcIndex(const AcNpc* npc)
{
    s32 index;
    std::memcpy(&index, reinterpret_cast<const u8*>(npc) + 0x1030, sizeof(index));
    return index;
}

// marks the NPCs of the list of BsNpcMgr in order (the actor is free and its object passes Func753588)
// RTTI N47_GLOBAL__N__23_dSvInsectConvention_cpp_5e7668e423KameyamaForeachFunctionE
// vtable 0x0088F724 (vptr 0x0088F72C)
class KameyamaForeachFunction : public BsNpcMgr::NpcForeachFunction
{
public:
    KameyamaForeachFunction() : m_Num(0)
    {
        for (u32 i = 0; i < 10; i++) {
            m_IsMarked[i] = false;
        }
    }

    void Invoke(AcNpc* npc) override; // 0x0052A294 slot 0x00

    bool m_IsMarked[10]; // 0x04
    s32 m_Num;           // 0x10 the number of NPCs handed in
};

// the result notice of the Bug-Off: the message and the three places with their points (the words of the delegate)
class InsectBbsNotice
{
public:
    InsectBbsNotice()
    {
        for (u32 i = 0; i < 3; i++) {
            netgame::ClearPersonalId(&m_Players[i]);
            Func30836C(&m_Npcs[i]);
            m_Points[i] = -1;
        }
    }

    // the name of the player or villager of each place is the word 2B4/place, the points the number word 4/6/place
    void SetWords(script::Phrase* phrase); // 0x0052A08C

    SvBulletinBoardMessage m_Message; // 0x000
    SvPersonalId m_Players[3];        // 0x1AC
    SvNpcRef m_Npcs[3];               // 0x236
    u32 unk_0x2C8;
    s32 m_Points[3];                  // 0x2CC
};

// the delegate of the words of the result notice (set while the notice is written)
// 0x00AD6374
sead::Delegate1<InsectBbsNotice, script::Phrase*> s_NoticeDelegate;
// the method of the delegate / none
// 0x0088F6F8
void (InsectBbsNotice::*const s_NoticeMethod)(script::Phrase*) = &InsectBbsNotice::SetWords;
// 0x0088F700
void (InsectBbsNotice::*const s_NoNoticeMethod)(script::Phrase*) = NULL;

// (inline) the next Bug-Off from a date: the next Saturday on the day of the event
DECOMP_ALWAYS_INLINE nn::fnd::DateTimeParameters GetNextDay(const nn::fnd::DateTimeParameters* from)
{
    nn::fnd::DateTimeParameters date = *from;
    time::Func308EF0(&date);
    if (event::IsEventOnDate(event::EVENT_ID_BUG_OFF, date, true)) {
        event::AddDays(&date, 7);
    }
    while (date.dayOfWeek != 6) {
        event::AddDays(&date, 1);
    }
    while (!event::IsEventOnDate(event::EVENT_ID_BUG_OFF, date, true)) {
        event::AddDays(&date, 7);
    }
    return date;
}
} // namespace

// (the class is only used through the delegate; name is ours)
// 0x002EB494
nn::fnd::DateTimeParameters SvInsectConventionGetNextDay(const nn::fnd::DateTimeParameters* from)
{
    return GetNextDay(from);
}

// (inline) a villager is picked from the set: first one that may take part, then one of state 1 (else the others are
// removed), then a free villager of the town (-1: none)
DECOMP_ALWAYS_INLINE s32 PickVillager(SvInsectConvention* convention, BitSet32* villagers, bool isFirst, bool check)
{
    BitSet32 set;
    set = *villagers;
    s32 index;
    if (isFirst) {
        for (s32 i = 0; i < 10; i++) {
            if (!convention->Func2EB97C(i, false, false, check)) {
                set.Remove(i);
            }
        }
    }
    if (set.count != 0) {
        index = set.PickRandom();
    } else {
        set = *villagers;
        for (s32 i = 0; i < 10; i++) {
            if (convention->Func2EB97C(i, true, false, false)) {
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

// 0x002EB0CC (name is ours)
DECOMP_NOINLINE void SvInsectConvention::Func2EB0CC(bool a, bool b, bool c)
{
    if (!(unk_0x0 & 1) || (unk_0x0 & 2)) {
        return;
    }
    const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
    if (!time::SvTimeIsValid(&unk_0x1)) {
        return;
    }
    nn::fnd::DateTimeParameters step = time::SvTimeToParameters(&unk_0x1);
    if (!Func2FB6DC(now, step)) {
        return;
    }
    unk_0x0 &= ~0xC;
    if (!a) {
        nn::fnd::DateTimeParameters first = step;
        Func312848(&first, 0, 0, 30, 0);
        if (now.hour < 18 && !Func2FB6DC(now, first)) {
            return;
        }
        a = true;
    }
    Func312848(&step, 0, 0, 3, 0);
    if (!time::SvTimeIsValid(&unk_0x11)) {
        return;
    }
    const nn::fnd::DateTimeParameters end = time::SvTimeToParameters(&unk_0x11);
    unk_0x0 |= 4;
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
                Func2EACF4(&npc->npc, &step, c);
            }
            isNext = true;
        }
        Func312848(&step, 0, 0, 3, 0);
    }
    Func5CE904(&unk_0x1);
}

// 0x002EB558 (name is ours)
void SvInsectConvention::Func2EB558(bool isReset)
{
    unk_0x0 &= 0x70;
    Func10F514(GetGardenData()->GetNpcData()->npcs);
    time::SvTimeClear(&unk_0x1);
    time::SvTimeClear(&unk_0x9);
    time::SvTimeClear(&unk_0x11);
    netgame::ClearPersonalId(&unk_0x1A[0]);
    netgame::ClearPersonalId(&unk_0x1A[1]);
    netgame::ClearPersonalId(&unk_0x1A[2]);
    Func30836C(&unk_0xA4[0]);
    Func30836C(&unk_0xA4[1]);
    Func30836C(&unk_0xA4[2]);
    unk_0x134[0] = 0;
    unk_0x134[1] = 0;
    unk_0x134[2] = 0;
    unk_0x13A[0] = SvFgName::s_Empty;
    unk_0x13A[1] = SvFgName::s_Empty;
    unk_0x13A[2] = SvFgName::s_Empty;
    SvPlayerData* players = &GetGardenData()->players;
    for (u32 i = 0; i < 4; i++) {
        SvPlayer* player = players->Get(static_cast<u8>(i));
        if (player != NULL) {
            player->unk_0x8A3C.unk_0x42 = 0;
            if (isReset) {
                reinterpret_cast<u8&>(player->unk_0x8A3C.natLetter) &= ~7;
            }
            reinterpret_cast<u8&>(player->unk_0x8A3C.natLetter) &= ~8;
        }
    }
}

// 0x002EB680 (name is ours)
void SvInsectConvention::Func2EB680()
{
    if (!Func2FDE58(event::EVENT_ID_BUG_OFF)) {
        return;
    }
    unk_0x0 |= 1;
    nn::fnd::DateTimeParameters start;
    event::GetStartDate(event::EVENT_ID_BUG_OFF, &start);
    nn::fnd::DateTimeParameters end;
    event::GetEndDate(event::EVENT_ID_BUG_OFF, &end);
    Func312848(&start, 0, 3, 0, 0);
    Func312848(&end, 0, -4, 0, 0);
    time::SvTimeSet(&unk_0x1, start);
    time::SvTimeSet(&unk_0x9, start);
    time::SvTimeSet(&unk_0x11, end);
    if ((unk_0x0 >> 4) == 0) {
        const u32 random = GetRandom(2) + 1;
        unk_0x0 = (unk_0x0 & 0x8F) | ((random << 4) & 0x70);
    }
    BitSet32 villagers;
    villagers.Init(10, true);
    bool isNext = false;
    while (!Func302050(&unk_0xA4[2])) {
        const s32 index = PickVillager(this, &villagers, !isNext, false);
        if (index < 0) {
            return;
        }
        villagers.Remove(index);
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, index));
        if (npc != NULL && !Func2EAA1C(&npc->npc)) {
            villagers.Add(index);
        }
        isNext = true;
    }
}

// 0x002EB97C (name is ours)
DECOMP_NOINLINE bool SvInsectConvention::Func2EB97C(s32 index, bool a, bool b, bool c)
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
    } else if (npc->unk_0x250F == 3 || npc->unk_0x250E != 0) {
        return false;
    }
    if (Func118E3C(GetGardenPlus()->data.npcData.npcs, npc) || Func11A1E0(npc) || !Func312888(npc)) {
        return false;
    }
    if (!c) {
        return true;
    }
    KameyamaForeachFunction func;
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr == NULL) {
        return false;
    }
    for (AcNpc** actor = mgr->m_pNpcsBegin; actor != mgr->m_pNpcsEnd; actor++) {
        if (*actor != NULL) {
            func.Invoke(*actor);
        }
    }
    for (s32 i = 0; i < func.m_Num; i++) {
        if (i < 10 && func.m_IsMarked[i]) {
            if (static_cast<u32>(i) >= mgr->GetNpcNum()) {
                return true;
            }
            AcNpc* actor = mgr->m_pNpcsBegin[i];
            if (actor == NULL || GetActorNpcIndex(actor) == index) {
                return true;
            }
        }
    }
    return false;
}

// 0x002EBB9C (name is ours)
DECOMP_NOINLINE void SvInsectConvention::Func2EBB9C(s32 value)
{
    const nn::fnd::DateTimeParameters today = time::GetGameDate();
    if (time::SvTimeIsValid(&unk_0x9)) {
        const nn::fnd::DateTimeParameters start = time::SvTimeToParameters(&unk_0x9);
        if (!event::Func314104(start, today)) {
            for (u32 i = 0; i < 3; i++) {
                if (unk_0x1A[i].IsValid()) {
                    SvPlayerData* players = &GetGardenPlus()->data.players;
                    const u32 index = Func71AC00(players, &unk_0x1A[i]);
                    SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(Func30A1FC(players, index)));
                    if (!(unk_0x0 & 2) && player != NULL) {
                        reinterpret_cast<u8&>(player->unk_0x8A3C.natLetter) |= 1 << i;
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
            if (!(reinterpret_cast<const u8&>(player->unk_0x8A3C.natLetter) & bit)) {
                continue;
            }
            SvMail mail;
            SvFgName paper;
            Func6BC8A4(&paper, 41, 16);
            SvFgName::Name prize = SvFgName::NAME_EMPTY;
            if (i == 0) {
                prize = SvFgName::NAME_GOLD_BUG_TROPHY;
            } else if (i == 1) {
                prize = SvFgName::NAME_SILVER_BUG_TROPHY;
            } else if (i == 2) {
                prize = SvFgName::NAME_BRONZE_BUG_TROPHY;
            }
            if (mail.Func306374("Mail_SP_Kameyama", i + 1, &player->playerInfo, &paper, 17, 0)) {
                mail.Func769154(SvFgName(static_cast<SvFgName::ID>(prize)), 0);
                if (mail.Func2FF418(1, 1, 0)) {
                    reinterpret_cast<u8&>(player->unk_0x8A3C.natLetter) &= ~bit;
                }
            }
        }
    }
}

// 0x002EBDFC (name is ours)
DECOMP_NOINLINE void SvInsectConvention::Func2EBDFC()
{
    if (!time::SvTimeIsValid(&unk_0x11)) {
        return;
    }
    const nn::fnd::DateTimeParameters now = *time::GetCurrentDate();
    nn::fnd::DateTimeParameters date = time::SvTimeToParameters(&unk_0x11);
    time::Func30A5AC(&date, 0, 3, 0, 0);
    if (Func2FB6DC(date, now)) {
        return;
    }
    unk_0x0 |= 0x80;
    SvTownDataUnk1AE94* board = &GetGardenData()->townData.unk_0x1AE94;
    InsectBbsNotice notice;
    for (u32 i = 0; i < 3; i++) {
        if (unk_0x1A[i].IsValid()) {
            netgame::AssignPersonalId(&notice.m_Players[i], &unk_0x1A[i]);
        } else if (Func302050(&unk_0xA4[i])) {
            Func6148BC(&notice.m_Npcs[i], &unk_0xA4[i]);
        }
        notice.m_Points[i] = unk_0x134[i];
    }
    s_NoticeDelegate.mObject = &notice;
    s_NoticeDelegate.mMethod = s_NoticeMethod;
    if (Func3020CC(&notice.m_Message, "BBS_Event", 63, &date, NULL, NULL, &s_NoticeDelegate)) {
        board->Func302120(&notice.m_Message);
    }
    s_NoticeDelegate.mObject = NULL;
    s_NoticeDelegate.mMethod = s_NoNoticeMethod;
}

// 0x002EA2BC (name is ours)
SvStorageDataUnk153FCC90::SvStorageDataUnk153FCC90()
{
    unk_0x0 = 0;
    unk_0x2 = 0;
    unk_0x4 = 0;
    unk_0x5 = 0;
}

// 0x002EA2D4 (name is ours)
void SvStorageDataUnk153FCInsect::Func2EA2D4(u32 index)
{
    SvStorageDataUnk153FCC90* entry = &unk_0xC90[index];
    entry->unk_0x0 = 0;
    entry->unk_0x2 = 0;
    entry->unk_0x4 = 0;
}

// 0x002EA2F4 (name is ours)
void SvStorageDataUnk153FCInsect::Func2EA2F4()
{
    for (u32 i = 0; i < 50; i++) {
        unk_0x8[i].Clear();
    }
    for (u32 i = 0; i < 4; i++) {
        unk_0xC90[i].unk_0x0 = 0;
        unk_0xC90[i].unk_0x2 = 0;
        unk_0xC90[i].unk_0x4 = 0;
    }
    for (u32 i = 0; i < 50; i++) {
        unk_0x968[i].Clear();
    }
    unk_0xC88 = 0;
    unk_0xC8C = 0;
}

// 0x002EA398 (name is ours)
SvStorageDataUnk153FCInsect::SvStorageDataUnk153FCInsect() : unk_0x0(0), unk_0x4(0)
{
    unk_0xC88 = 0;
    unk_0xC8C = 0;
    for (u32 i = 0; i < 50; i++) {
        unk_0x968[i].Clear();
    }
}

namespace {
// 0x0052A08C (name is ours)
void InsectBbsNotice::SetWords(script::Phrase* phrase)
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
        } else if (Func302050(&m_Npcs[i])) {
            script::WordFix<9> name;
            Func56E310(&name, &m_Npcs[i]);
            phrase->SetWord2B4(name, index);
        }
        script::WordNumDigit number;
        number.Func312CD4(m_Points[i], 10, 10, 4);
        phrase->SetTag4_6(number, index);
    }
}

// 0x0052A294 slot 0x00 (name is ours)
void KameyamaForeachFunction::Invoke(AcNpc* npc)
{
    if (npc != NULL && GetActorByte9A8(npc) == 0 && Func753588(GetActorObject868(npc))) {
        m_IsMarked[m_Num] = true;
    } else {
        m_IsMarked[m_Num] = false;
    }
    m_Num++;
}
} // namespace

// the instances of the delegate of InsectBbsNotice
// 0x0081471C slot 0x00
template void sead::Delegate1<InsectBbsNotice, script::Phrase*>::invoke(script::Phrase*);
// 0x0082A218 slot 0x04
template sead::IDelegate1<script::Phrase*>* sead::Delegate1<InsectBbsNotice, script::Phrase*>::clone(sead::Heap*) const;
