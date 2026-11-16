#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory30InsectCatchingRiseStoneBuilderE @ 0x008D4220
// vtable 0x0090C5F8 (vptr 0x0090C600), offset_to_top 0, 11 entries
class InsectCatchingRiseStoneBuilder : public ::isfactory::VillageBuilder
{
public:
    InsectCatchingRiseStoneBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~InsectCatchingRiseStoneBuilder() override;
    void Setup1C() override;
};
ASSERT_SIZE(InsectCatchingRiseStoneBuilder, 0x10);
} // namespace isfactory
