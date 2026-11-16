#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory14FishingBuilderE @ 0x008D4184
// vtable 0x0090C3BC (vptr 0x0090C3C4), offset_to_top 0, 11 entries
class FishingBuilder : public ::isfactory::VillageBuilder
{
public:
    FishingBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~FishingBuilder() override;
};
ASSERT_SIZE(FishingBuilder, 0x10);
} // namespace isfactory
