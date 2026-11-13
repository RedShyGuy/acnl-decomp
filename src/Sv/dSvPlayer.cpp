#include "Sv/dSvPlayer.h"

#include <cstring>

#include "Bs/dBsSvMgr.h"
#include "Item/item_Api.h"
#include "script/dPhrase.h"
#include "script/dScriptExtern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvShared.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvProcExtern.h"

extern const f32 FIELD_UNIT_SIZE_X;

// The globals of the file (names are ours). The static initializer computes g_Unknown95D2E8, builds s_MailItem and
// registers its destructor (GCC builds both constantly).
// 0x00788374 (static initializer)

namespace {
// the seconds of the play time below a full minute (AddPlaytime)
// 0x0095D2E0
u32 s_PlaytimeSeconds;
// the item of the letter that is written (name is ours)
// 0x0095D2E4
SvFgName s_MailItem;
} // namespace

// (FIELD_UNIT_SIZE_X / 5, through u16; nothing in code.bin reads it, the same value as g_Unknown95D36C of dSvProc.cpp)
// 0x0095D2E8
f32 g_Unknown95D2E8 = static_cast<u16>(FIELD_UNIT_SIZE_X * 0.2f);

namespace {

// the name of the item is the word 2C0/0 of the letter (name is ours)
// 0x006BE894
void SetMailItemWord(script::Phrase* phrase)
{
    phrase->SetWord2C0(s_MailItem.GetName(NULL), 0);
}
} // namespace

// 0x006BD504 (name is ours)
void SvPlayer::AddPlaytime(s64 time)
{
    const u64 sum = static_cast<u64>(playtime) + static_cast<u64>(time);
    if (sum <= 0x7FFFFFFFFFFFFFFFull) {
        playtime = sum;
    } else {
        playtime = 0x7FFFFFFFFFFFFFFFll;
    }
    Func6F0C80(16, static_cast<s32>(time / 60));
    s_PlaytimeSeconds += static_cast<u32>(time % 60);
    if (s_PlaytimeSeconds >= 60) {
        Func6F0C80(16, 1);
        s_PlaytimeSeconds -= 60;
    }
}

// 0x002FB9EC (name is ours)
// 0x00302030 (copy)
SvPlayer* SvPlayerData::Get(u32 index)
{
    if (index >= 4) {
        return NULL;
    }
    return &players[index];
}

// 0x006BD90C (name is ours)
void SvPlayer::Func6BD90C()
{
    debtAmount.Set(Func76E7CC(this));
    unk_0x8F77.data[12] |= 0x10;
}

// 0x006BD940 (name is ours)
void SvPlayer::Func6BD940()
{
    Func58381C(this);
}

// (falls through into Func6BD94C)
// 0x006BD944 (name is ours)
void Func6BD944(SvMovePlayerPls* data, const SvPlayer* player)
{
    Func6BD94C(&data->player, player);
}

// 0x006BD94C (name is ours)
void Func6BD94C(SvPlayer* dst, const SvPlayer* src)
{
    const s32 distance = reinterpret_cast<u8*>(dst) - reinterpret_cast<const u8*>(src);
    if (distance > 0 && static_cast<s32>(sizeof(SvPlayer)) > distance) {
        u8* to = reinterpret_cast<u8*>(dst);
        const u8* from = reinterpret_cast<const u8*>(src);
        for (u32 i = 0; i < sizeof(SvPlayer); i++) {
            to[i] = from[i];
        }
        return;
    }
    std::memmove(static_cast<void*>(dst), src, sizeof(SvPlayer));
}

// 0x006BD9A0 (name is ours)
void SvPlayer::Func6BD9A0(const SvFgName& item)
{
    if (item.GetKind() != 44) {
        return;
    }
    const s32 index = item::GetIndex(item);
    if (index >= 67) {
        return;
    }
    const u32 byte = index >> 3;
    if (byte < sizeof(unk_0x8F6E)) {
        unk_0x8F6E[byte] |= 1 << (index & 7);
    }
}

