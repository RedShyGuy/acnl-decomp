#include "isfactory/dFossilDigBuilder.h"
#include "field/dField.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "Item/item_Api.h"
#include "Other/dTourData.h"
#include "script/dScriptExtern.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace isfactory {
// 0x00705298 slot 0x00
// 0x00705288 (deleting dtor)
isfactory::FossilDigBuilder::~FossilDigBuilder()
{
    // empty (in the original too)
}

// 0x00704EC8 slot 0x1C (name is ours)
void isfactory::FossilDigBuilder::Setup1C()
{
    const FieldRect* units = &m_pField->GetUnitBounds();
    TourData::HutFossil (*huts)[TourData::HUT_FOSSIL_NUM] = m_pTourData->m_HutFossils;
    SvFgNameSet fossils;
    // the fossils of the huts of the players are buried here
    const u32 playerCount = Func6E1510();
    for (u32 player = 0; player < playerCount; player++) {
        for (u32 i = 0; i < TourData::HUT_FOSSIL_NUM; i++) {
            if (huts[player][i].item == SvFgName::NAME_EMPTY) {
                continue;
            }
            SvFgName fossil(static_cast<SvFgName::ID>(huts[player][i].item));
            fossil.SetBuried();
            Func527094(m_pField, units, &fossil, m_pRandom);
        }
    }
    // the other fossils without the Ptera and the Diplo and without the fossils of all huts
    fossils.Clear();
    Func302978(&fossils, item::KIND_FOSSIL);
    const u16 excluded[] = {
        SvFgName::NAME_PTERA_SKULL, SvFgName::NAME_PTERA_RIGHT_WING, SvFgName::NAME_PTERA_LEFT_WING,
        SvFgName::NAME_DIPLO_NECK,  SvFgName::NAME_DIPLO_CHEST,      SvFgName::NAME_DIPLO_HIP,
        SvFgName::NAME_DIPLO_TAIL,  SvFgName::NAME_DIPLO_SKULL,
    };
    for (u32 i = 0; i < sizeof(excluded) / sizeof(excluded[0]); i++) {
        const SvFgName fossil(static_cast<SvFgName::ID>(excluded[i]));
        RemoveFgName(&fossils, &fossil);
    }
    for (u32 hut = 0; hut < 4; hut++) {
        for (u32 i = 0; i < TourData::HUT_FOSSIL_NUM; i++) {
            if (huts[hut][i].item == SvFgName::NAME_EMPTY) {
                continue;
            }
            const SvFgName fossil(static_cast<SvFgName::ID>(huts[hut][i].item));
            RemoveFgName(&fossils, &fossil);
        }
    }
    // the number of the other fossils for the level and the players
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    const u32 players = Func6E1510();
    u32 count;
    if (level == 0) {
        count = players * 4 + 4;
    } else if (level == 1) {
        count = (players + 1) * 5;
    } else if (level == 2) {
        count = (players + 1) * 6;
    } else {
        count = 8;
    }
    for (u32 i = 0; i < count; i++) {
        SvFgName fossil = Func1AD2E0(&fossils, m_pRandom->getU32());
        fossil.SetBuried();
        Func527094(m_pField, units, &fossil, m_pRandom);
        RemoveFgName(&fossils, &fossil);
    }
}

// 0x00705230 slot 0x24 (name is ours)
void isfactory::FossilDigBuilder::Setup24()
{
    Unknown1DC370 object;
    const u32 num = Func6E1510() * 2;
    for (u32 i = 0; i < num; i += 2) {
        object.Func1DD618(static_cast<u8>(i + 66), static_cast<u8>(i + 67), m_pRandom);
    }
}

} // namespace isfactory
