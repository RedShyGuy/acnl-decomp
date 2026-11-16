#pragma once

#include "decomp.h"
#include "isfactory/dBasicBuilder.h"

namespace isfactory {
// the tours on the island: the acre map of the island
// RTTI N9isfactory13IslandBuilderE @ 0x008D4160
class IslandBuilder : public ::isfactory::BasicBuilder
{
public:
    IslandBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData)
    {
    }
    void SetupBlocks() override;
};
ASSERT_SIZE(IslandBuilder, 0x10);
} // namespace isfactory
