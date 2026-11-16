#include "isfactory/dBorrowingRaceBuilder.h"
#include "field/field_Extern.h"
#include "ishutfact2/ishutfact2_Api.h"

namespace isfactory {
// 0x007065D4 slot 0x00
// 0x007065C4 (deleting dtor)
isfactory::BorrowingRaceBuilder::~BorrowingRaceBuilder()
{
    // empty (in the original too)
}

// 0x00706570 slot 0x24 (name is ours)
void isfactory::BorrowingRaceBuilder::Setup24()
{
    Unknown1DC370 object;
    // (the number of the huts; the original calls a function of the tour data that returns 5)
    for (u32 i = 0; i < ishutfact2::HUT_NUM; i++) {
        object.Func1DC910(static_cast<u8>(i + 66), m_pRandom);
    }
}

} // namespace isfactory