// 0x006BD9FC (name is ours)
void SvPlayer::Func6BD9FC(u32 value)
{
    bellsFromReeseAmount.Add(value, 0xFFFFFFFF);
}

// 0x006BDA0C (name is ours)
void SvPlayer::Func6BDA0C()
{
    std::memset(unk_0x8F6E, 0, sizeof(unk_0x8F6E));
}

// 0x006BDB48 (name is ours)
void SvPlayer::Func6BDB48()
{
    SvEncValue* coupons = &unk_0x8D1C.meowCoupons;
    unk_0x8D1C.Func2BF528();
    coupons->Add(Func24AC8C(&initiative), 99999);
}

// 0x006BDB84 (name is ours)
void SvPlayer::Func6BDB84(u32 value)
{
    debtAmount.Set(value);
    unk_0x8F77.data[12] |= 0x10;
}

// 0x006BDBB0 (name is ours)
SvPlayerUnkA468::SvPlayerUnkA468() : unk_0x0(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY))
{
    unk_0x4 = 0;
}

// 0x006BDBCC (name is ours)
void SvPlayer::Func6BDBCC(u32 index)
{
    if (index >= 0x18F) {
        return;
    }
    const u32 byte = index >> 3;
    if (byte < sizeof(unk_0x8F3C)) {
        unk_0x8F3C[byte] |= 1 << (index & 7);
    }
}

// 0x006BDA24 (name is ours)
s32 SvPlayer::Func6BDA24()
{
    s32 total = 0;
    if (!Func769644(Func71B394(&playerAppearance.playerOutfit))) {
        const s32 value = Func76A018(Func71B394(&playerAppearance.playerOutfit));
        if (value > 0) {
            bankAmount.Add(value, 999999999);
            total = value;
        }
    }
    playerAppearance.playerOutfit.heldItem = SvFgName::s_Empty;
    for (u32 i = 0; i < SvPlayerInventory::SIZE; i++) {
        const SvFgName* item = inventory.GetItem(i);
        if (!item->IsEmpty()) {
            const s32 value = Func76A018(item);
            if (value > 0) {
                bankAmount.Add(value, 999999999);
                total += value;
            }
        }
        inventory.SetItem(i, SvFgName::s_Empty, SvPlayerInventory::ITEM_APPEARANCE_DEFAULT, NULL);
    }
    return total;
}

// 0x006BDC0C (name is ours)
void SvPlayer::Func6BDC0C()
{
    SvMail* letters = GetSharedData()->storageBox.letterPool;
    for (s32 i = 0; i < 3; i++) {
        const s32 index = Func711124(letters);
        if (index > -1) {
            SvMail mail;
            SvFgName* item = &unk_0xA468[i].unk_0x0;
            if (item->IsUsual()) {
                s_MailItem = *item;
                const SvFgName paper(static_cast<SvFgName::ID>(SvFgName::NAME_CAMPGROUND_PAPER));
                if (mail.LoadPlayer("Mail_SP_Autocamp", 1, &playerInfo, &paper, 32, &SetMailItemWord)) {
                    mail.Func769154(*item, 0);
                    Func1E4904(letters, index, &mail);
                    *item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
                    unk_0xA468[i].unk_0x4 = 0;
                }
            }
        }
    }

    s32 to = 0;
    s32 from = 0;
    do {
        if (!unk_0xA468[to].unk_0x0.IsUsual()) {
            if (to != from && unk_0xA468[from].unk_0x0.IsUsual()) {
                unk_0xA468[to] = unk_0xA468[from];
                unk_0xA468[from].unk_0x0 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
                to++;
                unk_0xA468[from].unk_0x4 = 0;
            }
            from++;
            if (from >= 3) {
                return;
            }
        } else {
            to++;
        }
    } while (to < 3);
}

inline u16 SvPlayer::GetEncyclopediaSize(item::Kind kind, u32 index) const
{
    switch (kind) {
    case item::KIND_INSECT:
        if (index < 72) {
            return encyclopediaSizes.insects[index];
        }
        break;
    case item::KIND_FISH:
        if (index < 72) {
            return encyclopediaSizes.fish[index];
        }
        break;
    case item::KIND_DIVE_FISH:
        if (index < 30) {
            return encyclopediaSizes.seaCreatures[index];
        }
        break;
    default:
        break;
    }
    return 0;
}

