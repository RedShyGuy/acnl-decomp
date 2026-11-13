#include "Sv/dSvMuseum.h"

#include "Item/item_Api.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvFgName.h"
#include "event/event_Extern.h"
#include "time/time_Api.h"

namespace {
// (inline) the place of an item of a donated kind in the arrays (-1 for other kinds)
DECOMP_ALWAYS_INLINE s32 GetFirstPlace(u8 kind)
{
    switch (kind) {
    case item::KIND_FOSSIL:
        return SvMuseum::FOSSIL_FIRST;
    case item::KIND_INSECT:
        return SvMuseum::INSECT_FIRST;
    case item::KIND_FISH:
        return SvMuseum::FISH_FIRST;
    case item::KIND_DIVE_FISH:
        return SvMuseum::DIVE_FISH_FIRST;
    case item::KIND_PICTURE:
        return SvMuseum::PICTURE_FIRST;
    case item::KIND_SCULPTURE:
        return SvMuseum::SCULPTURE_FIRST;
    }
    return -1;
}

// (inline) all items of a kind are donated
DECOMP_ALWAYS_INLINE bool IsKindComplete(const SvMuseum* museum, u8 kind, u32 count)
{
    for (u32 i = 0; i < count; i++) {
        const SvFgName item = SvFgName::MakeOfKind(kind, i);
        if (museum->GetDonor(item) == 0) {
            return false;
        }
    }
    return true;
}

// (inline) the donor of the items of a kind of a player is replaced
DECOMP_ALWAYS_INLINE void RemoveDonor(SvMuseum* museum, u8 kind, u32 count, u32 donor)
{
    for (u32 i = 0; i < count; i++) {
        const SvFgName item = SvFgName::MakeOfKind(kind, i);
        if (museum->GetDonor(item) == donor) {
            museum->SetFormerDonor(item);
        }
    }
}

// (inline) the item gets the donor and the date of today
DECOMP_ALWAYS_INLINE void SetDonation(SvMuseum* museum, u32 place)
{
    museum->donations[place] = SvGetPlayerSaveNo() + 1;
    SvDate date;
    event::Func30F6DC(&date, time::GetCurrentDate());
    museum->donationDates[place] = date;
}
} // namespace

// 0x006BCCBC (name is ours)
void SvMuseum::RemovePlayer(u32 saveNo)
{
    const u32 donor = (saveNo & 3) + 1;
    RemoveDonor(this, item::KIND_FOSSIL, 67, donor);
    RemoveDonor(this, item::KIND_FISH, 72, donor);
    RemoveDonor(this, item::KIND_DIVE_FISH, 30, donor);
    RemoveDonor(this, item::KIND_INSECT, 72, donor);
    RemoveDonor(this, item::KIND_PICTURE, 25, donor);
    RemoveDonor(this, item::KIND_SCULPTURE, 8, donor);
    for (u32 i = 0; i < 4; i++) {
        SvMuseumExhibit* exhibit = &exhibits[i];
        if (exhibit->playerIndex == static_cast<s32>(saveNo)) {
            Func1B63DC(exhibit);
        }
    }
}

// 0x006BCED8 (name is ours)
SvMuseumExhibit* SvMuseum::GetExhibit(u32 index)
{
    if (index >= 4) {
        return &exhibits[0];
    }
    return &exhibits[index];
}

// 0x006BCF04 (name is ours)
void SvMuseum::ClearDonations()
{
    for (s32 i = 0; i < 67; i++) {
        donationDates[FOSSIL_FIRST + i] = SvDate();
        donations[FOSSIL_FIRST + i] = 0;
    }
    for (s32 i = 0; i < 72; i++) {
        donationDates[FISH_FIRST + i] = SvDate();
        donations[FISH_FIRST + i] = 0;
    }
    for (s32 i = 0; i < 30; i++) {
        donationDates[DIVE_FISH_FIRST + i] = SvDate();
        donations[DIVE_FISH_FIRST + i] = 0;
    }
    for (s32 i = 0; i < 72; i++) {
        donationDates[INSECT_FIRST + i] = SvDate();
        donations[INSECT_FIRST + i] = 0;
    }
    for (s32 i = 0; i < 25; i++) {
        donationDates[PICTURE_FIRST + i] = SvDate();
        donations[PICTURE_FIRST + i] = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        donationDates[SCULPTURE_FIRST + i] = SvDate();
        donations[SCULPTURE_FIRST + i] = 0;
    }
}

