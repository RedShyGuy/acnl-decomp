#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexNfaStateCopier
{
public:
    RegexNfaStateCopier(); // TODO: default ctor added so derived stubs compile - may not exist
    RegexNfaStateCopier(nn::ngc::UnitList<nn::ngc::RegexNfaState>*, nn::ngc::ProfanityFilterTemporaryPool*); // 0x003DFC58 | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
