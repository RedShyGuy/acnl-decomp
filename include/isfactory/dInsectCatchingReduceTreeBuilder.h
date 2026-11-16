#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory31InsectCatchingReduceTreeBuilderE @ 0x008D422C
// vtable 0x0090C62C (vptr 0x0090C634), offset_to_top 0, 11 entries
class InsectCatchingReduceTreeBuilder : public ::isfactory::VillageBuilder
{
public:
    InsectCatchingReduceTreeBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~InsectCatchingReduceTreeBuilder() override;
    void SetupItems() override;
};
ASSERT_SIZE(InsectCatchingReduceTreeBuilder, 0x10);
} // namespace isfactory
