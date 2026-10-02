#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

namespace escape {
// vtable +0xE0CAC in ModuleMiniGame0.cro, offset_to_top 0, 23 entries
class BsEscapeScoreManager : public ::UtlBase<Base>
{
public:
    BsEscapeScoreManager(); // ctor address unknown
    virtual ~BsEscapeScoreManager(); // ModuleMiniGame0.cro +0x0818E0 slot 0x00
    virtual void Calc(); // ModuleMiniGame0.cro +0x081878 slot 0x24
    virtual void Draw(); // ModuleMiniGame0.cro +0x08168C slot 0x30
};
} // namespace escape