// 0x006BD014 (name is ours)
void SvMuseum::ClearExhibits()
{
    for (s32 i = 0; i < 4; i++) {
        Func1B63DC(&exhibits[i]);
    }
}

// 0x006BD04C (name is ours)
void SvMuseum::SetFormerDonor(const SvFgName& item)
{
    const u8 kind = item.GetKind();
    const s32 index = item::GetIndex(item);
    if (!HasPlace(kind, index)) {
        return;
    }
    const s32 first = GetFirstPlace(kind);
    if (first >= 0) {
        donations[first + index] = DONOR_FORMER;
    }
}

// 0x006BD0F0 (name is ours)
void SvMuseum::ClearExhibits2()
{
    for (s32 i = 0; i < 4; i++) {
        Func1B63DC(&exhibits[i]);
    }
}

// 0x006BD128 (name is ours)
void SvMuseum::Init()
{
    ClearDonations();
    for (s32 i = 0; i < 4; i++) {
        Func1B6538(&exhibits[i]);
    }
}

// 0x006BD168 (name is ours)
void SvMuseum::Donate(const SvFgName& item)
{
    const u8 kind = item.GetKind();
    const s32 index = item::GetIndex(item);
    if (!HasPlace(kind, index)) {
        return;
    }
    switch (kind) {
    case item::KIND_FOSSIL:
        SetDonation(this, FOSSIL_FIRST + index);
        IsKindComplete(this, item::KIND_FOSSIL, 67);
        break;
    case item::KIND_INSECT:
        SetDonation(this, INSECT_FIRST + index);
        IsKindComplete(this, item::KIND_INSECT, 72);
        break;
    case item::KIND_FISH:
        SetDonation(this, FISH_FIRST + index);
        IsFishComplete();
        break;
    case item::KIND_DIVE_FISH:
        SetDonation(this, DIVE_FISH_FIRST + index);
        IsFishComplete();
        break;
    case item::KIND_PICTURE:
        SetDonation(this, PICTURE_FIRST + index);
        IsArtComplete();
        break;
    case item::KIND_SCULPTURE:
        SetDonation(this, SCULPTURE_FIRST + index);
        IsArtComplete();
        break;
    default:
        return;
    }
    // (the whole museum; what followed was removed)
    if (IsArtComplete() && IsFishComplete() && IsKindComplete(this, item::KIND_INSECT, 72)) {
        IsKindComplete(this, item::KIND_FOSSIL, 67);
    }
}

// 0x0076E684 (name is ours)
u8 SvMuseum::GetDonor(const SvFgName& item) const
{
    const u8 kind = item.GetKind();
    const s32 index = item::GetIndex(item);
    if (!HasPlace(kind, index)) {
        return 0;
    }
    const s32 first = GetFirstPlace(kind);
    if (first < 0) {
        return 0;
    }
    return donations[first + index];
}

// 0x0076E730 (name is ours)
bool SvMuseum::HasPlace(u8 kind, u32 index)
{
    switch (kind) {
    case item::KIND_FOSSIL:
        return index < 67;
    case item::KIND_PICTURE:
        return index < 25;
    case item::KIND_SCULPTURE:
        return index < 8;
    case item::KIND_INSECT:
        return index < 72;
    case item::KIND_FISH:
        return index < 72;
    case item::KIND_DIVE_FISH:
        return index < 30;
    }
    return false;
}

// 0x0076D8D4 (name is ours)
bool SvMuseum::IsFishComplete() const
{
    return IsKindComplete(this, item::KIND_FISH, 72) && IsKindComplete(this, item::KIND_DIVE_FISH, 30);
}

// 0x0076E150 (name is ours)
bool SvMuseum::IsArtComplete() const
{
    return IsKindComplete(this, item::KIND_PICTURE, 25) && IsKindComplete(this, item::KIND_SCULPTURE, 8);
}
