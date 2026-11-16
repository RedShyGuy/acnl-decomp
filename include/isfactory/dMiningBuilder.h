#pragma once

#include "decomp.h"
#include "isfactory/dPeninsulaBuilder.h"

namespace isfactory {
// RTTI N9isfactory13MiningBuilderE @ 0x008D416C
// vtable 0x0090C354 (vptr 0x0090C35C), offset_to_top 0, 11 entries
class MiningBuilder : public ::isfactory::PeninsulaBuilder
{
public:
    MiningBuilder(Field* field, sead::Random* random, TourData* tourData) : PeninsulaBuilder(field, random, tourData)
    {
    }
    ~MiningBuilder() override;
    void SetupItems() override;
    void Setup1C() override;
};
ASSERT_SIZE(MiningBuilder, 0x10);
} // namespace isfactory
