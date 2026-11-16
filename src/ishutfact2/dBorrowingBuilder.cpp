#include "ishutfact2/dBorrowingBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "event/event_Extern.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "Other/dTourData.h"
#include "Sv/dSvFgBlock.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "Sound/sound_Extern.h"

namespace {
// the items of the layout of the key of the hut (level 2), each one remembered for the hut
struct BorrowingItemsSetter
{
    BorrowingItemsSetter(Field* field, const FgDataLoader* data, u32 key, u32 hut)
        : m_pField(field), m_pData(data), m_Key(key), m_Hut(hut)
    {
    }

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (!m_pData->IsLoaded()) {
            return;
        }
        const u32 num = m_pData->GetItems(m_Items, sizeof(m_Items), m_Key);
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            SvFgName item(static_cast<SvFgName::ID>(entry.id));
            if (Func30B658(&item)) {
                Func311860(&item, SvFgName(static_cast<SvFgName::ID>(entry.id)), entry.flags >> 14);
            }
            bg->SetFg(item, entry.x, entry.z, 0);
            Func6C59BC(&TourData::s_pInstance->m_Seed, &item, m_Hut);
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
    u32 m_Key;
    u32 m_Hut;
};
} // namespace

namespace ishutfact2 {
// 0x001BAABC slot 0x00
// 0x001BAAAC (deleting dtor)
ishutfact2::BorrowingBuilder::~BorrowingBuilder()
{
    // empty (in the original too)
}

// 0x001BA62C slot 0x1C (name is ours)
void ishutfact2::BorrowingBuilder::Setup1C()
{
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    const u32 hut = m_Hut - FIELD_NAME_ISLAND_HUT_0;
    if (level == 2) {
        // the items of a layout
        BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
        if (mgr == NULL) {
            return;
        }
        FgDataLoader* data = mgr->GetFgData("FgDataTour/L0.bin");
        if (data == NULL) {
            return;
        }
        ForEachBlock(m_pField->GetBlockBounds(), BorrowingItemsSetter(m_pField, data, m_Entries[hut], hut));
        return;
    }
    // the room of the villager of the hut
    const u16 villager = static_cast<u16>(m_Entries[hut]);
    const UnknownA8449CEntry* villagers = g_UnknownA8449C.entries;
    if (villagers == NULL || villager >= 399) {
        return;
    }
    const UnknownA8449CEntry* entry = &villagers[villager];
    if (entry == NULL) {
        return;
    }
    FgDataItem items[12];
    Func318FDC(items, sizeof(items), entry->roomLayout);
    const bool isLevel0 = level == 0;
    SvFgBlock layer0;
    SvFgBlock layer1;
    layer0.Clear();
    layer1.Clear();
    Func112300(items, &layer0, &layer1, m_pRandom, villager, hut, isLevel0);
    const FieldRect& units = BgInfo::GetLocalUnitRect();
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        m_pField->PlaceItem(*layer0.GetFgChecked((*it).x, (*it).y), (*it).x, (*it).y, 0);
        m_pField->PlaceItem(*layer1.GetFgChecked((*it).x, (*it).y), (*it).x, (*it).y, 1);
    }
}

} // namespace ishutfact2
