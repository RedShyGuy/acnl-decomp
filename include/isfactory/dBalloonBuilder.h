#pragma once

#include "decomp.h"
#include "isfactory/dPeninsulaBuilder.h"

namespace isfactory {
// RTTI N9isfactory14BalloonBuilderE @ 0x008D4178
// vtable 0x0090C388 (vptr 0x0090C390), offset_to_top 0, 11 entries
class BalloonBuilder : public ::isfactory::PeninsulaBuilder
{
public:
    BalloonBuilder(Field* field, sead::Random* random, TourData* tourData) : PeninsulaBuilder(field, random, tourData)
    {
    }
    ~BalloonBuilder() override;
    void SetupItems() override;
    void Setup1C() override;
};
ASSERT_SIZE(BalloonBuilder, 0x10);
} // namespace isfactory
