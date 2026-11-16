#include "Sv/dSvFgBlock.h"

#include "field/dField.h"

// (its own file: the static initializer only builds this item)
// 0x00790B34 (static initializer)

// 0x0095CFF2
SvFgName SvFgBlock::s_EmptyItem(static_cast<SvFgName::ID>(SvFgName::NAME_EMPTY));

// 0x001399DC (name is ours)
SvFgBlock::~SvFgBlock()
{
    // (only the items are destroyed)
}

// 0x006F0890 (name is ours)
void SvFgBlock::Clear()
{
    for (s32 z = 0; z < static_cast<s32>(UNIT_BASE_NUM); z++) {
        for (s32 x = 0; x < static_cast<s32>(UNIT_BASE_NUM); x++) {
            if (BgInfo::IsInside(x, z)) {
                items[z][x] = s_EmptyItem;
            }
        }
    }
}

// 0x002FCC78 (name is ours)
void SvFgBlock::SetFg(const SvFgName& item, s32 x, s32 z)
{
    if (BgInfo::IsInside(x, z)) {
        items[z][x] = item;
    }
}

// 0x00771380 (name is ours)
SvFgName* SvFgBlock::GetFgChecked(s32 x, s32 z)
{
    if (BgInfo::IsInside(x, z)) {
        return &items[z][x];
    }
    return &s_EmptyItem;
}
