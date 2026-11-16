#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory18HideAndSeekBuilderE @ 0x008D41C0
// vtable 0x0090C458 (vptr 0x0090C460), offset_to_top 0, 11 entries
class HideAndSeekBuilder : public ::isfactory::VillageBuilder
{
public:
    HideAndSeekBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~HideAndSeekBuilder() override;
    void SetupItems() override;
    void Setup1C() override;
    void Setup24() override;

    // a random id 0..251 for which Func5CAF84 and Func5CB03C hold (252 if there is none; name is ours)
    DECOMP_NOINLINE u8 PickRandomId(sead::Random* random); // 0x007058A4
};
ASSERT_SIZE(HideAndSeekBuilder, 0x10);
} // namespace isfactory
