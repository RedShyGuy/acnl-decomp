#include "script/dInflectEngine.h"
#include "script/dWordFix.h"

namespace script {
// 0x005E4184 slot 0x18
void InflectEngine::OnTag(const Tag* tag)
{
    if (tag->group != 14 || tag->index != 0) {
        return;
    }
    const u8* args = reinterpret_cast<const u8*>(tag->args);
    u8 color = args[0];
    u8 begin = args[1];
    u8 buf = args[2];
    WordInfo* info = m_pWord->GetInfo();
    info->color = color;
    info->begin = begin;
    info->buf = buf;
}

// 0x005E41D8 slot 0x00
// 0x005E41C8 (deleting dtor)
InflectEngine::~InflectEngine()
{
    // (only the base is destroyed)
}
} // namespace script
