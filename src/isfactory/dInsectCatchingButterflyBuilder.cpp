#include "isfactory/dInsectCatchingButterflyBuilder.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dJmpBlock.h"
#include "field/field_Extern.h"
#include "Item/item_Api.h"
#include "script/dScriptExtern.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace isfactory {
// 0x00707A2C slot 0x00
// 0x00707A1C (deleting dtor)
isfactory::InsectCatchingButterflyBuilder::~InsectCatchingButterflyBuilder()
{
    // empty (in the original too)
}

// 0x007077DC slot 0x1C (name is ours)
void isfactory::InsectCatchingButterflyBuilder::Setup1C()
{
    // flowers on 5 % of the free soil units of all blocks but one
    SvFgNameSet flowers;
    Func302978(&flowers, item::KIND_FLOWER);
    Func302A90(&flowers, 74);
    const FieldRect& units = m_pField->GetUnitBounds();
    Vector2i excluded;
    Func6FAD4C(&excluded.x, &excluded.y);
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        const Vector2i block = FdInfo::ToBlock(*it);
        if (!(excluded != block)) {
            continue;
        }
        const Vector2i unit = FdInfo::ToLocalUnit(*it);
        JmpBlock* jmpBlock = m_pField->GetJmpBlock(block.x, block.y);
        if (jmpBlock == NULL || jmpBlock->GetAttribute(unit.x, unit.y) != FIELD_ATTRIBUTE_SOIL_14) {
            continue;
        }
        if (m_pRandom->getU32() % 100 >= 5) {
            continue;
        }
        const SvFgName* item = m_pField->GetFg((*it).x, (*it).y, 0);
        if (item == NULL || !item->IsEmpty()) {
            continue;
        }
        const SvFgName flower = Func1AD2E0(&flowers, m_pRandom->getU32());
        SvFgName plant;
        Func768A60(&flower, &plant);
        m_pField->PlaceItem(plant, (*it).x, (*it).y, 0);
    }
}

} // namespace isfactory
