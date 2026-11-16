#pragma once

#include "decomp.h"
#include "isfactory/dIslandBuilder.h"

namespace isfactory {
// RTTI N9isfactory20FishingIslandBuilderE @ 0x008D41E4
// vtable 0x0090C4F4 (vptr 0x0090C4FC), offset_to_top 0, 11 entries
class FishingIslandBuilder : public ::isfactory::IslandBuilder
{
public:
    FishingIslandBuilder(Field* field, sead::Random* random, TourData* tourData) : IslandBuilder(field, random, tourData)
    {
    }
    ~FishingIslandBuilder() override;
};
ASSERT_SIZE(FishingIslandBuilder, 0x10);
} // namespace isfactory
