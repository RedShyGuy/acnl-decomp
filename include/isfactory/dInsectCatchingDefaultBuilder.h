#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory28InsectCatchingDefaultBuilderE @ 0x008D41FC
// vtable 0x0090C55C (vptr 0x0090C564), offset_to_top 0, 11 entries
class InsectCatchingDefaultBuilder : public ::isfactory::VillageBuilder
{
public:
    InsectCatchingDefaultBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~InsectCatchingDefaultBuilder() override;
};
ASSERT_SIZE(InsectCatchingDefaultBuilder, 0x10);
} // namespace isfactory
