#pragma once

#include "decomp.h"
class FgDataLoader;
#include "isfactory/dBasicBuilder.h"

namespace isfactory {
// RTTI N9isfactory11MazeBuilderE @ 0x008D413C
// vtable 0x0090C2EC (vptr 0x0090C2F4), offset_to_top 0, 11 entries
class MazeBuilder : public ::isfactory::BasicBuilder
{
public:
    MazeBuilder(Field* field, sead::Random* random, TourData* tourData) : BasicBuilder(field, random, tourData), m_pData(NULL)
    {
    }
    ~MazeBuilder() override;
    void SetupBlocks() override;
    void Setup10() override;
    void SetupItems() override;
    void Setup1C() override;

    FgDataLoader* m_pData; // 0x10 the item layout of the maze (SetupItems)
};
ASSERT_SIZE(MazeBuilder, 0x14);
} // namespace isfactory
