#include "script/dContainTagEngine.h"

namespace script {
// 0x005E7178 slot 0x1C
void ContainTagEngine::OnTagEnd()
{
    m_HasTagEnd = 1;
}

// 0x005E7184 slot 0x18
void ContainTagEngine::OnTag(const Tag*)
{
    m_HasTag = 1;
}

// 0x005E7190 slot 0x10
bool ContainTagEngine::IsContinue()
{
    return !(m_HasTag || m_HasTagEnd);
}

// 0x005E71BC slot 0x00
// 0x005E71AC (deleting dtor)
ContainTagEngine::~ContainTagEngine()
{
    // (only the base is destroyed)
}
} // namespace script
