#include "nn/nex/nex_ErrorDescriptionTable.h"

namespace nn {
namespace nex {
// 0x0096CAF0 (name is ours)
ErrorDescriptionTable* ErrorDescriptionTable::s_pTables[MODULE_COUNT];

// 0x00398648 slot 0x00 | fefates:bytes
// 0x0039860C (deleting dtor)
nn::nex::ErrorDescriptionTable::~ErrorDescriptionTable()
{
    m_ppDescriptions = nullptr;
    s_pTables[m_Module] = nullptr;
}

} // namespace nex
} // namespace nn
