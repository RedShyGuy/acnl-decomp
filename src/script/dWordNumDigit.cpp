#include "script/dWordNumDigit.h"

namespace script {
// 0x00312C58 (name after the RTTI)
WordNumDigit::WordNumDigit() : m_Kind(0), m_Value(0)
{
}

// 0x00312D38 slot 0x00 | slot vf_0x00 of script::WordPtr
// 0x005E3E04 (deleting dtor)
WordNumDigit::~WordNumDigit()
{
    // empty (in the original too)
}

// 0x005E3DD4 slot 0x1C
void WordNumDigit::Clear()
{
    WordPtr::Clear();
    m_Info.Reset();
    m_Kind = 0;
    m_Value = 0;
}
} // namespace script
