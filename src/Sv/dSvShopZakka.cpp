// The shop of the Nooklings (SvShopZakka, the first shop of the shops of the town): the ARM part of the original
// dSvShopZakka.cpp. All names but the class names are ours.

#include "Sv/dSvTown.h"

#include "Other/dFdInfo.h"
#include "Other/dOrderList.h"
#include "Other/dField.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Item/item_Api.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "dSvShopZakkaNotice.h"
#include "event/event_Extern.h"
#include "script/dScriptExtern.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"
#include "time/time_Extern.h"

namespace {
// the items of the bulletin board of the Nooklings (the labels are the level + 5, + 8 or the time service labels)
const char* const NOTICE_FILE = "BBS_Kodanuki";

// the bells spent that end a level / the value of a level after a renewal (unk_0x10)
// 0x00890760
const u32 s_SpentLimits[5] = {12000, 25000, 50000, 100000, 100000};
// 0x00890774
const s32 s_RenewalValues[5] = {7, 10, 21, 30, 0};

// (inline) the item of a place: the sold-out sign when it is bought
DECOMP_ALWAYS_INLINE SvFgName GetShopItem(const SvShopZakka* shop, u32 index)
{
    if (shop->unk_0x80[index]) {
        if (Func303BD4(&shop->nooklingsItems[index])) {
            return SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_NOOKLINGS_SOLD_OUT_SIGN_WALL));
        }
        return SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_GENERAL_SOLD_OUT_SIGN));
    }
    return shop->nooklingsItems[index];
}

// (inline) the days from a time to today (the date of Func3004A8)
DECOMP_ALWAYS_INLINE s32 GetDaysToToday(const SvTime* time)
{
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    const nn::fnd::DateTimeParameters today = now;
    return Func303850(time, &today, 1, 1);
}

// (inline) the town flags of the save data
DECOMP_ALWAYS_INLINE SvTownFlags* GetTownFlags()
{
    return &GetGardenData()->townData.townFlags;
}
} // namespace

// 0x001E5238 (name is ours)
void SvShopZakka::AddBellsSpent(u32 value)
{
    nooklingBellsSpent.Add(value, 0xFFFFFFFF);
    if (nooklingBellsSpent.Get() >= s_SpentLimits[nooklingState]) {
        nooklingBellsSpent.Set(s_SpentLimits[nooklingState]);
    }
}

// 0x001E5280 (name is ours)
void SvShopZakka::BuyItem(u32 stage, u32 index, s32 x, s32 z, s32 unknown)
{
    if (index >= 24) {
        return;
    }
    const SvFgName item = GetShopItem(this, index);
    if (unk_0x80[index]) {
        return;
    }
    const SvFgName bought = nooklingsItems[index];
    const item::Kind kind = bought.GetKind();
    if (kind == item::KIND_PAPER || kind == item::KIND_MELODY_PAPER || kind == item::KIND_WRAPPING_PAPER) {
        return;
    }
    unk_0x80[index] = 1;
    if (unk_0x98 > 0) {
        unk_0x98--;
    }
    if (stage::GetCurrent() != static_cast<stage::Name>(stage)) {
        return;
    }
    if (Func30B658(&item)) {
        if (Func303BD4(&item)) {
            Func4EA100(x, z, 2, unknown);
        } else {
            Func4EA100(x, z, 1, unknown);
        }
    } else {
        Field* field = FdInfo::GetCurrentField();
        if (field != NULL) {
            field->PlaceItem(SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_GENERAL_SOLD_OUT_SIGN)), x, z, 0);
            Func59F144(x, z, 1);
        }
    }
}

