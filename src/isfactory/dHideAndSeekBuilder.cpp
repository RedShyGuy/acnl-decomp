#include "isfactory/dHideAndSeekBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"
#include "sead/seadObjArray.h"
#include "sead/seadRandom.h"

namespace isfactory {
// 0x007059E8 slot 0x00
// 0x007059D8 (deleting dtor)
isfactory::HideAndSeekBuilder::~HideAndSeekBuilder()
{
    // empty (in the original too)
}

// 0x00705400 slot 0x18 (name is ours)
void isfactory::HideAndSeekBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    // (the level is read and not used; the layout is not checked)
    Func76F7E4(&m_pTourData->m_Seed);
    FgDataLoader* data = mgr->GetFgData("FgDataTour/A0.bin");
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsSetter(m_pField, data));
}

// 0x0070553C slot 0x1C (name is ours)
void isfactory::HideAndSeekBuilder::Setup1C()
{
    if (m_pField == NULL) {
        return;
    }
    // pitfalls only at level 2
    if (Func76F7E4(&m_pTourData->m_Seed) != 2) {
        return;
    }
    PlacePitfalls(m_pField, m_pRandom, 2);
}

// 0x007057AC slot 0x24 (name is ours)
void isfactory::HideAndSeekBuilder::Setup24()
{
    Unknown1DC370 object;
    object.Func1DC77C(66, m_pRandom);
    object.Func1DC77C(67, m_pRandom);
    object.Func1DC77C(68, m_pRandom);
    object.Func1DC77C(69, m_pRandom);
    object.Func1DC77C(70, m_pRandom);
    object.Func1DC77C(PickRandomId(m_pRandom), m_pRandom);
    object.Func1DC77C(PickRandomId(m_pRandom), m_pRandom);
    object.Func1DC77C(PickRandomId(m_pRandom), m_pRandom);
    object.Func1DC77C(PickRandomId(m_pRandom), m_pRandom);
    object.Func1DC77C(PickRandomId(m_pRandom), m_pRandom);
}

// 0x007058A4 (name is ours)
u8 isfactory::HideAndSeekBuilder::PickRandomId(sead::Random* random)
{
    sead::FixedObjArray<u32, 252> ids;
    for (u32 i = 0; i < 252; i++) {
        const u8 id = static_cast<u8>(i);
        if (Func5CAF84(id) && Func5CB03C(id)) {
            ids.pushBack(id);
        }
    }
    ids.shuffle(random);
    const u32* id = ids.front();
    if (id == NULL) {
        return 252;
    }
    return static_cast<u8>(*id);
}

} // namespace isfactory
