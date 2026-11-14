#include "Sv/dSvStrc.h"

#include <cstring>

#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "stage/stage_Extern.h"

namespace {
// the number of the normal / event structures (name is ours)
const u32 STRC_NUM = 0x38;
const u32 EVENT_PWP_NUM = 2;
// the first id that is no structure
const u16 STRC_ID_EMPTY = 0xFC;

// (inline) a structure with this id and place is removed
DECOMP_ALWAYS_INLINE bool RemoveStrcInline(SvStrcData* data, u8 id, u8 x, u8 y)
{
    for (u32 i = 0; i < STRC_NUM; i++) {
        SvStrc* strc = &data->strcs.strcs[i];
        if (strc->IsEmpty()) {
            continue;
        }
        if (strc->GetId() == id && strc->xCoord == x && strc->yCoord == y) {
            strc->Clear();
            data->normalPwPsAmount--;
            return true;
        }
    }
    return false;
}

// (inline) the words of the unlocked public works projects
DECOMP_ALWAYS_INLINE u32* GetUnlockWords(SvStrcData* data)
{
    return reinterpret_cast<u32*>(&data->unlockedPwPs);
}
} // namespace

// 0x00718EC8 (name is ours)
bool SvStrc::IsEmpty() const
{
    return id >= STRC_ID_EMPTY;
}

// 0x00302720 (name is ours)
u8 SvStrc::GetId() const
{
    return id;
}

// 0x0024EF94 (name is ours)
void SvStrc::Set(u16 newId, u8 x, u8 y)
{
    if (id >= STRC_ID_EMPTY && newId < STRC_ID_EMPTY) {
        xCoord = x;
        id = newId;
        yCoord = y;
    }
}

// 0x0024EFD8 (name is ours)
void SvStrc::Clear()
{
    id = STRC_ID_EMPTY;
    xCoord = 0;
    yCoord = 0;
}

// 0x005CE370 (name is ours)
void SvStrcData::RemovePlayerHouse(u32 saveNo)
{
    if (saveNo >= 4) {
        return;
    }
    u8 house = STRUCTURE_PLAYER_1_HOUSE;
    if (saveNo == 0) {
        house = STRUCTURE_PLAYER_1_HOUSE;
    } else if (saveNo == 1) {
        house = STRUCTURE_PLAYER_2_HOUSE;
    } else if (saveNo == 2) {
        house = STRUCTURE_PLAYER_3_HOUSE;
    } else if (saveNo == 3) {
        house = STRUCTURE_PLAYER_4_HOUSE;
    }
    for (u32 i = 0; i < STRC_NUM; i++) {
        SvStrc* strc = GetStrc(i);
        if (strc->IsEmpty() || strc->GetId() != house) {
            continue;
        }
        RemoveStrcInline(this, strc->GetId(), strc->xCoord, strc->yCoord);
        return;
    }
}

// 0x005CE494 (name is ours)
void SvStrcData::UnlockPwp(SvPwpUnlockId index)
{
    if (index < PWP_UNLOCK_NUM) {
        GetUnlockWords(this)[index >> 5] |= 1 << (index & 31);
    }
}

// 0x005CE4C8 (name is ours)
SvStrc* SvStrcData::GetStrc(u32 index)
{
    if (index < STRC_NUM) {
        return &strcs.strcs[index];
    }
    return &strcs.strcs[0];
}

// 0x005CE4D8 (name is ours)
bool SvStrcData::ClearEventPwps()
{
    for (u32 i = 0; i < EVENT_PWP_NUM; i++) {
        strcs.eventPwps[i].Clear();
    }
    eventPwPsAmount = 0;
    return true;
}

// 0x005CE50C (name is ours)
SvDesignStand* SvStrcData::GetDesignStands()
{
    return stands;
}

// 0x005CE514 (name is ours)
u32 SvStrcData::AddStrc(u16 id, u8 x, u8 y)
{
    for (u32 i = 0; i < STRC_NUM; i++) {
        if (strcs.strcs[i].IsEmpty()) {
            strcs.strcs[i].Set(id, x, y);
            normalPwPsAmount++;
            return i;
        }
    }
    return 0;
}

// 0x005CE584 (name is ours)
u32 SvStrcData::AddEventPwp(u16 id, u8 x, u8 y)
{
    for (u32 i = 0; i < EVENT_PWP_NUM; i++) {
        if (strcs.eventPwps[i].IsEmpty()) {
            strcs.eventPwps[i].Set(id, x, y);
            eventPwPsAmount++;
            return i;
        }
    }
    return 0;
}

// 0x005CE5F4 (name is ours)
bool SvStrcData::RemoveStrc(u16 id, u8 x, u8 y)
{
    for (u32 i = 0; i < STRC_NUM; i++) {
        SvStrc* strc = &strcs.strcs[i];
        if (strc->IsEmpty()) {
            continue;
        }
        if (strc->GetId() == id && strc->xCoord == x && strc->yCoord == y) {
            strc->Clear();
            normalPwPsAmount--;
            return true;
        }
    }
    return false;
}

// 0x005CE688 (name is ours)
void SvStrcData::SetupNewTown()
{
    townTreeSize = 0;
    Func27A61C(stands);
    u32* words = GetUnlockWords(this);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}

// 0x005CE6C8 (name is ours)
void SvStrcData::Clear()
{
    normalPwPsAmount = 0;
    eventPwPsAmount = 0;
    for (u32 i = 0; i < STRC_NUM; i++) {
        strcs.strcs[i].Clear();
    }
    for (u32 i = 0; i < EVENT_PWP_NUM; i++) {
        strcs.eventPwps[i].Clear();
    }
    townTreeSize = 0;
    Func27A65C(stands);
    u32* words = GetUnlockWords(this);
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}

// 0x005CE748 (name is ours)
void SvStrcData::UpdateTownTreeSize()
{
    const s32 hours = GetGardenData()->townData.playtime / 3600;
    u8 size;
    if (hours < 5) {
        size = 0;
    } else if (hours < 20) {
        size = 1;
    } else if (hours < 50) {
        size = 2;
    } else if (hours < 100) {
        size = 3;
    } else if (hours < 150) {
        size = 4;
    } else if (hours < 200) {
        size = 5;
    } else if (hours < 250) {
        size = 6;
    } else {
        size = 7;
    }
    townTreeSize = size;
    const u16 days = GetGardenPlus()->data.townData.daysPlayed;
    u8 sizeByDays;
    if (days < 5) {
        sizeByDays = 0;
    } else if (days < 20) {
        sizeByDays = 1;
    } else if (days < 50) {
        sizeByDays = 2;
    } else if (days < 100) {
        sizeByDays = 3;
    } else if (days < 180) {
        sizeByDays = 4;
    } else if (days < 300) {
        sizeByDays = 5;
    } else if (days < 500) {
        sizeByDays = 6;
    } else {
        sizeByDays = 7;
    }
    if (townTreeSize > sizeByDays) {
        townTreeSize = sizeByDays;
    }
}