// 0x006BDE18 (name is ours)
u16 SvPlayer::Func6BDE18(const SvFgName& item) const
{
    switch (item.GetKind()) {
    case item::KIND_INSECT:
        return GetEncyclopediaSize(item::KIND_INSECT, Func70162C(item));
    case item::KIND_FISH:
    case item::KIND_DIVE_FISH: {
        s32 index = 0;
        item::Kind kind = item::KIND_FISH;
        Func617740(&index, &kind, Func61643C(item));
        return GetEncyclopediaSize(kind, index);
    }
    default:
        return 0;
    }
}

// 0x006BDF08 (name is ours)
void SvPlayer::Func6BDF08(s32 value)
{
    if (value > 0) {
        const s32 sum = value + unk_0x8F28;
        unk_0x8F28 = sum >= 0xFFFF ? 0xFFFF : sum;
    }
}

// 0x006BDF34 (name is ours)
void SvPlayer::Func6BDF34(const SvFgName& item, u16 value)
{
    for (s32 i = 0; i < 3; i++) {
        if (!unk_0xA468[i].unk_0x0.IsUsual()) {
            unk_0xA468[i].unk_0x0 = item;
            unk_0xA468[i].unk_0x4 = value;
            return;
        }
    }
}

// 0x006BDFA0 (name is ours)
bool SvPlayer::Func6BDFA0()
{
    for (s32 i = 0; i < 3; i++) {
        if (!unk_0xA468[i].unk_0x0.IsUsual()) {
            unk_0xA468[i].unk_0x0 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
            unk_0xA468[i].unk_0x4 = 0;
            return true;
        }
    }
    return false;
}

// 0x006BE028 (name is ours)
void SvPlayer::Func6BE028(u32 index)
{
    if (index != 0xFFFF && index < 512) {
        unk_0xA3E4[index >> 3] |= 1 << (index & 7);
    }
}

// 0x006BE05C (name is ours)
void SvPlayer::Func6BE05C(u32 index)
{
    if (index != 0xFFFF && index < 512) {
        unk_0xA424[index >> 3] |= 1 << (index & 7);
    }
}

// 0x006BE090 (name is ours)
void SvPlayer::Func6BE090()
{
    std::memset(unk_0x8F3C, 0, sizeof(unk_0x8F3C));
}

// 0x006BE0A0 (name is ours)
SvPlayerUnk8D84* SvPlayer::Func6BE0A0(const SvNpcRef* npc, s32 index)
{
    if (Func76EE94(this, npc, index)) {
        return &unk_0x8D84;
    }
    return NULL;
}

// 0x006BE0BC (name is ours)
void SvPlayer::Func6BE0BC(const SvFgName& item)
{
    const u16 size = Func76981C(item);
    const item::Kind itemKind = item.GetKind();
    if (itemKind == item::KIND_INSECT) {
        const u32 index = Func70162C(item);
        if (index < 72 && encyclopediaSizes.insects[index] < size) {
            encyclopediaSizes.insects[index] = size;
        }
    } else if (itemKind == item::KIND_FISH || itemKind == item::KIND_DIVE_FISH) {
        s32 index = 0;
        item::Kind kind = item::KIND_FISH;
        Func617740(&index, &kind, Func61643C(item));
        if (GetEncyclopediaSize(kind, index) < size) {
            if (itemKind == item::KIND_FISH) {
                if (static_cast<u32>(index) < 72) {
                    encyclopediaSizes.fish[index] = size;
                }
            } else if (itemKind == item::KIND_DIVE_FISH) {
                if (static_cast<u32>(index) < 30) {
                    encyclopediaSizes.seaCreatures[index] = size;
                }
            }
        }
    }
}

// 0x006BE210 (name is ours)
void SvPlayer::Func6BE210()
{
    s32* value = &unk_0xA464;
    const s32 old = *value;
    if (old < 0) {
        *value = GetRandom(5);
        return;
    }
    s32 random = GetRandom(4);
    *value = random;
    if (old <= random) {
        *value = random + 1;
    }
}

