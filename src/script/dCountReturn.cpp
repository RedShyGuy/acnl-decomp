#include "script/dCountReturn.h"

namespace script {
// 0x005D916C slot 0x14
void CountReturn::OnChar(const CharInfo* info)
{
    if (info->units == '\n') {
        m_Count++;
    }
}

// 0x005D9198 slot 0x00
// 0x005D9188 (deleting dtor)
CountReturn::~CountReturn()
{
    // (only the base is destroyed)
}
} // namespace script
