#pragma once

#include "decomp.h"
#include "isfactory/dBasicBuilder.h"

namespace isfactory {
// the tours on the peninsula
// RTTI N9isfactory16PeninsulaBuilderE @ 0x008D4184
class PeninsulaBuilder : public ::isfactory::BasicBuilder
{
public:
    PeninsulaBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData)
    {
    }
    void SetupBlocks() override;
    void Setup10() override;
};
ASSERT_SIZE(PeninsulaBuilder, 0x10);
} // namespace isfactory
