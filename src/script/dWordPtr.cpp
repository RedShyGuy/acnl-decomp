#include "script/dWordPtr.h"
#include "script/dCatEngine.h"
#include "script/dCatRepelTagEngine.h"
#include "script/dContainTagEngine.h"
#include "script/dCopyEngine.h"
#include "script/dCopyRepelTagEngine.h"
#include <algorithm>

namespace script {
// 0x003081E8
WordPtr::WordPtr(char16* buffer, u32 size) : m_pBuffer(buffer), m_Size(size)
{
}

// 0x008916F4
extern const char16 WORD_NULL_CHAR = 0;

// 0x00308368 slot 0x00 | libgarden
// 0x005FDA88 (deleting dtor)
WordPtr::~WordPtr()
{
    // empty (in the original too)
}

// 0x005FD5F0 slot 0x18
void WordPtr::Append(const IWord& src)
{
    bool isSrcSv = src.IsSv();
    bool isSv = IsSv();
    if (isSrcSv || isSv) {
        if (isSv && !isSrcSv) {
            CatRepelTagEngine engine;
            engine.Cat(this, &src);
            return;
        }
        if (isSrcSv && !isSv && src.HasTag()) {
            // a text of the save data with tags is not appended
            return;
        }
    }
    CatEngine engine;
    engine.Cat(this, &src);
}

// 0x005FD774 slot 0x14
void WordPtr::Copy(const IWord& src)
{
    bool isSrcSv = src.IsSv();
    bool isSv = IsSv();
    if (isSrcSv || isSv) {
        if (isSv && !isSrcSv) {
            CopyRepelTagEngine engine;
            engine.Copy(this, &src);
            return;
        }
        if (isSrcSv && !isSv && src.HasTag()) {
            // a text of the save data with tags is not taken
            Clear();
            return;
        }
    }
    CopyEngine engine;
    engine.Copy(this, &src);
}

// 0x005FD980 slot 0x20
void WordPtr::Fill(char16 c, u32 num)
{
    if (m_pBuffer == NULL || m_Size == 0) {
        return;
    }
    if (num == 0 || num >= m_Size) {
        num = m_Size - 1;
    }
    char16* end = m_pBuffer + num;
    std::fill(m_pBuffer, end, c);
    *end = 0;
}

// 0x005FD9F4 slot 0x1C
void WordPtr::Clear()
{
    if (m_pBuffer == NULL || m_Size == 0) {
        return;
    }
    std::fill(m_pBuffer, m_pBuffer + m_Size, 0);
}

// 0x005FDA64 slot 0x08 | libgarden
char16* WordPtr::GetWord()
{
    return m_pBuffer;
}

// 0x0075DC78 slot 0x0C | libgarden
const char16* WordPtr::GetWord() const
{
    return m_pBuffer;
}

// 0x0075DC60 slot 0x10 | libgarden
u32 WordPtr::GetSize() const
{
    return m_Size;
}
} // namespace script
