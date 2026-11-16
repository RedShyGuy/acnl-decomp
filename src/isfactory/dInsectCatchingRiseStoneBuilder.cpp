#include "isfactory/dInsectCatchingRiseStoneBuilder.h"
#include "field/dField.h"
#include "field/field_Extern.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace isfactory {
// 0x00707B88 slot 0x00
// 0x00707B78 (deleting dtor)
isfactory::InsectCatchingRiseStoneBuilder::~InsectCatchingRiseStoneBuilder()
{
    // empty (in the original too)
}

// 0x00707A30 slot 0x1C (name is ours)
void isfactory::InsectCatchingRiseStoneBuilder::Setup1C()
{
    // twice as many rocks again
    const FieldRect& units = m_pField->GetUnitBounds();
    Field* field = m_pField;
    u32 rockNum = 0;
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        if (field == NULL) {
            continue;
        }
        const SvFgName* item = field->GetFg((*it).x, (*it).y, 0);
        if (item != NULL && item->GetFgKind() == SvFgName::FG_KIND_ROCK) {
            rockNum++;
        }
    }
    for (u32 i = 0; i < rockNum * 2; i++) {
        const SvFgName rock(static_cast<SvFgName::ID>(SvFgName::NAME_ROCK_A + m_pRandom->getU32(5)));
        Func527094(m_pField, &units, &rock, m_pRandom);
    }
}

} // namespace isfactory
