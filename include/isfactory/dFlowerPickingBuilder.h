#pragma once

#include "decomp.h"
#include "isfactory/dPeninsulaBuilder.h"

namespace isfactory {
// RTTI N9isfactory20FlowerPickingBuilderE @ 0x008D41F0
// vtable 0x0090C528 (vptr 0x0090C530), offset_to_top 0, 11 entries
class FlowerPickingBuilder : public ::isfactory::PeninsulaBuilder
{
public:
    FlowerPickingBuilder(Field* field, sead::Random* random, TourData* tourData) : PeninsulaBuilder(field, random, tourData)
    {
    }
    ~FlowerPickingBuilder() override;
    void SetupBlocks() override;
    void Setup1C() override;
    void Setup20() override;
};
ASSERT_SIZE(FlowerPickingBuilder, 0x10);
} // namespace isfactory
