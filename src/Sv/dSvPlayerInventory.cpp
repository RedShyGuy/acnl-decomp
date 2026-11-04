#include "Sv/dSvPlayerInventory.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCensus.h"
#include "Item/item_Api.h"

// the object whose player SetItem compares (class unknown, an object of another file; +0x234 is the current player)
struct UnkAAFF18
{
    u8 unk_0x0[0x234];
    SvPlayer* pPlayer;  // 0x234
};

// 0x00AAFF18
UnkAAFF18 g_UnkAAFF18;

// 0x007250DC | libgarden [tier A]
const SvFgName* SvPlayerInventory::GetItem(unsigned int index, ItemAppearance* pAppearance) const
{
    ItemAppearance appearance = ITEM_APPEARANCE_DEFAULT;
    if (pAppearance == NULL) {
        pAppearance = &appearance;
    }
    if (index < SIZE) {
        const SvFgName* item = &items[index];
        *pAppearance = static_cast<ItemAppearance>(appearances[index]);
        if (item->IsEmpty() || item->CanBeInInventory()) {
            return item;
        }
    }
    return &SvFgName::s_Empty;
}

// 0x003233A4 | libgarden [tier A]
bool SvPlayerInventory::AddItem(const SvFgName& item, ItemAppearance appearance, SvFgName* pNewItem)
{
    s32 slot = -1;
    for (s32 i = 0; i < static_cast<s32>(SIZE); i++) {
        if (GetItem(i)->IsEmpty()) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        return false;
    }
    return SetItem(slot, item, appearance, pNewItem);
}

// 0x00323444 (name is ours)
void SvPlayerInventory::Clear()
{
    u8* appearance = appearances;
    for (SvFgName* item = items; item < items + SIZE; item++) {
        *item = SvFgName::s_Empty;
        *appearance++ = ITEM_APPEARANCE_DEFAULT;
    }
}

// 0x00323484 | libgarden [tier A]
DECOMP_NOINLINE bool SvPlayerInventory::SetItem(unsigned int index, const SvFgName& item, ItemAppearance appearance, SvFgName* pNewItem)
{
    if (index >= SIZE) {
        return false;
    }
    SvFgName newItem(item);
    if (item::IsMoney(newItem)) {
        // money picked up into the pockets of the current player counts as earned bells
        SvPlayer* player = g_UnkAAFF18.pPlayer;
        if (player != NULL && &player->inventory == this && newItem.GetUnkFlag0()) {
            s32 price = item::GetPrice(newItem);
            GetCensus()->bellsEarned.Add(price);
        }
        newItem.ClearUnkFlag0();
    }
    if (!newItem.PrepareForInventory()) {
        return false;
    }
    const item::KindParamEntry* entry = item::GetKindParamEntry(newItem);
    bool isMail = entry != NULL && (entry->flags2 >> 7) != 0;
    items[index] = newItem;
    appearances[index] = isMail ? ITEM_APPEARANCE_MAIL : appearance;
    if (pNewItem != NULL) {
        *pNewItem = newItem;
    }
    return true;
}
