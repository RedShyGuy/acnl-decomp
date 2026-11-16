#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory30InsectCatchingButterflyBuilderE @ 0x008D4214
// vtable 0x0090C5C4 (vptr 0x0090C5CC), offset_to_top 0, 11 entries
class InsectCatchingButterflyBuilder : public ::isfactory::VillageBuilder
{
public:
    InsectCatchingButterflyBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~InsectCatchingButterflyBuilder() override;
    void Setup1C() override;
};
ASSERT_SIZE(InsectCatchingButterflyBuilder, 0x10);
} // namespace isfactory
