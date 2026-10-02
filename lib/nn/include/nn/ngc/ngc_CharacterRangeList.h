#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class CharacterRangeList
{
public:
    void RemoveFromClass(const nn::ngc::CharacterRangeList*); // 0x003DF05C | fefates:bytes [tier B]
    void RemoveFromClass(wchar_t); // 0x003DF174 | fefates:bytes [tier B]
    void AddCharacterRange(wchar_t, wchar_t); // 0x003DF260 | fefates:bytes [tier B]
    void MergeCharacterRanges(const nn::ngc::CharacterRangeList*); // 0x003DF404 | fefates:bytes [tier B]
    void Inverse(); // 0x003DF44C | fefates:bytes [tier B]
    void Intersect(const nn::ngc::CharacterRangeList*); // 0x003DF7B8 | fefates:bytes [tier B]
    void HasSharedCharRangeArea(const nn::ngc::CharacterRangeList*) const; // 0x0072EBBC | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
