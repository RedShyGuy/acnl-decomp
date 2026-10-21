#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The texts of the errors of a module (SystemError::GetErrorDescription). No table is made in this
// build. The names of the members are ours.
// RTTI N2nn3nex21ErrorDescriptionTableE @ 0x008CEA84
// vtable 0x008FDCEC (vptr 0x008FDCF4), offset_to_top 0, 2 entries
class ErrorDescriptionTable : public ::nn::nex::RootObject
{
public:
    // the modules with a table
    static const u32 MODULE_COUNT = 4;

    virtual ~ErrorDescriptionTable(); // 0x00398648 slot 0x00 | fefates:bytes
    // 0x0039860C slot 0x04 (deleting dtor)

    u8 m_Module;                        // 0x04
    const wchar_t** m_ppDescriptions;   // 0x08 (by the description of the error code)

    // the tables by module
    static ErrorDescriptionTable* s_pTables[MODULE_COUNT];
};
ASSERT_SIZE(ErrorDescriptionTable, 0x0C);
} // namespace nex
} // namespace nn
