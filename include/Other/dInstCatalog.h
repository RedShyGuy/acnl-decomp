#pragma once

#include "decomp.h"
#include "Other/dCatalogBase.h"
#include "Other/dContentNode.h"

// A CatalogBase with N rows as members (only the class name is from the RTTI).
// Instantiations found in the binary:
//   InstCatalog<8>  typeinfo 0x008CB2E8  vtable 0x008ECBC8
template <int N>
class InstCatalog : public ::CatalogBase
{
public:
    InstCatalog()
    {
        m_RowNum = N;
        m_pContents = m_Contents;
    }
    DECOMP_NOINLINE virtual ~InstCatalog() {}

    ContentNode m_Contents[N];      // 0x9E0
};
ASSERT_SIZE(InstCatalog<8>, 0xAC0);
