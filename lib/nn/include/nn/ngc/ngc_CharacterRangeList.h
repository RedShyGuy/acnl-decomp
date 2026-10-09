#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// characters from m_Begin to m_End (both included); names are ours
struct CharacterRange
{
    wchar_t m_Begin;    // 0x0
    wchar_t m_End;      // 0x2
};
ASSERT_SIZE(CharacterRange, 0x4);

// The characters of a character class: ranges in ascending order without overlaps.
class CharacterRangeList : public UnitList<CharacterRange>
{
public:
    static const wchar_t CHARACTER_MAX = 0xFFFF;

    bool RemoveFromClass(const nn::ngc::CharacterRangeList* other); // 0x003DF05C | fefates:bytes [tier B]
    bool RemoveFromClass(wchar_t character); // 0x003DF174 | fefates:bytes [tier B]
    bool AddCharacterRange(wchar_t begin, wchar_t end); // 0x003DF260 | fefates:bytes [tier B]
    bool MergeCharacterRanges(const nn::ngc::CharacterRangeList* other); // 0x003DF404 | fefates:bytes [tier B]
    bool Inverse(); // 0x003DF44C | fefates:bytes [tier B]
    bool Intersect(const nn::ngc::CharacterRangeList* other); // 0x003DF7B8 | fefates:bytes [tier B]
    // (inline in the original, with a copy out of line)
    bool HasSharedCharRangeArea(const nn::ngc::CharacterRangeList* other) const; // 0x0072EBBC | fefates:bytes [tier B]
};
ASSERT_SIZE(CharacterRangeList, 0xC);
} // namespace ngc
} // namespace nn
