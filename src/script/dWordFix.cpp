#include "script/dWordFix.h"
#include "script/dWordKey.h"

// The WordFix and WordKey instances (one copy of each member per size; the constructors only where ARMCC made a
// copy, the other users have them inline; name of the file is ours).

namespace script {
// 0x005F56CC (name is ours)
WordInfo::WordInfo() : color(4), begin(-1), buf(-1)
{
}

// 0x005F565C (name is ours)
WordInfo& WordInfo::operator=(const WordInfo& info)
{
    color = info.color;
    begin = info.begin;
    buf = info.buf;
    return *this;
}

// 0x005F56AC (name is ours)
void WordInfo::Reset()
{
    color = 4;
    begin = -1;
    buf = -1;
}

// 0x005F56C4 (name is ours)
void WordInfo::SetColor(u8 c)
{
    color = c;
}

// 0x0081E1C0 slot 0x00
// 0x0081E1B0 (deleting dtor)
template script::WordFix<1u>::~WordFix();
// 0x0081E13C slot 0x14
template void script::WordFix<1u>::Copy(const IWord& src);
// 0x0081E18C slot 0x1C
template void script::WordFix<1u>::Clear();
// 0x0081E134 slot 0x24
template WordInfo* script::WordFix<1u>::GetInfo();
// 0x0082DAA8 slot 0x28
template const WordInfo* script::WordFix<1u>::GetInfo() const;

// 0x0081E814 slot 0x00
// 0x0081E804 (deleting dtor)
template script::WordFix<2u>::~WordFix();
// 0x0081E790 slot 0x14
template void script::WordFix<2u>::Copy(const IWord& src);
// 0x0081E7E0 slot 0x1C
template void script::WordFix<2u>::Clear();
// 0x0081E788 slot 0x24
template WordInfo* script::WordFix<2u>::GetInfo();
// 0x0082DAE8 slot 0x28
template const WordInfo* script::WordFix<2u>::GetInfo() const;

// 0x0081F050
template <>
WordFix<4u>::WordFix() : WordPtr(m_Buffer, 4)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 4; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081F0B4 slot 0x00
// 0x0081F0A4 (deleting dtor)
template script::WordFix<4u>::~WordFix();
// 0x0081EFF4 slot 0x14
template void script::WordFix<4u>::Copy(const IWord& src);
// 0x0081F030 slot 0x1C
template void script::WordFix<4u>::Clear();
// 0x0081EFEC slot 0x24
template WordInfo* script::WordFix<4u>::GetInfo();
// 0x0082DB48 slot 0x28
template const WordInfo* script::WordFix<4u>::GetInfo() const;

// 0x0081F8B4 slot 0x00
// 0x0081F890 (deleting dtor)
template script::WordFix<8u>::~WordFix();
// 0x0081F83C slot 0x14
template void script::WordFix<8u>::Copy(const IWord& src);
// 0x0081F878 slot 0x1C
template void script::WordFix<8u>::Clear();
// 0x0081F834 slot 0x24
template WordInfo* script::WordFix<8u>::GetInfo();
// 0x0082DBA0 slot 0x28
template const WordInfo* script::WordFix<8u>::GetInfo() const;

// 0x0081F9D0
template <>
WordFix<9u>::WordFix() : WordPtr(m_Buffer, 9)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 9; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0030094C slot 0x00
// 0x0081FA38 (deleting dtor)
template script::WordFix<9u>::~WordFix();
// 0x0081F97C slot 0x14
template void script::WordFix<9u>::Copy(const IWord& src);
// 0x0081F9B8 slot 0x1C
template void script::WordFix<9u>::Clear();
// 0x0081F974 slot 0x24
template WordInfo* script::WordFix<9u>::GetInfo();
// 0x0082DBB0 slot 0x28
template const WordInfo* script::WordFix<9u>::GetInfo() const;

// 0x0081D678 slot 0x00
// 0x0081D654 (deleting dtor)
template script::WordFix<10u>::~WordFix();
// 0x0081D600 slot 0x14
template void script::WordFix<10u>::Copy(const IWord& src);
// 0x0081D63C slot 0x1C
template void script::WordFix<10u>::Clear();
// 0x0081D5F8 slot 0x24
template WordInfo* script::WordFix<10u>::GetInfo();
// 0x0082DA20 slot 0x28
template const WordInfo* script::WordFix<10u>::GetInfo() const;

// 0x0081D794
template <>
WordFix<11u>::WordFix() : WordPtr(m_Buffer, 11)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 11; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081D80C slot 0x00
// 0x0081D7FC (deleting dtor)
template script::WordFix<11u>::~WordFix();
// 0x0081D740 slot 0x14
template void script::WordFix<11u>::Copy(const IWord& src);
// 0x0081D77C slot 0x1C
template void script::WordFix<11u>::Clear();
// 0x0081D738 slot 0x24
template WordInfo* script::WordFix<11u>::GetInfo();
// 0x0082DA30 slot 0x28
template const WordInfo* script::WordFix<11u>::GetInfo() const;

// 0x0081D9B8
template <>
WordFix<12u>::WordFix() : WordPtr(m_Buffer, 12)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 12; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081DA30 slot 0x00
// 0x0081DA20 (deleting dtor)
template script::WordFix<12u>::~WordFix();
// 0x0081D964 slot 0x14
template void script::WordFix<12u>::Copy(const IWord& src);
// 0x0081D9A0 slot 0x1C
template void script::WordFix<12u>::Clear();
// 0x0081D95C slot 0x24
template WordInfo* script::WordFix<12u>::GetInfo();
// 0x0082DA48 slot 0x28
template const WordInfo* script::WordFix<12u>::GetInfo() const;

// 0x0081DABC
template <>
WordFix<13u>::WordFix() : WordPtr(m_Buffer, 13)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 13; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081DB34 slot 0x00
// 0x0081DB24 (deleting dtor)
template script::WordFix<13u>::~WordFix();
// 0x0081DA48 slot 0x14
template void script::WordFix<13u>::Copy(const IWord& src);
// 0x0081DA98 slot 0x1C
template void script::WordFix<13u>::Clear();
// 0x0081DA40 slot 0x24
template WordInfo* script::WordFix<13u>::GetInfo();
// 0x0082DA50 slot 0x28
template const WordInfo* script::WordFix<13u>::GetInfo() const;

// 0x0081DBC4 slot 0x00
// 0x0081DBA0 (deleting dtor)
template script::WordFix<14u>::~WordFix();
// 0x0081DB4C slot 0x14
template void script::WordFix<14u>::Copy(const IWord& src);
// 0x0081DB88 slot 0x1C
template void script::WordFix<14u>::Clear();
// 0x0081DB44 slot 0x24
template WordInfo* script::WordFix<14u>::GetInfo();
// 0x0082DA58 slot 0x28
template const WordInfo* script::WordFix<14u>::GetInfo() const;

// 0x0081DCEC slot 0x00
// 0x0081DCDC (deleting dtor)
template script::WordFix<15u>::~WordFix();
// 0x0081DC88 slot 0x14
template void script::WordFix<15u>::Copy(const IWord& src);
// 0x0081DCC4 slot 0x1C
template void script::WordFix<15u>::Clear();
// 0x0081DC80 slot 0x24
template WordInfo* script::WordFix<15u>::GetInfo();
// 0x0082DA68 slot 0x28
template const WordInfo* script::WordFix<15u>::GetInfo() const;

// 0x0081DD7C slot 0x00
// 0x0081DD58 (deleting dtor)
template script::WordFix<16u>::~WordFix();
// 0x0081DD04 slot 0x14
template void script::WordFix<16u>::Copy(const IWord& src);
// 0x0081DD40 slot 0x1C
template void script::WordFix<16u>::Clear();
// 0x0081DCFC slot 0x24
template WordInfo* script::WordFix<16u>::GetInfo();
// 0x0082DA70 slot 0x28
template const WordInfo* script::WordFix<16u>::GetInfo() const;

// 0x0081DE28 slot 0x00
// 0x0081DE18 (deleting dtor)
template script::WordFix<17u>::~WordFix();
// 0x0081DDA4 slot 0x14
template void script::WordFix<17u>::Copy(const IWord& src);
// 0x0081DDF4 slot 0x1C
template void script::WordFix<17u>::Clear();
// 0x0081DD9C slot 0x24
template WordInfo* script::WordFix<17u>::GetInfo();
// 0x0082DA78 slot 0x28
template const WordInfo* script::WordFix<17u>::GetInfo() const;

// 0x0081DF58 slot 0x00
// 0x0081DF34 (deleting dtor)
template script::WordFix<18u>::~WordFix();
// 0x0081DEE0 slot 0x14
template void script::WordFix<18u>::Copy(const IWord& src);
// 0x0081DF1C slot 0x1C
template void script::WordFix<18u>::Clear();
// 0x0081DED8 slot 0x24
template WordInfo* script::WordFix<18u>::GetInfo();
// 0x0082DA88 slot 0x28
template const WordInfo* script::WordFix<18u>::GetInfo() const;

// 0x0081E124 slot 0x00
// 0x0081E114 (deleting dtor)
template script::WordFix<19u>::~WordFix();
// 0x0081E0C0 slot 0x14
template void script::WordFix<19u>::Copy(const IWord& src);
// 0x0081E0FC slot 0x1C
template void script::WordFix<19u>::Clear();
// 0x0081E0B8 slot 0x24
template WordInfo* script::WordFix<19u>::GetInfo();
// 0x0082DAA0 slot 0x28
template const WordInfo* script::WordFix<19u>::GetInfo() const;

// 0x0081E22C
template <>
WordFix<20u>::WordFix() : WordPtr(m_Buffer, 20)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 20; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081E2CC slot 0x00
// 0x0081E2A8 (deleting dtor)
template script::WordFix<20u>::~WordFix();
// 0x0081E1D8 slot 0x14
template void script::WordFix<20u>::Copy(const IWord& src);
// 0x0081E214 slot 0x1C
template void script::WordFix<20u>::Clear();
// 0x0081E1D0 slot 0x24
template WordInfo* script::WordFix<20u>::GetInfo();
// 0x0082DAB0 slot 0x28
template const WordInfo* script::WordFix<20u>::GetInfo() const;

// 0x0081E348
template <>
WordFix<21u>::WordFix() : WordPtr(m_Buffer, 21)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 21; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081E3D4 slot 0x00
// 0x0081E3B0 (deleting dtor)
template script::WordFix<21u>::~WordFix();
// 0x0081E2F4 slot 0x14
template void script::WordFix<21u>::Copy(const IWord& src);
// 0x0081E330 slot 0x1C
template void script::WordFix<21u>::Clear();
// 0x0081E2EC slot 0x24
template WordInfo* script::WordFix<21u>::GetInfo();
// 0x0082DAB8 slot 0x28
template const WordInfo* script::WordFix<21u>::GetInfo() const;

// 0x0081E470
template <>
WordFix<22u>::WordFix() : WordPtr(m_Buffer, 22)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 22; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081E4FC slot 0x00
// 0x0081E4EC (deleting dtor)
template script::WordFix<22u>::~WordFix();
// 0x0081E3FC slot 0x14
template void script::WordFix<22u>::Copy(const IWord& src);
// 0x0081E44C slot 0x1C
template void script::WordFix<22u>::Clear();
// 0x0081E3F4 slot 0x24
template WordInfo* script::WordFix<22u>::GetInfo();
// 0x0082DAC0 slot 0x28
template const WordInfo* script::WordFix<22u>::GetInfo() const;

// 0x0081E58C slot 0x00
// 0x0081E568 (deleting dtor)
template script::WordFix<24u>::~WordFix();
// 0x0081E514 slot 0x14
template void script::WordFix<24u>::Copy(const IWord& src);
// 0x0081E550 slot 0x1C
template void script::WordFix<24u>::Clear();
// 0x0081E50C slot 0x24
template WordInfo* script::WordFix<24u>::GetInfo();
// 0x0082DAC8 slot 0x28
template const WordInfo* script::WordFix<24u>::GetInfo() const;

// 0x0081E6CC slot 0x00
// 0x0081E6A8 (deleting dtor)
template script::WordFix<25u>::~WordFix();
// 0x0081E654 slot 0x14
template void script::WordFix<25u>::Copy(const IWord& src);
// 0x0081E690 slot 0x1C
template void script::WordFix<25u>::Clear();
// 0x0081E64C slot 0x24
template WordInfo* script::WordFix<25u>::GetInfo();
// 0x0082DAD8 slot 0x28
template const WordInfo* script::WordFix<25u>::GetInfo() const;

// 0x0081E778 slot 0x00
// 0x0081E768 (deleting dtor)
template script::WordFix<29u>::~WordFix();
// 0x0081E6F4 slot 0x14
template void script::WordFix<29u>::Copy(const IWord& src);
// 0x0081E744 slot 0x1C
template void script::WordFix<29u>::Clear();
// 0x0081E6EC slot 0x24
template WordInfo* script::WordFix<29u>::GetInfo();
// 0x0082DAE0 slot 0x28
template const WordInfo* script::WordFix<29u>::GetInfo() const;

// 0x0081E8A4 slot 0x00
// 0x0081E880 (deleting dtor)
template script::WordFix<30u>::~WordFix();
// 0x0081E82C slot 0x14
template void script::WordFix<30u>::Copy(const IWord& src);
// 0x0081E868 slot 0x1C
template void script::WordFix<30u>::Clear();
// 0x0081E824 slot 0x24
template WordInfo* script::WordFix<30u>::GetInfo();
// 0x0082DAF0 slot 0x28
template const WordInfo* script::WordFix<30u>::GetInfo() const;

// 0x0081E944 slot 0x00
// 0x0081E920 (deleting dtor)
template script::WordFix<32u>::~WordFix();
// 0x0081E8CC slot 0x14
template void script::WordFix<32u>::Copy(const IWord& src);
// 0x0081E908 slot 0x1C
template void script::WordFix<32u>::Clear();
// 0x0081E8C4 slot 0x24
template WordInfo* script::WordFix<32u>::GetInfo();
// 0x0082DAF8 slot 0x28
template const WordInfo* script::WordFix<32u>::GetInfo() const;

// 0x0081E9C0
template <>
WordFix<33u>::WordFix() : WordPtr(m_Buffer, 33)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 33; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x002FEB08 slot 0x00
// 0x0081EA28 (deleting dtor)
template script::WordFix<33u>::~WordFix();
// 0x0081E96C slot 0x14
template void script::WordFix<33u>::Copy(const IWord& src);
// 0x0081E9A8 slot 0x1C
template void script::WordFix<33u>::Clear();
// 0x0081E964 slot 0x24
template WordInfo* script::WordFix<33u>::GetInfo();
// 0x0082DB00 slot 0x28
template const WordInfo* script::WordFix<33u>::GetInfo() const;

// 0x0081EB68
template <>
WordFix<36u>::WordFix() : WordPtr(m_Buffer, 36)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 36; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081EBDC slot 0x00
// 0x0081EBCC (deleting dtor)
template script::WordFix<36u>::~WordFix();
// 0x0081EAF4 slot 0x14
template void script::WordFix<36u>::Copy(const IWord& src);
// 0x0081EB44 slot 0x1C
template void script::WordFix<36u>::Clear();
// 0x0081EAEC slot 0x24
template WordInfo* script::WordFix<36u>::GetInfo();
// 0x0082DB10 slot 0x28
template const WordInfo* script::WordFix<36u>::GetInfo() const;

// 0x0030505C slot 0x00
// 0x0081EC48 (deleting dtor)
template script::WordFix<38u>::~WordFix();
// 0x0081EBF4 slot 0x14
template void script::WordFix<38u>::Copy(const IWord& src);
// 0x0081EC30 slot 0x1C
template void script::WordFix<38u>::Clear();
// 0x0081EBEC slot 0x24
template WordInfo* script::WordFix<38u>::GetInfo();
// 0x0082DB18 slot 0x28
template const WordInfo* script::WordFix<38u>::GetInfo() const;

// 0x0081ED88 slot 0x00
// 0x0081ED64 (deleting dtor)
template script::WordFix<40u>::~WordFix();
// 0x0081ED10 slot 0x14
template void script::WordFix<40u>::Copy(const IWord& src);
// 0x0081ED4C slot 0x1C
template void script::WordFix<40u>::Clear();
// 0x0081ED08 slot 0x24
template WordInfo* script::WordFix<40u>::GetInfo();
// 0x0082DB28 slot 0x28
template const WordInfo* script::WordFix<40u>::GetInfo() const;

// 0x0081EE28 slot 0x00
// 0x0081EE04 (deleting dtor)
template script::WordFix<41u>::~WordFix();
// 0x0081EDB0 slot 0x14
template void script::WordFix<41u>::Copy(const IWord& src);
// 0x0081EDEC slot 0x1C
template void script::WordFix<41u>::Clear();
// 0x0081EDA8 slot 0x24
template WordInfo* script::WordFix<41u>::GetInfo();
// 0x0082DB30 slot 0x28
template const WordInfo* script::WordFix<41u>::GetInfo() const;

// 0x0081EF64
template <>
WordFix<45u>::WordFix() : WordPtr(m_Buffer, 45)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 45; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081EFDC slot 0x00
// 0x0081EFCC (deleting dtor)
template script::WordFix<45u>::~WordFix();
// 0x0081EEF0 slot 0x14
template void script::WordFix<45u>::Copy(const IWord& src);
// 0x0081EF40 slot 0x1C
template void script::WordFix<45u>::Clear();
// 0x0081EEE8 slot 0x24
template WordInfo* script::WordFix<45u>::GetInfo();
// 0x0082DB40 slot 0x28
template const WordInfo* script::WordFix<45u>::GetInfo() const;

// 0x0081F120
template <>
WordFix<50u>::WordFix() : WordPtr(m_Buffer, 50)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 50; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081F1A4 slot 0x00
// 0x0081F180 (deleting dtor)
template script::WordFix<50u>::~WordFix();
// 0x0081F0CC slot 0x14
template void script::WordFix<50u>::Copy(const IWord& src);
// 0x0081F108 slot 0x1C
template void script::WordFix<50u>::Clear();
// 0x0081F0C4 slot 0x24
template WordInfo* script::WordFix<50u>::GetInfo();
// 0x0082DB50 slot 0x28
template const WordInfo* script::WordFix<50u>::GetInfo() const;

// 0x0081F2F0 slot 0x00
// 0x0081F2E0 (deleting dtor)
template script::WordFix<57u>::~WordFix();
// 0x0081F26C slot 0x14
template void script::WordFix<57u>::Copy(const IWord& src);
// 0x0081F2BC slot 0x1C
template void script::WordFix<57u>::Clear();
// 0x0081F264 slot 0x24
template WordInfo* script::WordFix<57u>::GetInfo();
// 0x0082DB60 slot 0x28
template const WordInfo* script::WordFix<57u>::GetInfo() const;

// 0x0081F35C
template <>
WordFix<60u>::WordFix() : WordPtr(m_Buffer, 60)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 60; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081F3E0 slot 0x00
// 0x0081F3BC (deleting dtor)
template script::WordFix<60u>::~WordFix();
// 0x0081F308 slot 0x14
template void script::WordFix<60u>::Copy(const IWord& src);
// 0x0081F344 slot 0x1C
template void script::WordFix<60u>::Clear();
// 0x0081F300 slot 0x24
template WordInfo* script::WordFix<60u>::GetInfo();
// 0x0082DB68 slot 0x28
template const WordInfo* script::WordFix<60u>::GetInfo() const;

// 0x0081F45C
template <>
WordFix<64u>::WordFix() : WordPtr(m_Buffer, 64)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 64; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081F4CC slot 0x00
// 0x0081F4BC (deleting dtor)
template script::WordFix<64u>::~WordFix();
// 0x0081F408 slot 0x14
template void script::WordFix<64u>::Copy(const IWord& src);
// 0x0081F444 slot 0x1C
template void script::WordFix<64u>::Clear();
// 0x0081F400 slot 0x24
template WordInfo* script::WordFix<64u>::GetInfo();
// 0x0082DB70 slot 0x28
template const WordInfo* script::WordFix<64u>::GetInfo() const;

// 0x0081F548 slot 0x00
// 0x0081F538 (deleting dtor)
template script::WordFix<65u>::~WordFix();
// 0x0081F4E4 slot 0x14
template void script::WordFix<65u>::Copy(const IWord& src);
// 0x0081F520 slot 0x1C
template void script::WordFix<65u>::Clear();
// 0x0081F4DC slot 0x24
template WordInfo* script::WordFix<65u>::GetInfo();
// 0x0082DB78 slot 0x28
template const WordInfo* script::WordFix<65u>::GetInfo() const;

// 0x0081F5D8 slot 0x00
// 0x0081F5B4 (deleting dtor)
template script::WordFix<69u>::~WordFix();
// 0x0081F560 slot 0x14
template void script::WordFix<69u>::Copy(const IWord& src);
// 0x0081F59C slot 0x1C
template void script::WordFix<69u>::Clear();
// 0x0081F558 slot 0x24
template WordInfo* script::WordFix<69u>::GetInfo();
// 0x0082DB80 slot 0x28
template const WordInfo* script::WordFix<69u>::GetInfo() const;

// 0x0081F65C
template <>
WordFix<70u>::WordFix() : WordPtr(m_Buffer, 70)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 70; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081F6D8 slot 0x00
// 0x0081F6B4 (deleting dtor)
template script::WordFix<70u>::~WordFix();
// 0x0081F600 slot 0x14
template void script::WordFix<70u>::Copy(const IWord& src);
// 0x0081F63C slot 0x1C
template void script::WordFix<70u>::Clear();
// 0x0081F5F8 slot 0x24
template WordInfo* script::WordFix<70u>::GetInfo();
// 0x0082DB88 slot 0x28
template const WordInfo* script::WordFix<70u>::GetInfo() const;

// 0x0081F778 slot 0x00
// 0x0081F754 (deleting dtor)
template script::WordFix<75u>::~WordFix();
// 0x0081F700 slot 0x14
template void script::WordFix<75u>::Copy(const IWord& src);
// 0x0081F73C slot 0x1C
template void script::WordFix<75u>::Clear();
// 0x0081F6F8 slot 0x24
template WordInfo* script::WordFix<75u>::GetInfo();
// 0x0082DB90 slot 0x28
template const WordInfo* script::WordFix<75u>::GetInfo() const;

// 0x0081F824 slot 0x00
// 0x0081F814 (deleting dtor)
template script::WordFix<85u>::~WordFix();
// 0x0081F7A0 slot 0x14
template void script::WordFix<85u>::Copy(const IWord& src);
// 0x0081F7F0 slot 0x1C
template void script::WordFix<85u>::Clear();
// 0x0081F798 slot 0x24
template WordInfo* script::WordFix<85u>::GetInfo();
// 0x0082DB98 slot 0x28
template const WordInfo* script::WordFix<85u>::GetInfo() const;

// 0x0081F954 slot 0x00
// 0x0081F930 (deleting dtor)
template script::WordFix<94u>::~WordFix();
// 0x0081F8DC slot 0x14
template void script::WordFix<94u>::Copy(const IWord& src);
// 0x0081F918 slot 0x1C
template void script::WordFix<94u>::Clear();
// 0x0081F8D4 slot 0x24
template WordInfo* script::WordFix<94u>::GetInfo();
// 0x0082DBA8 slot 0x28
template const WordInfo* script::WordFix<94u>::GetInfo() const;

// 0x0081D4D4 slot 0x00
// 0x0081D4B0 (deleting dtor)
template script::WordFix<100u>::~WordFix();
// 0x005F5670 slot 0x14
template void script::WordFix<100u>::Copy(const IWord& src);
// 0x0081D498 slot 0x1C
template void script::WordFix<100u>::Clear();
// 0x0081D490 slot 0x24
template WordInfo* script::WordFix<100u>::GetInfo();
// 0x0082DA10 slot 0x28
template const WordInfo* script::WordFix<100u>::GetInfo() const;

// 0x0081D570
template <>
WordFix<109u>::WordFix() : WordPtr(m_Buffer, 109)
{
    std::memset(m_Buffer, 0, sizeof(m_Buffer));
    for (u32 i = 0; i < 109; i++) {
        m_Buffer[i] = WORD_NULL_CHAR;
    }
}
// 0x0081D5E8 slot 0x00
// 0x0081D5D8 (deleting dtor)
template script::WordFix<109u>::~WordFix();
// 0x0081D4FC slot 0x14
template void script::WordFix<109u>::Copy(const IWord& src);
// 0x0081D54C slot 0x1C
template void script::WordFix<109u>::Clear();
// 0x0081D4F4 slot 0x24
template WordInfo* script::WordFix<109u>::GetInfo();
// 0x0082DA18 slot 0x28
template const WordInfo* script::WordFix<109u>::GetInfo() const;

// 0x0081D718 slot 0x00
// 0x0081D6F4 (deleting dtor)
template script::WordFix<111u>::~WordFix();
// 0x0081D6A0 slot 0x14
template void script::WordFix<111u>::Copy(const IWord& src);
// 0x0081D6DC slot 0x1C
template void script::WordFix<111u>::Clear();
// 0x0081D698 slot 0x24
template WordInfo* script::WordFix<111u>::GetInfo();
// 0x0082DA28 slot 0x28
template const WordInfo* script::WordFix<111u>::GetInfo() const;

// 0x0081D89C slot 0x00
// 0x0081D878 (deleting dtor)
template script::WordFix<120u>::~WordFix();
// 0x0081D824 slot 0x14
template void script::WordFix<120u>::Copy(const IWord& src);
// 0x0081D860 slot 0x1C
template void script::WordFix<120u>::Clear();
// 0x0081D81C slot 0x24
template WordInfo* script::WordFix<120u>::GetInfo();
// 0x0082DA38 slot 0x28
template const WordInfo* script::WordFix<120u>::GetInfo() const;

// 0x0081D93C slot 0x00
// 0x0081D918 (deleting dtor)
template script::WordFix<128u>::~WordFix();
// 0x0081D8C4 slot 0x14
template void script::WordFix<128u>::Copy(const IWord& src);
// 0x0081D900 slot 0x1C
template void script::WordFix<128u>::Clear();
// 0x0081D8BC slot 0x24
template WordInfo* script::WordFix<128u>::GetInfo();
// 0x0082DA40 slot 0x28
template const WordInfo* script::WordFix<128u>::GetInfo() const;

// 0x0081DEB8 slot 0x00
// 0x0081DE94 (deleting dtor)
template script::WordFix<188u>::~WordFix();
// 0x0081DE40 slot 0x14
template void script::WordFix<188u>::Copy(const IWord& src);
// 0x0081DE7C slot 0x1C
template void script::WordFix<188u>::Clear();
// 0x0081DE38 slot 0x24
template WordInfo* script::WordFix<188u>::GetInfo();
// 0x0082DA80 slot 0x28
template const WordInfo* script::WordFix<188u>::GetInfo() const;

// 0x0081DFF8 slot 0x00
// 0x0081DFD4 (deleting dtor)
template script::WordFix<193u>::~WordFix();
// 0x0081DF80 slot 0x14
template void script::WordFix<193u>::Copy(const IWord& src);
// 0x0081DFBC slot 0x1C
template void script::WordFix<193u>::Clear();
// 0x0081DF78 slot 0x24
template WordInfo* script::WordFix<193u>::GetInfo();
// 0x0082DA90 slot 0x28
template const WordInfo* script::WordFix<193u>::GetInfo() const;

// 0x0081E098 slot 0x00
// 0x0081E074 (deleting dtor)
template script::WordFix<197u>::~WordFix();
// 0x0081E020 slot 0x14
template void script::WordFix<197u>::Copy(const IWord& src);
// 0x0081E05C slot 0x1C
template void script::WordFix<197u>::Clear();
// 0x0081E018 slot 0x24
template WordInfo* script::WordFix<197u>::GetInfo();
// 0x0082DA98 slot 0x28
template const WordInfo* script::WordFix<197u>::GetInfo() const;

// 0x0081E62C slot 0x00
// 0x0081E608 (deleting dtor)
template script::WordFix<256u>::~WordFix();
// 0x0081E5B4 slot 0x14
template void script::WordFix<256u>::Copy(const IWord& src);
// 0x0081E5F0 slot 0x1C
template void script::WordFix<256u>::Clear();
// 0x0081E5AC slot 0x24
template WordInfo* script::WordFix<256u>::GetInfo();
// 0x0082DAD0 slot 0x28
template const WordInfo* script::WordFix<256u>::GetInfo() const;

// 0x0081EACC slot 0x00
// 0x0081EAA8 (deleting dtor)
template script::WordFix<350u>::~WordFix();
// 0x0081EA54 slot 0x14
template void script::WordFix<350u>::Copy(const IWord& src);
// 0x0081EA90 slot 0x1C
template void script::WordFix<350u>::Clear();
// 0x0081EA4C slot 0x24
template WordInfo* script::WordFix<350u>::GetInfo();
// 0x0082DB08 slot 0x28
template const WordInfo* script::WordFix<350u>::GetInfo() const;

// 0x0081ECF8 slot 0x00
// 0x0081ECE8 (deleting dtor)
template script::WordFix<400u>::~WordFix();
// 0x0081EC74 slot 0x14
template void script::WordFix<400u>::Copy(const IWord& src);
// 0x0081ECC4 slot 0x1C
template void script::WordFix<400u>::Clear();
// 0x0081EC6C slot 0x24
template WordInfo* script::WordFix<400u>::GetInfo();
// 0x0082DB20 slot 0x28
template const WordInfo* script::WordFix<400u>::GetInfo() const;

// 0x0081EEC8 slot 0x00
// 0x0081EEA4 (deleting dtor)
template script::WordFix<450u>::~WordFix();
// 0x0081EE50 slot 0x14
template void script::WordFix<450u>::Copy(const IWord& src);
// 0x0081EE8C slot 0x1C
template void script::WordFix<450u>::Clear();
// 0x0081EE48 slot 0x24
template WordInfo* script::WordFix<450u>::GetInfo();
// 0x0082DB38 slot 0x28
template const WordInfo* script::WordFix<450u>::GetInfo() const;

// 0x0081F244 slot 0x00
// 0x0081F220 (deleting dtor)
template script::WordFix<512u>::~WordFix();
// 0x0081F1CC slot 0x14
template void script::WordFix<512u>::Copy(const IWord& src);
// 0x0081F208 slot 0x1C
template void script::WordFix<512u>::Clear();
// 0x0081F1C4 slot 0x24
template WordInfo* script::WordFix<512u>::GetInfo();
// 0x0082DB58 slot 0x28
template const WordInfo* script::WordFix<512u>::GetInfo() const;

// 0x0081DC70 slot 0x00
// 0x0081DC60 (deleting dtor)
template script::WordFix<1536u>::~WordFix();
// 0x0081DBEC slot 0x14
template void script::WordFix<1536u>::Copy(const IWord& src);
// 0x0081DC3C slot 0x1C
template void script::WordFix<1536u>::Clear();
// 0x0081DBE4 slot 0x24
template WordInfo* script::WordFix<1536u>::GetInfo();
// 0x0082DA60 slot 0x28
template const WordInfo* script::WordFix<1536u>::GetInfo() const;

// 0x0081FD60
template script::WordKey<9u, int>::WordKey();
// 0x0081FD90 slot 0x00
// 0x0081FD80 (deleting dtor)
template script::WordKey<9u, int>::~WordKey();
// 0x0081FD30 slot 0x1C
template void script::WordKey<9u, int>::Clear();

// 0x0081FA8C
template script::WordKey<29u, int>::WordKey();
// 0x0081FB18 slot 0x00
// 0x0081FB08 (deleting dtor)
template script::WordKey<29u, int>::~WordKey();
// 0x0081FA5C slot 0x1C
template void script::WordKey<29u, int>::Clear();

// 0x0081FB58
template script::WordKey<45u, int>::WordKey();
// 0x0081FB88 slot 0x00
// 0x0081FB78 (deleting dtor)
template script::WordKey<45u, int>::~WordKey();
// 0x0081FB28 slot 0x1C
template void script::WordKey<45u, int>::Clear();

// 0x0081FBC8
template script::WordKey<57u, int>::WordKey();
// 0x0081FC54 slot 0x00
// 0x0081FC44 (deleting dtor)
template script::WordKey<57u, int>::~WordKey();
// 0x0081FB98 slot 0x1C
template void script::WordKey<57u, int>::Clear();

// 0x0081FC94
template script::WordKey<85u, int>::WordKey();
// 0x0081FD20 slot 0x00
// 0x0081FD10 (deleting dtor)
template script::WordKey<85u, int>::~WordKey();
// 0x0081FC64 slot 0x1C
template void script::WordKey<85u, int>::Clear();

} // namespace script