// 0x001E545C (name is ours)
void SvShopZakka::Upgrade(u8 level)
{
    if (!time::SvTimeIsValid(&renovationDate)) {
        Func5CE904(&renovationDate);
        time::SvTimeNormalize(&renovationDate);
        Func5CE8B8(&renovationDate, -1);
    }
    nn::fnd::DateTimeParameters date = time::SvTimeToParameters(&renovationDate);
    event::AddDays(&date, 1);
    SvDate day;
    event::Func30F6DC(&day, &date);
    nooklingStateUnknown = level;
    nooklingBellsSpent.Set(0);
    nooklingState = level;
    unk_0x10 = s_RenewalValues[nooklingState];
    unk_0x98 = 0;
    for (u32 i = 0; i < 4; i++) {
        SvPlayer* player = SvGetPlayer(static_cast<PlayerSaveNumber>(i));
        if (player != NULL && player->playerInfo.IsValid()) {
            player->playerFlags.unk_0x7_4 = 0;
            player->playerFlags.unk_0x7_5 = 0;
        }
    }
    switch (level) {
    case 0:
        if (GetGardenPlus()->data.townData.unk_0xEE2C.leifUnlockStatus != 0) {
            Func30607C(&GetGardenData()->townData.unk_0xEE2C.unk_0x132C0, 0);
        }
        unk_0x98 = 15;
        break;
    case 1:
        GetTownFlags()->unk_0x2_5 = 1;
        Func1B6680(&GetGardenData()->townData.unk_0x1E368, 9, &day);
        break;
    case 2:
        GetTownFlags()->unk_0x2_6 = 1;
        Func1B6680(&GetGardenData()->townData.unk_0x1E368, 10, &day);
        break;
    case 3:
        if (GetGardenPlus()->data.townData.unk_0xEE2C.leifUnlockStatus != 3) {
            Func30607C(&GetGardenData()->townData.unk_0xEE2C.unk_0x132C0, 3);
        }
        GetTownFlags()->unk_0x2_7 = 1;
        Func1B6680(&GetGardenData()->townData.unk_0x1E368, 11, &day);
        break;
    case 4:
        if (GetGardenPlus()->data.townData.unk_0xEE2C.leifUnlockStatus != 4) {
            Func30607C(&GetGardenData()->townData.unk_0xEE2C.unk_0x132C0, 4);
        }
        GetTownFlags()->unk_0x3_0 = 1;
        Func1B6680(&GetGardenData()->townData.unk_0x1E368, 12, &day);
        break;
    }
    NoticeRenewal notice;
    SvBulletinBoardMessage message;
    notice.Write(&message, NOTICE_FILE, nooklingStateUnknown + 8, &date, NULL, NULL);
    GetGardenData()->townData.unk_0x1AE94.Func302120(&message);
}

// 0x001E57AC (name is ours)
DECOMP_NOINLINE void SvShopZakka::UpdateUpgrade(s32 days)
{
    if (days == 0) {
        return;
    }
    if (days > 0) {
        unk_0x10 = days > unk_0x10 ? 0 : unk_0x10 - days;
    }
    if (nooklingState == 3 && nooklingBellsSpent.Get() >= 70000) {
        GetTownFlags()->unk_0x1_0 = 1;
    }
    if (nooklingState == nooklingStateUnknown && nooklingState < 4 && unk_0x10 <= 0) {
        const u32 spent = nooklingBellsSpent.Get();
        if (spent >= s_SpentLimits[nooklingState] || (nooklingState == 0 && unk_0x98 <= 0)) {
            bool isRenewed = true;
            switch (nooklingState) {
            case 0:
                isRenewed = GetGardenPlus()->data.townData.townFlags.unk_0x1_1;
                break;
            case 1: {
                const SvTownDataUnkEE2C* shops = &GetGardenData()->townData.unk_0xEE2C;
                isRenewed = shops->leifUnlockStatus >= 2 && shops->unk_0x132C0 >= 10;
                break;
            }
            case 3:
                isRenewed = (reinterpret_cast<const u8*>(&GetGardenPlus()->data.townData.townFlags)[0] >> 5) >= 4;
                break;
            }
            if (isRenewed) {
                nooklingStateUnknown = nooklingState + 1;
                Func5CE904(&renovationDate);
                Func5CE8B8(&renovationDate, 1);
                time::SvTimeNormalize(&renovationDate);
                goto renewed;
            }
        }
    }
    if (nooklingState != nooklingStateUnknown) {
        if (days < 0) {
            Func5CE904(&renovationDate);
            time::SvTimeNormalize(&renovationDate);
        } else if (Func303850(&renovationDate, time::GetCurrentDate(), 1, 1) > 0) {
            Upgrade(nooklingStateUnknown);
        }
    }
renewed:
    if (nooklingState == nooklingStateUnknown) {
        SvTime time = renovationDate;
        if (time::SvTimeIsValid(&time) && (days < 0 || GetDaysToToday(&renovationDate) >= 2)) {
            time::SvTimeClear(&renovationDate);
        }
    }
}

