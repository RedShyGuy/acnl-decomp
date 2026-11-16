#pragma once

#include "decomp.h"
#include "Other/dFieldBuilder.h"

class TourData;
namespace sead {
class Random;
} // namespace sead

// The builders of the fields of the island huts (namespace and class names from the RTTI; the rest is ours).
namespace ishutfact2 {
// the fields of the huts (Island_Hut0..9 = the houses of the Fossil Finder's and Scavenger Tours) are made by
// BuildFields (ishutfact2_Api.h)

// An empty hut (e.g. the empty house of the Fossil Finder's Tour that the players fill): one block with the ground of
// the hut and its heights, no items.
// RTTI N10ishutfact212BasicBuilderE @ 0x008CD888
// vtable 0x008FAB0C (vptr 0x008FAB14), offset_to_top 0, 11 entries
class BasicBuilder : public ::FieldBuilder
{
public:
    BasicBuilder(Field* field, sead::Random* random, TourData* tourData)
        : FieldBuilder(field), m_pRandom(random), m_pTourData(tourData)
    {
    }
    ~BasicBuilder() override;

    void SetupBlocks() override;
    void SetupHeights() override;
    void Setup10() override;
    void ClearItems() override;
    void SetupItems() override;
    void Setup1C() override;
    void Setup20() override;
    void Setup24() override;
    void SetupGrass() override;

    sead::Random* m_pRandom; // 0x08 the random numbers of the tour (from TourData::m_Seed)
    TourData* m_pTourData;   // 0x0C
};
ASSERT_SIZE(BasicBuilder, 0x10);
} // namespace ishutfact2
