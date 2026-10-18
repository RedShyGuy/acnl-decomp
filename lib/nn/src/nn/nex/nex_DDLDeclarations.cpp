#include "nn/nex/nex_DDLDeclarations.h"

namespace nn {
namespace nex {
// 0x00377620
nn::nex::DDLDeclarations::DDLDeclarations(bool unknown0x10)
{
    m_Unknown0x08 = false;
    m_Unknown0x04 = 0;
    m_Unknown0x0C = 0;
    m_Unknown0x10 = unknown0x10;
}

// 0x0037764C slot 0x00
// 0x00377644 (deleting dtor)
nn::nex::DDLDeclarations::~DDLDeclarations()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
