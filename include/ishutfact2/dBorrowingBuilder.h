#pragma once

#include "decomp.h"
#include "field/dFieldTypes.h"
#include "ishutfact2/dBasicBuilder.h"

namespace ishutfact2 {
// The five houses of the Scavenger Tour (the huts lead back to Island_BorrowingRace): the room of a villager with its
// items, in which the players look for the furniture of the list.
// RTTI N10ishutfact216BorrowingBuilderE @ 0x008CD894
// vtable 0x008FAB40 (vptr 0x008FAB48), offset_to_top 0, 11 entries
class BorrowingBuilder : public ::ishutfact2::BasicBuilder
{
public:
    // the entries of the huts (BuildFields fills 5; the original copies 6 words)
    static const u32 ENTRY_NUM = 6;

    BorrowingBuilder(Field* field, sead::Random* random, TourData* tourData, const u32* entries, FieldName hut)
        : BasicBuilder(field, random, tourData), m_Hut(hut)
    {
        for (u32 i = 0; i < ENTRY_NUM; i++) {
            m_Entries[i] = entries[i];
        }
    }
    ~BorrowingBuilder() override;

    void Setup1C() override;

    // for each hut: the villager (an index of the villager tables), at level 2 the key of the layout "FgDataTour/L0.bin"
    u32 m_Entries[ENTRY_NUM]; // 0x10
    FieldName m_Hut;          // 0x28
};
ASSERT_SIZE(BorrowingBuilder, 0x2C);
} // namespace ishutfact2
