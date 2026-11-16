#pragma once

#include "decomp.h"
#include "isfactory/dBasicBuilder.h"

namespace isfactory {
// RTTI N9isfactory14PikohanBuilderE @ 0x008D4190
// vtable 0x0090C3F0 (vptr 0x0090C3F8), offset_to_top 0, 11 entries
class PikohanBuilder : public ::isfactory::BasicBuilder
{
public:
    PikohanBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData)
    {
    }
    ~PikohanBuilder() override;
    void SetupBlocks() override;
    void Setup10() override;
    void SetupItems() override;
};
ASSERT_SIZE(PikohanBuilder, 0x10);
} // namespace isfactory