// 0x001E5A28 (name is ours)
void SvShopZakka::UpdateDay(s32 days)
{
    UpdateUpgrade(days);
    if (days == 0) {
        return;
    }
    bool isStarted = false;
    if (!GetGardenPlus()->data.townData.townFlags.unk_0x0_0) {
        bool isOpen = true;
        SvTime time = renovationDate;
        if (time::SvTimeIsValid(&time)) {
            nn::fnd::DateTimeParameters now;
            time::Func3004A8(&now);
            const nn::fnd::DateTimeParameters today = now;
            time = renovationDate;
            if (Func303850(&time, &today, 1, 1) == -1) {
                isOpen = false;
            }
        }
        if (isOpen && !IsRenovationDay()) {
            time = renovationDate;
            if (time::SvTimeIsValid(&time)) {
                nn::fnd::DateTimeParameters now;
                time::Func3004A8(&now);
                const nn::fnd::DateTimeParameters today = now;
                time = renovationDate;
                if (Func303850(&time, &today, 1, 1) == 1) {
                    isOpen = false;
                }
            }
            if (isOpen && GetRandom(10) == 5) {
                SvTime start;
                time::SvTimeSet(&start, *time::GetCurrentDate());
                timeServiceTime = start;
                time::SvTimeNormalize(&timeServiceTime);
                Func5CE934(&timeServiceTime, 0, GetRandom(4) + 10, 0, 0);
                isStarted = true;
            }
        }
    }
    if (!isStarted) {
        time::SvTimeClear(&timeServiceTime);
    }
    PostNotices();
}

// 0x001E5BBC (name is ours)
void SvShopZakka::PostNotices()
{
    SvTime time = renovationDate;
    if (time::SvTimeIsValid(&time)) {
        nn::fnd::DateTimeParameters now;
        time::Func3004A8(&now);
        const nn::fnd::DateTimeParameters today = now;
        time = renovationDate;
        if (Func303850(&time, &today, 1, 1) == -1) {
            NoticeRenewal notice;
            SvBulletinBoardMessage message;
            nn::fnd::DateTimeParameters date;
            time::Func3004A8(&date);
            const nn::fnd::DateTimeParameters noticeDate = date;
            notice.Write(&message, NOTICE_FILE, nooklingState + 5, &noticeDate, NULL, NULL);
            GetGardenData()->townData.unk_0x1AE94.Func302120(&message);
        }
    }
    if (!IsRenovationDay() && time::SvTimeIsValid(&timeServiceTime) && Func75A6C8(&timeServiceTime, 1)) {
        NoticeTimeService notice;
        SvBulletinBoardMessage message;
        s32 label;
        if (nooklingStateUnknown != 0) {
            label = GetRandom(2) + nooklingStateUnknown * 4 - 4 + 13;
        } else {
            label = GetRandom(2) + 1;
        }
        nn::fnd::DateTimeParameters date;
        time::Func3004A8(&date);
        const nn::fnd::DateTimeParameters noticeDate = date;
        notice.Write(&message, NOTICE_FILE, label, &noticeDate, NULL, NULL);
        GetGardenData()->townData.unk_0x1AE94.Func302120(&message);
    }
}

// 0x00890718
const u8 SvShopZakka::s_NumItems[14][5] = {
    {2, 2, 1, 1, 1}, {0, 1, 2, 2, 2}, {0, 1, 2, 2, 2}, {0, 1, 2, 2, 2}, {1, 1, 2, 2, 2}, {0, 0, 1, 1, 1}, {0, 1, 1, 1, 1},
    {0, 0, 0, 1, 2}, {1, 2, 2, 2, 2}, {0, 0, 1, 1, 2}, {2, 2, 1, 1, 2}, {0, 0, 1, 2, 2}, {0, 1, 1, 1, 1}, {0, 0, 0, 1, 1},
};

