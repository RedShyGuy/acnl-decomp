#pragma once

#include "decomp.h"
#include "isfactory/dIslandBuilder.h"

namespace isfactory {
// RTTI N9isfactory13DivingBuilderE @ 0x008D4154
// vtable 0x0090C320 (vptr 0x0090C328), offset_to_top 0, 11 entries
class DivingBuilder : public ::isfactory::IslandBuilder
{
public:
    DivingBuilder(Field* field, sead::Random* random, TourData* tourData) : IslandBuilder(field, random, tourData)
    {
    }
    ~DivingBuilder() override;
};
ASSERT_SIZE(DivingBuilder, 0x10);
} // namespace isfactory