// 0x006BE254 (name is ours)
void SvPlayer::Func6BE254()
{
    std::memset(encyclopediaSizes.insects, 0, sizeof(encyclopediaSizes.insects));
    std::memset(encyclopediaSizes.fish, 0, sizeof(encyclopediaSizes.fish));
    std::memset(encyclopediaSizes.seaCreatures, 0, sizeof(encyclopediaSizes.seaCreatures));
}

// 0x006BE2C4 (name is ours)
void SvPlayer::Func6BE2C4(u32 a, u32 b, u32 c, const SvTownId* town, u32 townSlot)
{
    Func6BE460(true);
    Func20C8AC(this, a, b, c, town, townSlot);
    if (town != NULL) {
        townData2 = *town;
    }
    Func24E2B4(&emotes);
    Func6F0B74(hhaUnk3);
    inventory.Clear();
    unlockedItems.Clear();
    pocketMoney.Set(0);
    Func24CE14(&unk_0x8A3C);
    Func24AD90(&initiative);
    for (s32 i = 0; i < 3; i++) {
        unk_0xA468[i].unk_0x0 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
        unk_0xA468[i].unk_0x4 = 0;
    }
    unk_0xA47A.item = SvFgName::s_Empty;
    unk_0x8D1C.meowCoupons.Add(10, 99999);
    GetCensus()->AddMeowCouponsEarnedForSlot(townSlot, 10);
}

// 0x006BE460 (name is ours)
void SvPlayer::Func6BE460(bool keepUnk8F2C)
{
    const u16 saved = unk_0x8F2C;
    std::memset(static_cast<void*>(this), 0, sizeof(SvPlayer));
    Func20CABC(this);
    Func6F0B80(hhaUnk3);
    inventory.Clear();
    pocketMoney.Set(0);
    if (g_pSvCurrentPlayer != NULL && &g_pSvCurrentPlayer->inventory == &inventory) {
        Func5C7230();
    }
    bankAmount.Set(0);
    debtAmount.Set(0);
    medalAmount.Set(0);
    unk_0x8D1C.Clear();
    bellsFromReeseAmount.Set(0);
    playtime = 0;
    for (SvFgName* item = islandBox; item < islandBox + 0x28; item++) {
        *item = SvFgName::s_Empty;
    }
    u8* lock = islandInventoryItemLocks;
    for (SvFgName* item = islandInventory; item < islandInventory + 0x10; item++) {
        *item = SvFgName::s_Empty;
        *lock++ = 0;
    }
    unk_0x7000 = SvFgName::s_Empty;
    unk_0x7004 = SvFgName::s_Empty;
    for (u32 i = 0; i < 0xA; i++) {
        letters[i].Clear();
    }
    unk_0x8908.Clear();
    Func24E2D0(&emotes);
    for (s32 i = 0; i < 0x20; i++) {
        spotpassDlcReceivedIds.ids[i] = 0xFFFF;
    }
    Func24CE90(&unk_0x8A3C);
    unk_0x8F0A = SvFgName::s_Empty;
    unk_0x8F0E = SvFgName::s_Empty;
    Func305A94(unk_0x8F12);
    unk_0x8F28 = 0;
    Func5BDB2C(&unk_0x8D4C);
    Func5BDD70(&unk_0x8D5C);
    Func5BE334(&unk_0x8D6C);
    Func5BEBCC(&unk_0x8D84);
    Func30A834(&unk_0x8DFA);
    Func5BD6CC(&unk_0x8E08, this);
    filledWithFF.Func5BF34C();
    Func20C3D8(&unk_0x8EE0);
    unk_0x8F2A = 2011;
    unk_0x8F2C = keepUnk8F2C ? saved : 2011;
    std::memset(unk_0x8F3C, 0, sizeof(unk_0x8F3C));
    std::memset(unk_0x8F6E, 0, sizeof(unk_0x8F6E));
    unk_0x8F34 = 0;
    unk_0x8F38 = 0;
    unk_0x8F77.Init();
    addedSongs[0] = 0;
    addedSongs[1] = 0;
    addedSongs[2] = 0;
    Func6CD8A4(dressers);
    std::memset(bDayWish, 0, sizeof(bDayWish));
    std::memset(encyclopediaSizes.insects, 0, sizeof(encyclopediaSizes.insects));
    std::memset(encyclopediaSizes.fish, 0, sizeof(encyclopediaSizes.fish));
    std::memset(encyclopediaSizes.seaCreatures, 0, sizeof(encyclopediaSizes.seaCreatures));
    Func1B5184(santaBagInv);
    std::memset(unk_0xA3E4, 0, sizeof(unk_0xA3E4));
    std::memset(unk_0xA424, 0, sizeof(unk_0xA424));
    unk_0xA464 = -1;
    Func24AE04(&initiative);
    for (s32 i = 0; i < 3; i++) {
        unk_0xA468[i].unk_0x0 = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));
        unk_0xA468[i].unk_0x4 = 0;
    }
    unk_0xA47A.item = SvFgName::s_Empty;
}