// the copies of GetNumItems in the shared area (names are ours)
// 0x00711DA8
template u8 SvShopZakka::GetNumItems<0u>() const;
// 0x00711DC4
template u8 SvShopZakka::GetNumItems<1u>() const;
// 0x003038B4
template u8 SvShopZakka::GetNumItems<2u>() const;
// 0x003038D0
template u8 SvShopZakka::GetNumItems<3u>() const;
// 0x00711D8C
template u8 SvShopZakka::GetNumItems<4u>() const;
// 0x0071238C
template u8 SvShopZakka::GetNumItems<5u>() const;
// 0x00712230
template u8 SvShopZakka::GetNumItems<6u>() const;
// 0x00711D70
template u8 SvShopZakka::GetNumItems<7u>() const;
// 0x00303960
template u8 SvShopZakka::GetNumItems<8u>() const;
// 0x0030397C
template u8 SvShopZakka::GetNumItems<9u>() const;
// 0x00711CE8
template u8 SvShopZakka::GetNumItems<10u>() const;
// 0x00303998
template u8 SvShopZakka::GetNumItems<11u>() const;
// 0x003039B4
template u8 SvShopZakka::GetNumItems<12u>() const;
// 0x00712370
template u8 SvShopZakka::GetNumItems<13u>() const;

// 0x00711C14 (name is ours)
bool SvShopZakka::IsDayAfterRenovation() const
{
    SvTime time = renovationDate;
    if (!time::SvTimeIsValid(&time)) {
        return false;
    }
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    const nn::fnd::DateTimeParameters today = now;
    time = renovationDate;
    return Func303850(&time, &today, 1, 1) == 1;
}

// 0x00711C7C (name is ours)
bool SvShopZakka::IsDayBeforeRenovation() const
{
    SvTime time = renovationDate;
    if (!time::SvTimeIsValid(&time)) {
        return false;
    }
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    const nn::fnd::DateTimeParameters today = now;
    time = renovationDate;
    return Func303850(&time, &today, 1, 1) == -1;
}

// 0x00711D04 (name is ours)
DECOMP_NOINLINE bool SvShopZakka::IsRenovationDay() const
{
    SvTime time = renovationDate;
    if (!time::SvTimeIsValid(&time)) {
        return false;
    }
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    const nn::fnd::DateTimeParameters today = now;
    time = renovationDate;
    return Func303850(&time, &today, 1, 1) == 0;
}

// 0x00711DE0 (name is ours)
bool SvShopZakka::IsTimeService() const
{
    return time::SvTimeIsValid(&timeServiceTime) && Func75A6C8(&timeServiceTime, 1);
}

// 0x0071228C (name is ours)
SvFgName SvShopZakka::Func71228C() const
{
    for (u32 i = 0; i < 24; i++) {
        if (!unk_0x80[i]) {
            const SvFgName item = nooklingsItems[i];
            if (Func769264(&item) == 9) {
                return GetShopItem(this, i);
            }
        }
    }
    return SvFgName::s_Empty;
}

// 0x00712284 (name is ours)
u8 SvShopZakka::GetNumItems8() const
{
    return 1;
}

// 0x0071224C (name is ours)
u32 SvShopZakka::GetTimeServiceHour() const
{
    u32 hour = 0;
    if (time::SvTimeIsValid(&timeServiceTime)) {
        hour = time::SvTimeToParameters(&timeServiceTime).hour;
    }
    return hour;
}

// 0x00759E24 (name is ours)
bool SvShopZakka::IsNightOwlOrdinance() const
{
    return GetGardenPlus()->data.townData.townFlags.nightOwlOrdinance;
}

namespace {
// (falls through into ~NoticeLoadAdapt)
// 0x003028E4 slot 0x00
// 0x00525208 (deleting dtor)
NoticeRenewal::~NoticeRenewal()
{
    // (only the base is destroyed)
}

// 0x00525228 slot 0x00
// 0x00525218 (deleting dtor)
NoticeTimeService::~NoticeTimeService()
{
    // (only the base is destroyed)
}
} // namespace
