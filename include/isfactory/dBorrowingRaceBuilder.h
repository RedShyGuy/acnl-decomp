#pragma once

#include "decomp.h"
#include "isfactory/dVillageBuilder.h"

namespace isfactory {
// RTTI N9isfactory20BorrowingRaceBuilderE @ 0x008D41D8
// vtable 0x0090C4C0 (vptr 0x0090C4C8), offset_to_top 0, 11 entries
class BorrowingRaceBuilder : public ::isfactory::VillageBuilder
{
public:
    BorrowingRaceBuilder(Field* field, sead::Random* random, TourData* tourData) : VillageBuilder(field, random, tourData)
    {
    }
    ~BorrowingRaceBuilder() override;
    void Setup24() override;
};
ASSERT_SIZE(BorrowingRaceBuilder, 0x10);
} // namespace isfactory
