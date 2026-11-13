// The Thumb part of the original dSvShopZakka.cpp: clearing and restocking the shop of the Nooklings, the seasonal
// items, the writers of the notices and the destructors of NoticeEyecatcher. All names but the class names are ours.

#include "Sv/dSvTown.h"

#include <cstring>

#include "Other/dRegion.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "dSvShopZakkaNotice.h"
#include "netgame/dGameFunctions.h"
#include "script/dMailPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"
#include "time/time_Extern.h"

namespace {
// (inline) the places that are not empty are removed from the set
DECOMP_ALWAYS_INLINE void RemoveShopItems(SvShopZakka* shop, SvFgNameSet* set)
{
    for (s32 i = 0; i < 24; i++) {
        SvFgName* item = &shop->nooklingsItems[i];
        if (!item->IsEmpty()) {
            RemoveFgName(set, item);
        }
    }
}

// (inline) a seasonal floor item in the place 6 or a wallpaper in the place 4 (when a group of the places 4 or 6 is sold
// at the level)
DECOMP_ALWAYS_INLINE void SetSeasonalItem(SvShopZakka* shop, SvFgName::Name item6, SvFgName::Name item4)
{
    if (!shop->GetNumItems<2>() && !shop->GetNumItems<3>()) {
        return;
    }
    if (GetRandom(2) != 0) {
        shop->nooklingsItems[6] = SvFgName(static_cast<SvFgName::ID>(item6));
        return;
    }
    shop->nooklingsItems[4] = SvFgName(static_cast<SvFgName::ID>(item4));
}

// (inline) two places are swapped
DECOMP_ALWAYS_INLINE void SwapItems(SvFgName* a, SvFgName* b)
{
    const SvFgName item(*a);
    *a = *b;
    *b = item;
}
} // namespace

