#pragma once

#include "decomp.h"

namespace script {
class Insert;
class IWord;

// The phrase of a message: the game sets the inserted words through it (the functions pass the values to the
// insert). Function names after the slots of Insert (ours).
// RTTI N6script6PhraseE @ 0x008D3AA4
class Phrase
{
public:
    // (inline)
    explicit Phrase(Insert* pInsert) : m_pInsert(pInsert) {}
    virtual ~Phrase() {}

    void SetWord2AC(const IWord& word, u32 index, u32 subIndex); // 0x005F1C84
    void ClearNumber(u32 index); // 0x005F1C94
    void ClearTag5_7(); // 0x005F1CA4
    void SetTag4_17(u32 minute, u32 index); // 0x005F1CB4
    void SetTag5_32(const IWord& word, u32 index); // 0x005F1CC4
    void SetTag4_19(u32 value, u32 index); // 0x005F1CD4
    void SetTag4_16(u32 hour, u32 index); // 0x005F1CE4
    void SetTag4_13(s32 value, u32 index); // 0x005F1CF4
    void ClearTag4_18(); // 0x005F1D04
    void ClearDay(u32 index); // 0x005F1D14
    void ClearYear(u32 index); // 0x005F1D24
    void ClearWord238(); // 0x005F1D34
    void SetTag4_20(u32 value, u32 index); // 0x005F1D44
    void ClearMonth(u32 index); // 0x005F1D54
    void SetWord2B8(const IWord& word, u32 index); // 0x005F1D64
    void ClearExtra(); // 0x005F1D74
    void SetTag5_35(const IWord& word); // 0x005F1D84
    void SetTag0_4(u64 value); // 0x005F1D94
    void SetTag17_8(const IWord& word); // 0x005F1DA4
    void SetWord2CC(const IWord& word, u32 index); // 0x005F1DB4
    void SetWord2BC(const IWord& word, u32 index); // 0x005F1DC4
    void SetWord2DC(const IWord& word, u32 index); // 0x005F1DD4
    void ClearTag5_12(); // 0x005F1DE4
    void SetTag17_4(const IWord& word, u32 index); // 0x005F1DF4
    void SetWord2D8(const IWord& word, u32 index); // 0x005F1E04
    void SetTag17_5(const IWord& word); // 0x005F1E14
    void SetTag20_2(const IWord& word, u32 index); // 0x005F1E24
    void SetTag20_0(const IWord& word, u32 index); // 0x005F1E34
    void SetTag20_3(const IWord& word); // 0x005F1E44
    void SetTag20_1(const IWord& word, u32 index); // 0x005F1E54
    // (inline copy)
    Insert* GetInsert() { return m_pInsert; } // 0x005F1E64

    Insert* m_pInsert; // 0x4
};
} // namespace script
