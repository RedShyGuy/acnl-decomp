#pragma once

#include "decomp.h"
#include "isfactory/dIslandBuilder.h"

namespace isfactory {
// RTTI N9isfactory29InsectCatchingRiseTreeBuilderE @ 0x008D4208
// vtable 0x0090C590 (vptr 0x0090C598), offset_to_top 0, 11 entries
class InsectCatchingRiseTreeBuilder : public ::isfactory::IslandBuilder
{
public:
    InsectCatchingRiseTreeBuilder(Field* field, sead::Random* random, TourData* tourData) : IslandBuilder(field, random, tourData)
    {
    }
    ~InsectCatchingRiseTreeBuilder() override;
    void Setup1C() override;
};
ASSERT_SIZE(InsectCatchingRiseTreeBuilder, 0x10);
} // namespace isfactory
