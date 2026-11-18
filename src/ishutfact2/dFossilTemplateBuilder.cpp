#include "ishutfact2/dFossilTemplateBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "Item/item_Api.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"
#include "script/dScriptExtern.h"
#include "sead/seadRandom.h"
#include "stage/stage_Api.h"
#include "Sv/dSvFgName.h"

namespace {
// the placeholders of the layout "FgDataTour/J0.bin" (names are ours): a fossil of the set at random, a fossil of two parts
// / three parts in one of four directions (the parts follow 2 units apart)
const u16 PLACEHOLDER_FOSSIL = 0x6004;
const u16 PLACEHOLDER_FOSSIL2_FIRST = 0x6005;
const u16 PLACEHOLDER_FOSSIL3_FIRST = 0x600D;

// the index of an even hut (0..3; 0 for the other huts; inline)
inline u32 GetFossilHutIndex(FieldName hut)
{
    if (!stage::IsEvenIslandHut(static_cast<stage::Name>(Func6F9FE4(hut)))) {
        return 0;
    }
    return (hut - FIELD_NAME_ISLAND_HUT_0) / 2;
}

// an item is added to the set
inline void AddToSet(SvFgNameSet* set, const SvFgName& item)
{
    Func302F80(set, &item);
}

// the next part of a fossil in the direction (0: north, 1: west, 2: south, 3: east)
inline void MoveToNextPart(s32* x, s32* z, u8 direction)
{
    if (direction == 0) {
        *z -= 2;
    } else if (direction == 1) {
        *x -= 2;
    } else if (direction == 2) {
        *z += 2;
    } else if (direction == 3) {
        *x += 2;
    }
}

} // namespace

// (inline in the instance of ForEachBlock)
DECOMP_ALWAYS_INLINE void ishutfact2::FossilItemsSetter::operator()(const Vector2i& block)
{
    if (m_pField == NULL) {
        return;
    }
    BgInfo* bg = m_pField->GetBg(block.x, block.y);
    if (!m_pData->IsLoaded()) {
        return;
    }
    // (the level of the tour is read and not used)
    Func76F7E4(&m_pTourData->m_Seed);
    const u32 num = m_pData->GetItems(m_Items, sizeof(m_Items), m_Key);
    // the small fossils for the placeholder
    SvFgNameSet fossils;
    fossils.Clear();
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_AMBER)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_AMMONITE)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_COPROLITE)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_DINOSAUR_EGG)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_FERN_FOSSIL)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_DINOSAUR_TRACK)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_ARCHAEOPTERYX)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_PEKING_MAN)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_SHARK_TOOTH)));
    AddToSet(&fossils, SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_TRILOBITE)));
    for (u32 i = 0; i < num; i++) {
        const FgDataItem& entry = m_Items[i];
        const u16 id = entry.id;
        if (id == PLACEHOLDER_FOSSIL) {
            const SvFgName fossil = Func1AD2E0(&fossils, m_pRandom->getU32());
            bg->SetFg(fossil, entry.x, entry.z, 0);
            RemoveFgName(&fossils, &fossil);
        }
        switch (id) {
        case PLACEHOLDER_FOSSIL2_FIRST:
        case PLACEHOLDER_FOSSIL2_FIRST + 1:
        case PLACEHOLDER_FOSSIL2_FIRST + 2:
        case PLACEHOLDER_FOSSIL2_FIRST + 3: {
            const u8 direction = static_cast<u8>(id - PLACEHOLDER_FOSSIL2_FIRST);
            u16 angle = Func6947A0(direction);
            angle += 0x4000;
            const u32 orientation = Func692070(&angle);
            const u16* parts = isfactory::s_Fossils2[m_pRandom->getU32(4)];
            SvFgName first;
            SvFgName second;
            Func311860(&first, SvFgName(static_cast<SvFgName::ID>(parts[0])), orientation);
            Func311860(&second, SvFgName(static_cast<SvFgName::ID>(parts[1])), orientation);
            s32 x = entry.x;
            s32 z = entry.z;
            bg->SetFg(first, x, z, 0);
            MoveToNextPart(&x, &z, direction);
            bg->SetFg(second, x, z, 0);
            break;
        }
        case PLACEHOLDER_FOSSIL3_FIRST:
        case PLACEHOLDER_FOSSIL3_FIRST + 1:
        case PLACEHOLDER_FOSSIL3_FIRST + 2:
        case PLACEHOLDER_FOSSIL3_FIRST + 3: {
            const u8 direction = static_cast<u8>(id - PLACEHOLDER_FOSSIL3_FIRST);
            u16 angle = Func6947A0(direction);
            angle += 0x4000;
            const u32 orientation = Func692070(&angle);
            const u16* parts = isfactory::s_Fossils3[m_pRandom->getU32(11)];
            SvFgName first;
            SvFgName second;
            SvFgName third;
            Func311860(&first, SvFgName(static_cast<SvFgName::ID>(parts[0])), orientation);
            Func311860(&second, SvFgName(static_cast<SvFgName::ID>(parts[1])), orientation);
            Func311860(&third, SvFgName(static_cast<SvFgName::ID>(parts[2])), orientation);
            s32 x = entry.x;
            s32 z = entry.z;
            bg->SetFg(first, x, z, 0);
            MoveToNextPart(&x, &z, direction);
            bg->SetFg(second, x, z, 0);
            MoveToNextPart(&x, &z, direction);
            bg->SetFg(third, x, z, 0);
            break;
        }
        default:
            break;
        }
    }
}

