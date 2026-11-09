#include "script/dCatRepelTagEngine.h"
#include "script/dCopyRepelTagEngine.h"
#include "script/dWordPtr.h"

namespace script {
// 0x005E7D90 (name is ours)
void CatRepelTagEngine::Cat(IWord* dst, const IWord* src)
{
    if (dst == src) {
        return;
    }
    char16* buffer = dst->GetWord();
    u32 size = dst->GetSize();
    if (buffer == NULL || size == 0) {
        return;
    }
    Run(buffer);
    if (buffer > m_pCur) {
        return;
    }
    u32 length = m_pCur - buffer;
    if (length >= size) {
        return;
    }
    WordPtr rest(buffer + length, size - length);
    CopyRepelTagEngine engine;
    engine.Copy(&rest, src);
}

// 0x005E7F3C slot 0x00
// 0x005E7F2C (deleting dtor)
CatRepelTagEngine::~CatRepelTagEngine()
{
    // (only the base is destroyed)
}
} // namespace script
