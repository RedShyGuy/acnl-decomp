#include "script/dIWord.h"
#include "script/dWordPtr.h"
#include "script/dWordFix.h"

namespace script {
// ctor candidate 0x005FDA6C (unverified)
script::WordPtr::WordPtr()
{
}

// 0x003081E8
script::WordPtr::WordPtr(char16* buffer, u32 size) : m_pBuffer(buffer), m_Size(size)
{
}

// 0x008916F4
extern const char16 WORD_NULL_CHAR = 0;

// 0x005F56CC
script::WordFixUnk0C::WordFixUnk0C() : unk_0x0(4), unk_0x4(-1), unk_0x8(-1)
{
}

// 0x00308368 slot 0x00 | libgarden
script::WordPtr::~WordPtr()
{
}

// 0x005FDA88 slot 0x04 | virtual slot, introduced by script::WordPtr
void script::WordPtr::vf_0x04()
{
}

// 0x005FDA64 slot 0x08 | libgarden
void script::WordPtr::GetWord()
{
}

// 0x0075DC78 slot 0x0C | libgarden
void script::WordPtr::GetWord() const
{
}

// 0x0075DC60 slot 0x10 | libgarden
void script::WordPtr::GetSize() const
{
}

// 0x005FD774 slot 0x14 | virtual slot, introduced by script::WordPtr
void script::WordPtr::vf_0x14()
{
}

// 0x005FD5F0 slot 0x18 | virtual slot, introduced by script::WordPtr
void script::WordPtr::vf_0x18()
{
}

// 0x005FD9F4 slot 0x1C | virtual slot, introduced by script::WordPtr
void script::WordPtr::vf_0x1C()
{
}

// 0x005FD980 slot 0x20 | virtual slot, introduced by script::WordPtr
void script::WordPtr::vf_0x20()
{
}

// 0x005EB0A0 slot 0x24 | virtual slot, introduced by script::WordCPtr
void script::WordPtr::vf_0x24()
{
}

// 0x0075C8BC slot 0x28 | virtual slot, introduced by script::WordCPtr
void script::WordPtr::vf_0x28()
{
}

// 0x0075C8C4 slot 0x2C | virtual slot, introduced by script::WordCPtr
void script::WordPtr::vf_0x2C()
{
}

} // namespace script