// 0x0080DE20
template ishutfact2::FossilItemsSetter ForEachBlock<ishutfact2::FossilItemsSetter>(const FieldRect& rect,
                                                                                  ishutfact2::FossilItemsSetter function);

namespace ishutfact2 {
// 0x001BAD78 slot 0x00
// 0x001BAD68 (deleting dtor)
ishutfact2::FossilTemplateBuilder::~FossilTemplateBuilder()
{
    // empty (in the original too)
}

// 0x001BAC8C slot 0x1C (name is ours)
void ishutfact2::FossilTemplateBuilder::Setup1C()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data = mgr->GetFgData("FgDataTour/J0.bin");
    if (data == NULL) {
        return;
    }
    const u32 index = GetFossilHutIndex(m_Hut);
    if (index >= HUT_NUM) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(),
                 FossilItemsSetter(m_pField, data, m_pRandom, m_pTourData, m_Keys[index]));
}

// 0x001BAAC0 slot 0x28 (name is ours)
void ishutfact2::FossilTemplateBuilder::SetupGrass()
{
    if (m_pField == NULL) {
        return;
    }
    if (!stage::IsEvenIslandHut(static_cast<stage::Name>(Func6F9FE4(m_Hut)))) {
        return;
    }
    // the fossils of the hut are remembered
    const u32 index = GetFossilHutIndex(m_Hut);
    if (index >= HUT_NUM) {
        return;
    }
    TourData::HutFossil* fossils = TourData::s_pInstance->m_HutFossils[index];
    for (u32 i = 0; i < TourData::HUT_FOSSIL_NUM; i++) {
        fossils[i].x = 0;
        fossils[i].z = 0;
        fossils[i].item = SvFgName::NAME_EMPTY;
        fossils[i].unk_0xA = 0;
    }
    const FieldRect& units = m_pField->GetUnitBounds();
    u32 num = 0;
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        const SvFgName* item = m_pField->GetFg((*it).x, (*it).y, 0);
        if (item == NULL || item->GetKind() != item::KIND_FOSSIL || num >= TourData::HUT_FOSSIL_NUM) {
            continue;
        }
        fossils[num].x = (*it).x;
        fossils[num].z = (*it).y;
        fossils[num].item = GetFgNameId(item);
        fossils[num].unk_0xA = Func76A388(item);
        num++;
    }
}

} // namespace ishutfact2
