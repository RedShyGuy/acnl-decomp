#pragma once

#include "decomp.h"
#include "Other/dFieldBuilder.h"

class TourData;
namespace sead {
class Random;
} // namespace sead

namespace isfactory {
// The base of the builders of the fields of the tours (class name from the RTTI; it has no vtable of its own, its
// functions are the slots that the tour builders do not override; the rest is ours). The tour builders are made by
// CreateBuilder (isfactory_Api.h).
// RTTI N9isfactory12BasicBuilderE @ 0x008D4148
class BasicBuilder : public ::FieldBuilder
{
public:
    BasicBuilder(Field* field, sead::Random* random, TourData* tourData)
        : FieldBuilder(field), m_pRandom(random), m_pTourData(tourData)
    {
    }

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
} // namespace isfactory