// 0x00108C40 (Thumb) (name is ours)
void SvShopZakka::Restock()
{
    for (s32 i = 0; i < 24; i++) {
        nooklingsItems[i] = SvFgName::s_Empty;
        unk_0x80[i] = 0;
    }
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    const nn::fnd::DateTimeParameters today = now;
    SvTime time = renovationDate;
    if (time::SvTimeIsValid(&time)) {
        time = renovationDate;
        if (Func303850(&time, &today, 1, 1) == 0) {
            return;
        }
    }
    SvFgName* items = nooklingsItems;
    if (nooklingState != 0) {
        SvFgNameSet set;
        SetupFgNameSet(&set, item::GROUP_NET_ROD_SHOVEL);
        Func30031C(&items[0], GetNumItems<0>(), &set);
    } else {
        if (Func2FDE58(5)) {
            items[0] = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_FISHING_ROD));
        } else {
            SvFgNameSet set;
            SetupFgNameSet(&set, item::GROUP_NET_ROD_SHOVEL);
            const SvFgName item(static_cast<SvFgName::ID>(SvFgName::NAME_SHOVEL));
            RemoveFgName(&set, &item);
            Func30031C(&items[0], 1, &set);
        }
        items[1] = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_SHOVEL));
    }
    {
        SvFgNameSet set;
        SetupFgNameSet(&set, item::GROUP_SLINGSHOT_MEGAPHONE_TIMER);
        Func30031C(&items[2], GetNumItems<1>(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::Wallpaper, item::GROUP_SPECIAL_1, 0);
        Func30031C(&items[4], GetNumItems<2>(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::Carpets, item::GROUP_SPECIAL_1, 0);
        Func30031C(&items[6], GetNumItems<3>(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::WrappingPaper, item::GROUP_WRAPPING_PAPER, 0);
        Func30031C(&items[8], GetNumItems8(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::MailPapers, item::GROUP_STATIONERY, 0);
        Func30031C(&items[9], GetNumItems<4>(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::MelodyPaper, item::GROUP_MELODY_PAPER, 0);
        Func30031C(&items[11], GetNumItems<5>(), &set);
    }
    {
        SvFgNameSet set;
        Func2FECF8(&set, SvFgCategory::Medicine, item::GROUP_MEDICINE, 0);
        Func30031C(&items[12], GetNumItems<6>(), &set);
    }
    {
        SvFgNameSet set;
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_6);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_7);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_8);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_9);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_10);
        // the K.K. songs (ten groups) are only sold when a player has them
        for (u32 id = SvFgName::NAME_KK_CHORALE; id <= SvFgName::NAME_KK_BIRTHDAY; id++) {
            const SvFgName item(static_cast<SvFgName::ID>(id));
            bool isUnlocked = false;
            for (u32 i = 0; i < 4; i++) {
                SvPlayer* player = netgame::Func3038EC(static_cast<u8>(i));
                if (player != NULL && player->playerInfo.IsValid() && player->unlockedItems.Func7711D4(item)) {
                    isUnlocked = true;
                }
            }
            if (!isUnlocked) {
                RemoveFgName(&set, &item);
            }
        }
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_1);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_2);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_3);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_4);
        SetupFgNameSet(&set, item::GROUP_KK_SONGS_5);
        Func30031C(&items[13], GetNumItems<7>(), &set);
    }
    {
        SvFgNameSet set;
        // the items of the season: December (New Year's countdown), February 1..3 in the region 0 (Setsubun), October
        // (Halloween), August (fireworks); else the fortune cookie
        if (today.month == 12) {
            SetupFgNameSet(&set, item::GROUP_PARTY_POPPER_AND_GLOW_WANDS);
        }
        if (today.month == 2 && today.day >= 1 && today.day <= 3 && GetRegionIndex() == 0) {
            SetupFgNameSet(&set, item::GROUP_BEANS_AND_GOOD_LUCK_ROLL);
        }
        if (today.month == 10) {
            SetupFgNameSet(&set, item::GROUP_CANDY);
        }
        if (today.month == 8) {
            SetupFgNameSet(&set, item::GROUP_FIREWORKS);
        }
        if (Func302FC0(&set) == 0 || GetNumItems<8>() >= 2) {
            SetupFgNameSet(&set, item::GROUP_FORTUNE_COOKIE);
        }
        Func30031C(&items[15], GetNumItems<8>(), &set);
    }
    SvFgName* place17 = &items[17];
    {
        SvFgNameSet set;
        Func2FC32C(&set);
        Func30031C(place17, GetNumItems<9>(), &set);
    }
    {
        SvFgNameSet set;
        Func304A5C(&set, 2);
        if (GetRegionIndex() == 0) {
            GetRandom(100);
        }
        RemoveShopItems(this, &set);
        Func30031C(&items[19], GetNumItems<10>(), &set);
    }
    SvFgName* place21 = &items[21];
    {
        SvFgNameSet set;
        Func304A5C(&set, 1);
        if (GetRegionIndex() == 0) {
            GetRandom(100);
        }
        RemoveShopItems(this, &set);
        Func30031C(place21, GetNumItems<11>(), &set);
    }
    if (GetNumItems<12>() != 0) {
        SvFgName* place23 = &items[23];
        {
            SvFgNameSet set;
            Func304A5C(&set, 1);
            RemoveShopItems(this, &set);
            Func30031C(place23, GetNumItems<12>(), &set);
        }
        // the furniture of the season (Jingle until December 23, spooky in October, harvest in November, hinaningyo and
        // blossom lantern on March 1..3 and the carp banner on May 1..5 in the region 0), else any furniture
        SvFgNameSet set;
        if (today.month == 12 && today.day >= 1 && today.day <= 23) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_JINGLE_SERIES, 0);
            if (GetRegionIndex() != 1) {
                const SvFgName itemA(static_cast<SvFgName::ID>(SvFgName::NAME_FESTIVE_FLAG));
                RemoveFgName(&set, &itemA);
                const SvFgName itemB(static_cast<SvFgName::ID>(SvFgName::NAME_FESTIVE_CANDLE));
                RemoveFgName(&set, &itemB);
            }
        }
        if (today.month == 10) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_SPOOKY_SERIES, 0);
        }
        if (today.month == 11 && Func30141C(5, today, 0, 30, 1)) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_HARVEST_SERIES, 0);
        }
        if (today.month == 3 && today.day >= 1 && today.day <= 3 && GetRegionIndex() == 0) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_HINANINGYO_AND_BLOSSOM_LANTERN, 0);
        }
        if (today.month == 5 && today.day >= 1 && today.day <= 5 && GetRegionIndex() == 0) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_CARP_BANNER, 0);
        }
        if (Func302FC0(&set) < GetNumItems<12>()) {
            Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_SPECIAL_1, 0);
            RemoveShopItems(this, &set);
        }
        SvFgName item(SvFgName::s_Empty);
        Func30031C(&item, GetNumItems<12>(), &set);
        if (!item.IsEmpty()) {
            if (Func303BD4(&item)) {
                *place17 = item;
            } else {
                *place23 = item;
            }
        }
    }
    const u16 value = reinterpret_cast<const u16*>(Func303C0C())[1];
    if (GetNumItems<13>() != 0 && !GetGardenPlus()->data.townData.townFlags.unk_0x0_0 && !IsDayAfterRenovation() &&
        !IsTimeService() && static_cast<s32>(GetRandom(100)) < static_cast<s32>(Func77A6D4(value) >> 32) + 20) {
        SvFgNameSet set;
        SvFgName item;
        // the featured item: a furniture of the shop group 9 (covered wagon, moon, garden gnome, ...)
        Func2FECF8(&set, SvFgCategory::Furniture, item::GROUP_FEATURED_FURNITURE, 0);
        Func30031C(&item, 1, &set);
        if (Func303BD4(&item)) {
            if (GetNumItems<9>()) {
                *place17 = item;
            }
        } else if (GetNumItems<11>()) {
            *place21 = item;
        }
        NoticeEyecatcher notice;
        SvBulletinBoardMessage message;
        s32 label;
        if (nooklingStateUnknown != 0) {
            label = GetRandom(2) + nooklingStateUnknown * 4 - 4 + 15;
        } else {
            label = GetRandom(2) + 3;
        }
        const char* file = "BBS_Kodanuki";
        const SvShopZakka* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka;
        notice.Begin();
        const script::WordFix<33> name = shop->Func71228C().GetName(NULL);
        notice.GetPhrase()->SetWord2C0(name, 0);
        notice.Load(&message, file, label, &today, NULL, NULL);
        notice.End();
        notice.IsDone();
        GetGardenData()->townData.unk_0x1AE94.Func302120(&message);
    }
    for (s32 i = 19; i < 23; i++) {
        SvFgName* a = &items[i];
        if (a->IsEmpty()) {
            continue;
        }
        for (s32 j = i + 1; j < 24; j++) {
            SvFgName* b = &items[j];
            if (b->IsEmpty()) {
                continue;
            }
            if (Func302EDC(a) < Func302EDC(b)) {
                SwapItems(a, b);
            } else if (Func302EDC(a) == Func302EDC(b) && !Func303C2C(a) && Func303C2C(b)) {
                SwapItems(a, b);
            }
        }
    }
}

