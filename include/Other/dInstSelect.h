#pragma once

#include "decomp.h"
#include "Other/dSelectBase.h"

// A SelectBase with N rows: the rows and their buttons are members (only the class name is from the RTTI).
// Instantiations found in the binary:
//   InstSelect<8>  typeinfo 0x008CB0F0  vtable 0x008EC1D0
template <int N>
class InstSelect : public ::SelectBase
{
public:
    InstSelect()
    {
        m_RowNum = N;
        m_pContents = m_Contents;
        m_pNodes = m_Nodes;
    }
    DECOMP_NOINLINE virtual ~InstSelect() {}

    ContentNode m_Contents[N];      // 0xAEC
    ButtonActionNode m_Nodes[N];    // 0xAEC + N * 0x1C
};
ASSERT_SIZE(InstSelect<8>, 0x12CC);
