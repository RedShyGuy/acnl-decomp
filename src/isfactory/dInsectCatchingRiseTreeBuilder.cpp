#include "isfactory/dInsectCatchingRiseTreeBuilder.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dJmpBlock.h"
#include "field/field_Extern.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace {
// the units of the field with an item of the fg kind in layer 0
inline u32 CountFgKind(Field* field, const FieldRect& units, u8 kind)
{
    u32 num = 0;
    for (FieldRect::iterator it = units.begin(); it != units.end(); ++it) {
        if (field == NULL) {
            continue;
        }
        const SvFgName* item = field->GetFg((*it).x, (*it).y, 0);
        if (item != NULL && item->GetFgKind() == kind) {
            num++;
        }
    }
    return num;
}

// whether all units around position are sand (the attributes 0x16 and 0x80) without items
inline bool IsFreeSand(Field* field, const Vector2i& position)
{
    FieldRect around(Vector2i(-1, -1), Vector2i(1, 1));
    around += position;
    for (FieldRect::iterator it = around.begin(); it != around.end(); ++it) {
        if (field == NULL) {
            return false;
        }
        const Vector2i block = FdInfo::ToBlock(*it);
        JmpBlock* jmpBlock = field->GetJmpBlock(block.x, block.y);
        if (jmpBlock == NULL) {
            return false;
        }
        const Vector2i unit = FdInfo::ToLocalUnit(*it);
        const FieldAttribute attribute = jmpBlock->GetAttribute(unit.x, unit.y);
        if (attribute != FIELD_ATTRIBUTE_SAND_16 && attribute != FIELD_ATTRIBUTE_UNKNOWN_80) {
            return false;
        }
    }
    for (FieldRect::iterator it = around.begin(); it != around.end(); ++it) {
        if (field == NULL) {
            continue;
        }
        const SvFgName* item0 = field->GetFg((*it).x, (*it).y, 0);
        const SvFgName* item1 = field->GetFg((*it).x, (*it).y, 1);
        bool isUsed0 = false;
        if (item0 != NULL && !item0->IsEmpty()) {
            isUsed0 = true;
        }
        bool isUsed1 = false;
        if (item1 != NULL && !item1->IsEmpty()) {
            isUsed1 = true;
        }
        if (isUsed0 || isUsed1) {
            return false;
        }
    }
    return true;
}
} // namespace

namespace isfactory {
// 0x007077D8 slot 0x00
// 0x007077C8 (deleting dtor)
isfactory::InsectCatchingRiseTreeBuilder::~InsectCatchingRiseTreeBuilder()
{
    // empty (in the original too)
}

// 0x007070CC slot 0x1C (name is ours)
void isfactory::InsectCatchingRiseTreeBuilder::Setup1C()
{
    // half as many trees, cedars and palms again
    const FieldRect& units = m_pField->GetUnitBounds();
    const u32 treeNum = CountFgKind(m_pField, units, SvFgName::FG_KIND_TREE);
    const u32 cedarNum = CountFgKind(m_pField, units, SvFgName::FG_KIND_CEDAR_TREE);
    const u32 palmNum = CountFgKind(m_pField, units, SvFgName::FG_KIND_PALM_TREE);
    const SvFgName tree(SvFgName::NAME_TREE);
    for (u32 i = 0; i < treeNum / 2; i++) {
        Func527094(m_pField, &units, &tree, m_pRandom);
    }
    const SvFgName cedar(SvFgName::NAME_CEDAR);
    for (u32 i = 0; i < cedarNum / 2; i++) {
        Func527094(m_pField, &units, &cedar, m_pRandom);
    }
    // palms on free sand (500 tries each)
    const SvFgName palm(SvFgName::NAME_COCONUT_PALM_TREE);
    Vector2i placed;
    for (u32 i = 0; i < palmNum / 2; i++) {
        for (u32 tries = 0;; tries++) {
            Vector2i position(units.min);
            position.x += m_pRandom->getU32(units.GetWidth());
            position.y += m_pRandom->getU32(units.GetHeight());
            if (IsFreeSand(m_pField, position)) {
                m_pField->PlaceItem(palm, position.x, position.y, 0);
                placed = position;
                break;
            }
            if (tries >= 500) {
                placed = Vector2i(0, 0);
                break;
            }
        }
    }
}

} // namespace isfactory