// 0x006BE8E8 (name is ours)
void SvPlayer::Func6BE8E8(u32 value)
{
    SvEncValue* debt = &debtAmount;
    if (debt->Get() < value) {
        debt->Set(0);
    } else {
        debt->Sub(value);
    }
}

// 0x006BE928 (name is ours)
void SvPlayer::Func6BE928(u32 value)
{
    medalAmount.Add(value, 99999);
}

// 0x006BE93C (name is ours)
void SvPlayer::Func6BE93C()
{
    playerFlags.hasBeeSting = 0;
    if (playerFlags.unk_0x20_3) {
        if (playerFlags.unk_0x18_3) {
            if (!Func5C304C(this)) {
                goto skip;
            }
            playerFlags.unk_0x18_3 = 0;
        }
        playerFlags.unk_0x20_3 = 0;
    }
skip:
    if (playerFlags.unk_0x26_7 && Func5C6AF4(this)) {
        playerFlags.unk_0x26_7 = 0;
    }
}

// 0x006BE9E8 (name is ours)
void SvPlayer::Func6BE9E8(u32 value)
{
    medalAmount.Sub(value);
}

// 0x006BE9F4 (name is ours)
void SvPlayer::Func6BE9F4(u32 a, u32 b, u8 c)
{
    Func20CBD8(this, a, b, c);
    Func3238AC(&inventory, b);
    Func6CD8D0(dressers, b);
    Func2D5004(islandBox, b);
    Func6CDB2C(Func1DF90C(GetSharedData(), c), b);
    Func332E88(&unk_0x9604, a, &playerInfo, b);
    for (u32 i = 0; i < 0xA; i++) {
        if (letters[i].letterType & 0x80) {
            letters[i].Clear();
        } else {
            Func6BC774(&letters[i], a, &playerInfo, b);
        }
    }
    Func24DD34(&unk_0x8A3C);
    const u8 bit = (unk_0x8F77.data[12] >> 4) & 1;
    unk_0x8F77.Init();
    if (bit) {
        unk_0x8F77.data[12] |= 0x10;
    } else {
        unk_0x8F77.data[12] &= ~0x10;
    }
    unk_0x8F28 = 0;
    unk_0x8F2A = 2011;
    Func5BDB2C(&unk_0x8D4C);
    Func5BDD70(&unk_0x8D5C);
    Func5BE334(&unk_0x8D6C);
    Func5BEBCC(&unk_0x8D84);
    Func30A834(&unk_0x8DFA);
    Func5BD8D4(&unk_0x8E08, this);
    filledWithFF.Func5BF34C();
    std::memset(unk_0x8F3C, 0, sizeof(unk_0x8F3C));
    Func6F0B80(hhaUnk3);
    Func6F0B74(hhaUnk3);
    unk_0x8F2C = 2011;
}

// (the base, the members and their default values are built in order; the parts without a constructor stay as they
// are)
// 0x006BEBB0 (name is ours)
SvPlayer::SvPlayer()
{
    // (only the base and the members are constructed)
}
