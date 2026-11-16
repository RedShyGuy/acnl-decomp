#pragma once

#include "decomp.h"
#include "isfactory/dPeninsulaBuilder.h"

namespace isfactory {
// RTTI N9isfactory16FossilDigBuilderE @ 0x008D41A8
// vtable 0x0090C424 (vptr 0x0090C42C), offset_to_top 0, 11 entries
class FossilDigBuilder : public ::isfactory::PeninsulaBuilder
{
public:
    FossilDigBuilder(Field* field, sead::Random* random, TourData* tourData) : PeninsulaBuilder(field, random, tourData)
    {
    }
    ~FossilDigBuilder() override;
    void Setup1C() override;
    void Setup24() override;
};
ASSERT_SIZE(FossilDigBuilder, 0x10);
} // namespace isfactory
