#pragma once

#include "decomp.h"
#include "isfactory/dBasicBuilder.h"

namespace isfactory {
// the tours in the village of the island
// RTTI N9isfactory14VillageBuilderE @ 0x008D419C
class VillageBuilder : public ::isfactory::BasicBuilder
{
public:
    VillageBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData)
    {
    }
    void SetupBlocks() override;
    void Setup10() override;
};
ASSERT_SIZE(VillageBuilder, 0x10);
} // namespace isfactory