// 0x001094FC (Thumb) (name is ours)
void SvShopZakka::SetHarvestItems()
{
    SetSeasonalItem(this, SvFgName::NAME_HARVEST_RUG, SvFgName::NAME_HARVEST_WALL);
}

// 0x00109558 (Thumb) (name is ours)
void SvShopZakka::SetJingleItems()
{
    SetSeasonalItem(this, SvFgName::NAME_JINGLE_CARPET, SvFgName::NAME_JINGLE_WALL);
}

// 0x001095B4 (Thumb) (name is ours)
void SvShopZakka::SetSpookyItems()
{
    SetSeasonalItem(this, SvFgName::NAME_SPOOKY_CARPET, SvFgName::NAME_SPOOKY_WALL);
}

// 0x00109610 (Thumb) (name is ours)
void SvShopZakka::Clear()
{
    std::memset(static_cast<void*>(this), 0, sizeof(*this));
    nooklingStateUnknown = 0;
    nooklingState = 0;
    unk_0x10 = 10;
    time::SvTimeClear(&renovationDate);
    time::SvTimeClear(&timeServiceTime);
    nooklingBellsSpent.Set(0);
    unk_0x98 = 15;
}

namespace {
// 0x0010BEB4 (Thumb) (name is ours)
DECOMP_EMIT_COPY bool NoticeRenewal::Write(SvBulletinBoardMessage* message, const char* file, s32 label,
                                           const nn::fnd::DateTimeParameters* date, const script::IWord* name,
                                           const script::IWord* town)
{
    const SvShopZakka* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka;
    Begin();
    SvTime time = shop->renovationDate;
    const s8 month = time::SvTimeToParameters(&time).month;
    GetPhrase()->SetMonth(month, 0);
    time = shop->renovationDate;
    const s8 day = time::SvTimeToParameters(&time).day;
    GetPhrase()->SetDay(day, 0);
    Load(message, file, label, date, name, town);
    End();
    return IsDone();
}

// 0x0010BF30 (Thumb) (deleting dtor)
// 0x0010BF3C (Thumb) slot 0x00
DECOMP_EMIT_COPY NoticeEyecatcher::~NoticeEyecatcher()
{
    // (only the base is destroyed)
}

// 0x0010BF44 (Thumb) (name is ours)
DECOMP_EMIT_COPY bool NoticeTimeService::Write(SvBulletinBoardMessage* message, const char* file, s32 label,
                                               const nn::fnd::DateTimeParameters* date, const script::IWord* name,
                                               const script::IWord* town)
{
    const SvShopZakka* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka;
    Begin();
    const u32 hour = shop->GetTimeServiceHour();
    GetPhrase()->SetTag4_16(hour, 0);
    Load(message, file, label, date, name, town);
    End();
    return IsDone();
}
} // namespace
