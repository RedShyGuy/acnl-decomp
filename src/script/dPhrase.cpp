#include "script/dPhrase.h"
#include "script/dInsert.h"

namespace script {
// 0x005F1C84
void Phrase::SetWord2AC(const IWord& word, u32 index, u32 subIndex)
{
    m_pInsert->SetWord2AC(word, index, subIndex);
}

// 0x005F1C94
void Phrase::ClearNumber(u32 index)
{
    m_pInsert->ClearNumber(index);
}

// 0x005F1CA4
void Phrase::ClearTag5_7()
{
    m_pInsert->ClearTag5_7();
}

// 0x005F1CB4
void Phrase::SetTag4_17(u32 minute, u32 index)
{
    m_pInsert->SetTag4_17(minute, index);
}

// 0x005F1CC4
void Phrase::SetTag5_32(const IWord& word, u32 index)
{
    m_pInsert->SetTag5_32(word, index);
}

// 0x005F1CD4
void Phrase::SetTag4_19(u32 value, u32 index)
{
    m_pInsert->SetTag4_19(value, index);
}

// 0x005F1CE4
void Phrase::SetTag4_16(u32 hour, u32 index)
{
    m_pInsert->SetTag4_16(hour, index);
}

// 0x005F1CF4
void Phrase::SetTag4_13(s32 value, u32 index)
{
    m_pInsert->SetTag4_13(value, index);
}

// 0x005F1D04
void Phrase::ClearTag4_18()
{
    m_pInsert->ClearTag4_18();
}

// 0x005F1D14
void Phrase::ClearDay(u32 index)
{
    m_pInsert->ClearDay(index);
}

// 0x005F1D24
void Phrase::ClearYear(u32 index)
{
    m_pInsert->ClearYear(index);
}

// 0x005F1D34
void Phrase::ClearWord238()
{
    m_pInsert->ClearWord238();
}

// 0x005F1D44
void Phrase::SetTag4_20(u32 value, u32 index)
{
    m_pInsert->SetTag4_20(value, index);
}

// 0x005F1D54
void Phrase::ClearMonth(u32 index)
{
    m_pInsert->ClearMonth(index);
}

// 0x005F1D64
void Phrase::SetWord2B8(const IWord& word, u32 index)
{
    m_pInsert->SetWord2B8(word, index);
}

// 0x005F1D74
void Phrase::ClearExtra()
{
    m_pInsert->ClearExtra();
}

// 0x005F1D84
void Phrase::SetTag5_35(const IWord& word)
{
    m_pInsert->SetTag5_35(word);
}

// 0x005F1D94
void Phrase::SetTag0_4(u64 value)
{
    m_pInsert->SetTag0_4(value);
}

// 0x005F1DA4
void Phrase::SetTag17_8(const IWord& word)
{
    m_pInsert->SetTag17_8(word);
}

// 0x005F1DB4
void Phrase::SetWord2CC(const IWord& word, u32 index)
{
    m_pInsert->SetWord2CC(word, index);
}

// 0x005F1DC4
void Phrase::SetWord2BC(const IWord& word, u32 index)
{
    m_pInsert->SetWord2BC(word, index);
}

// 0x005F1DD4
void Phrase::SetWord2DC(const IWord& word, u32 index)
{
    m_pInsert->SetWord2DC(word, index);
}

// 0x005F1DE4
void Phrase::ClearTag5_12()
{
    m_pInsert->ClearTag5_12();
}

// 0x005F1DF4
void Phrase::SetTag17_4(const IWord& word, u32 index)
{
    m_pInsert->SetTag17_4(word, index);
}

// 0x005F1E04
void Phrase::SetWord2D8(const IWord& word, u32 index)
{
    m_pInsert->SetWord2D8(word, index);
}

// 0x005F1E14
void Phrase::SetTag17_5(const IWord& word)
{
    m_pInsert->SetTag17_5(word);
}

// 0x005F1E24
void Phrase::SetTag20_2(const IWord& word, u32 index)
{
    m_pInsert->SetTag20_2(word, index);
}

// 0x005F1E34
void Phrase::SetTag20_0(const IWord& word, u32 index)
{
    m_pInsert->SetTag20_0(word, index);
}

// 0x005F1E44
void Phrase::SetTag20_3(const IWord& word)
{
    m_pInsert->SetTag20_3(word);
}

// 0x005F1E54
void Phrase::SetTag20_1(const IWord& word, u32 index)
{
    m_pInsert->SetTag20_1(word, index);
}
// 0x002FEAF8
void Phrase::SetWord2C0(const IWord& word, u32 index)
{
    m_pInsert->SetWord2C0(word, index);
}

// 0x00300894
void Phrase::SetMonth(s32 month, u32 index)
{
    m_pInsert->SetMonth(month, index);
}

// 0x003008A4
void Phrase::SetDay(s32 day, u32 index)
{
    m_pInsert->SetDay(day, index);
}

// 0x00300B6C
void Phrase::SetWord2B4(const IWord& word, u32 index)
{
    m_pInsert->SetWord2B4(word, index);
}

// 0x00312D28
void Phrase::SetTag4_6(const WordNumDigit& number, u32 index)
{
    m_pInsert->SetTag4_6(number, index);
}

} // namespace script
