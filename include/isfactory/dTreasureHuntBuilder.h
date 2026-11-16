#pragma once

#include "decomp.h"

class FgDataLoader;
#include "isfactory/dBasicBuilder.h"
#include "isfactory/isfactory_Api.h"

namespace isfactory {
// RTTI N9isfactory19TreasureHuntBuilderE @ 0x008D41CC
// vtable 0x0090C48C (vptr 0x0090C494), offset_to_top 0, 11 entries
class TreasureHuntBuilder : public ::isfactory::BasicBuilder
{
public:
    TreasureHuntBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData), m_PlayerCount(Func6E1510()), m_pData(NULL)
    {
    }
    ~TreasureHuntBuilder() override;
    void SetupBlocks() override;
    void SetupItems() override;
    void Setup1C() override;

    u32 m_PlayerCount;     // 0x10
    FgDataLoader* m_pData; // 0x14 the item layout (SetupItems)
};
ASSERT_SIZE(TreasureHuntBuilder, 0x18);
} // namespace isfactory
