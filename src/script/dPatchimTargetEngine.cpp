#include "script/dPatchimTargetEngine.h"

namespace script {
// 0x005E9D84 slot 0x14
void PatchimTargetEngine::OnChar(const CharInfo* info)
{
    if (info->pText == NULL) {
        return;
    }
    char16 c = *info->pText;
    if (c == ' ' || c == 0x3000 || c == '\n') {
        return;
    }
    m_LastChar = c;
}

// 0x005E9DC0 slot 0x00
// 0x005E9DB0 (deleting dtor)
PatchimTargetEngine::~PatchimTargetEngine()
{
    // (only the base is destroyed)
}
} // namespace script
