#include "script/dCatEngine.h"
#include "script/dCopyEngine.h"
#include "script/dWordPtr.h"

namespace script {
// 0x005FEA74 (name is ours)
void CatEngine::Cat(IWord* dst, const IWord* src)
{
    if (dst == src) {
        return;
    }
    char16* buffer = dst->GetWord();
    u32 size = dst->GetSize();
    if (buffer == NULL || size == 0) {
        return;
    }
    // the end of the text of dst
    Run(buffer);
    if (buffer > m_pCur) {
        return;
    }
    u32 length = m_pCur - buffer;
    if (size <= length) {
        return;
    }
    WordPtr rest(buffer + length, size - length);
    CopyEngine engine;
    engine.Copy(&rest, src);
}

// 0x005FEB74 slot 0x00
// 0x005FEB64 (deleting dtor)
CatEngine::~CatEngine()
{
    // (only the base is destroyed)
}
} // namespace script
