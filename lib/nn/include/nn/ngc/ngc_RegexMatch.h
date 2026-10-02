#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexMatch
{
public:
    void JumpAtomicZeroWidthAssersion(int); // 0x003DAD74 | fefates:bytes [tier B]
    void IsMatch(const nn::ngc::UnitList<nn::ngc::RegexDfaState>*, const wchar_t*); // 0x003DB0DC | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
