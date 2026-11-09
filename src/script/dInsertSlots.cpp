#include "script/dInsert.h"

namespace script {
// 0x005ED58C slot 0x00C
void Insert::ClearNumber(u32 index)
{
    m_Tag4_6[index].Clear();
}

// 0x005EEC70 slot 0x010
void Insert::ClearYear(u32 index)
{
    m_Tag4_13[index].Clear();
}

// 0x005EEFB0 slot 0x014
void Insert::ClearMonth(u32 index)
{
    m_Month[index].Clear();
}

// 0x005EE8E0 slot 0x018
void Insert::ClearDay(u32 index)
{
    m_Day[index].Clear();
}

// 0x005EEC58 slot 0x01C
void Insert::ClearTag4_16(u32 index)
{
    m_Tag4_16[index].Clear();
}

// 0x005EE8F8 slot 0x020
void Insert::ClearTag4_17(u32 index)
{
    m_Tag4_17[index].Clear();
}

// 0x005EE8C4 slot 0x024
void Insert::ClearTag4_18()
{
    m_Tag4_18.Clear();
}

// 0x005EEC38 slot 0x028
void Insert::ClearTag4_19(u32 index)
{
    m_Tag4_19[index].Clear();
}

// 0x005EF444 slot 0x02C
void Insert::ClearTag4_20(u32 index)
{
    m_Tag4_20[index].Clear();
}

// 0x005ED5B4 slot 0x030
void Insert::ClearTag5_0()
{
    m_Tag5_0.Clear();
}

// 0x005ED5C8 slot 0x034
void Insert::ClearTag5_6()
{
    m_Tag5_6.Clear();
}

// 0x005ED5A0 slot 0x038
void Insert::ClearTag5_7()
{
    m_Tag5_7.Clear();
}

// 0x005EEC84 slot 0x03C
void Insert::ClearWord238()
{
    m_Word238.Clear();
}

// 0x005EFDC4 slot 0x040
void Insert::ClearConstellation()
{
    m_Constellation.Clear();
}

// 0x005EFDA8 slot 0x044
void Insert::ClearTag5_12()
{
    m_Tag5_12.Clear();
}

// 0x005ED51C slot 0x06C
void Insert::SetTag5_0(const IWord& word)
{
    m_Tag5_0.Copy(word);
}

// 0x005ED508 slot 0x070
void Insert::SetTag5_5(const IWord& word)
{
    m_Tag5_5.Copy(word);
}

// 0x005ED530 slot 0x074
void Insert::SetTag5_6(const IWord& word)
{
    m_Tag5_6.Copy(word);
}

// 0x005EF460 slot 0x078
void Insert::SetTag5_35(const IWord& word)
{
    m_Tag5_35.Copy(word);
}

// 0x005EF024 slot 0x07C
void Insert::SetTag5_37(const IWord& word)
{
    m_Tag5_37.Copy(word);
}

// 0x005ED69C slot 0x080
void Insert::SetTag5_32(const IWord& word, u32 index)
{
    if (index < 23) {
        m_Tag5_32[index].Copy(word);
    }
}

// 0x005EEFF8 slot 0x088
void Insert::SetWord2B4(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2B4[index].Copy(word);
    }
}

// 0x005EEFCC slot 0x08C
void Insert::SetWord2B8(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2B8[index].Copy(word);
    }
}

// 0x005EF94C slot 0x090
void Insert::SetWord2BC(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2BC[index].Copy(word);
    }
}

// 0x005EF920 slot 0x094
void Insert::SetWord2C0(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2C0[index].Copy(word);
    }
}

// 0x005EFF3C slot 0x098
void Insert::SetTag17_4(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Tag17_4[index].Copy(word);
    }
}

// 0x005F04EC slot 0x09C
void Insert::SetTag17_5(const IWord& word)
{
    m_Tag17_5.Copy(word);
}

// 0x005EF8F4 slot 0x0A0
void Insert::SetWord2CC(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2CC[index].Copy(word);
    }
}

// 0x005EF8E0 slot 0x0A8
void Insert::SetTag17_8(const IWord& word)
{
    m_Tag17_8.Copy(word);
}

// 0x005F04C0 slot 0x0AC
void Insert::SetWord2D8(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2D8[index].Copy(word);
    }
}

// 0x005EFCF8 slot 0x0B0
void Insert::SetWord2DC(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Word2DC[index].Copy(word);
    }
}

// 0x005F05D0 slot 0x0B4
void Insert::SetTag20_0(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Tag20_0[index].Copy(word);
    }
}

// 0x005F0724 slot 0x0B8
void Insert::SetTag20_1(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Tag20_1[index].Copy(word);
    }
}

// 0x005F05A4 slot 0x0BC
void Insert::SetTag20_2(const IWord& word, u32 index)
{
    if (index < 10) {
        m_Tag20_2[index].Copy(word);
    }
}

// 0x005F05FC slot 0x0C0
void Insert::SetTag20_3(const IWord& word)
{
    m_Tag20_3.Copy(word);
}

// 0x0075C99C slot 0x1F4
IWord* Insert::GetNumber(u32 index)
{
    return &m_Tag4_6[index];
}

// 0x0075CB40 slot 0x1F8
IWord* Insert::GetYear(u32 index)
{
    return &m_Tag4_13[index];
}

// 0x0075CB78 slot 0x1FC
IWord* Insert::GetMonth(u32 index)
{
    return &m_Month[index];
}

// 0x0075CACC slot 0x200
IWord* Insert::GetDay(u32 index)
{
    return &m_Day[index];
}

// 0x0075CB30 slot 0x204
IWord* Insert::GetTag4_16(u32 index)
{
    return &m_Tag4_16[index];
}

// 0x0075CAE4 slot 0x208
IWord* Insert::GetTag4_17(u32 index)
{
    return &m_Tag4_17[index];
}

// 0x0075CAC0 slot 0x20C
IWord* Insert::GetTag4_18()
{
    return &m_Tag4_18;
}

// 0x0075CB1C slot 0x210
IWord* Insert::GetTag4_19(u32 index)
{
    return &m_Tag4_19[index];
}

// 0x0075CBC0 slot 0x214
IWord* Insert::GetTag4_20(u32 index)
{
    return &m_Tag4_20[index];
}

// 0x0075CA24 slot 0x218
IWord* Insert::GetTag5_0()
{
    return &m_Tag5_0;
}

// 0x0075CBB0 slot 0x220
IWord* Insert::GetTag5_2()
{
    return &m_Tag5_2;
}

// 0x0075CBA4 slot 0x224
IWord* Insert::GetTag5_3()
{
    return &m_Tag5_3;
}

// 0x0075CC30 slot 0x228
IWord* Insert::GetWord228(u32 index)
{
    return &m_Word228[index];
}

// 0x0075C9AC slot 0x22C
IWord* Insert::GetTag5_5()
{
    return &m_Tag5_5;
}

// 0x0075CA30 slot 0x230
IWord* Insert::GetTag5_6()
{
    return &m_Tag5_6;
}

// 0x0075C9D0 slot 0x234
IWord* Insert::GetTag5_7()
{
    return &m_Tag5_7;
}

// 0x0075CB54 slot 0x238
IWord* Insert::GetWord238()
{
    return &m_Word238;
}

// 0x0075CC1C slot 0x240
IWord* Insert::GetImpression(u32 index)
{
    return &m_Impression[index];
}

// 0x0075CCFC slot 0x244
IWord* Insert::GetConstellation()
{
    return &m_Constellation;
}

// 0x0075CCF0 slot 0x248
IWord* Insert::GetTag5_12()
{
    return &m_Tag5_12;
}

// 0x0075C9B8 slot 0x24C
IWord* Insert::GetDrink(u32 index)
{
    return &m_Drink[index];
}

// 0x0075CAA8 slot 0x250
IWord* Insert::GetObject(u32 index)
{
    return &m_Object[index];
}

// 0x0075CA7C slot 0x254
IWord* Insert::GetSweets(u32 index)
{
    return &m_Sweets[index];
}

// 0x0075CA64 slot 0x258
IWord* Insert::GetSports(u32 index)
{
    return &m_Sports[index];
}

// 0x0075C9DC slot 0x25C
IWord* Insert::GetHobby(u32 index)
{
    return &m_Hobby[index];
}

// 0x0075C984 slot 0x260
IWord* Insert::GetFood(u32 index)
{
    return &m_Food[index];
}

// 0x0075C9F4 slot 0x264
IWord* Insert::GetMusic(u32 index)
{
    return &m_Music[index];
}

// 0x0075CB60 slot 0x268
IWord* Insert::GetTag5_21(u32 index)
{
    return &m_Tag5_21[index];
}

// 0x0075CAF8 slot 0x274
IWord* Insert::GetTag5_24()
{
    return &m_Tag5_24;
}

// 0x0075CC48 slot 0x278
IWord* Insert::GetWord278()
{
    return &m_Word278;
}

// 0x0075CBD4 slot 0x27C
IWord* Insert::GetTownFruit(u32 index)
{
    return &m_TownFruit[index];
}

// 0x0075CD08 slot 0x280
IWord* Insert::GetWord280(u32 index)
{
    return &m_Word280[index];
}

// 0x0075CCD8 slot 0x284
IWord* Insert::GetWord284(u32 index)
{
    return &m_Word284[index];
}

// 0x0075C96C slot 0x288
IWord* Insert::GetBook(u32 index)
{
    return &m_Book[index];
}

// 0x0075CA0C slot 0x290
IWord* Insert::GetTag5_31(u32 index)
{
    return &m_Tag5_31[index];
}

// 0x0075CA94 slot 0x294
IWord* Insert::GetTag5_33(u32 index)
{
    return &m_Tag5_33[index];
}

// 0x0075CB8C slot 0x298
IWord* Insert::GetWord298(u32 index)
{
    return &m_Word298[index];
}

// 0x0075CC64 slot 0x29C
IWord* Insert::GetTag5_35()
{
    return &m_Tag5_35;
}

// 0x0075CC7C slot 0x2A0
IWord* Insert::GetTag5_36()
{
    return &m_Tag5_36;
}

// 0x0075CC54 slot 0x2A4
IWord* Insert::GetTag5_37()
{
    return &m_Tag5_37;
}

// 0x0075CB04 slot 0x2A8
IWord* Insert::GetTag5_32(u32 index)
{
    return &m_Tag5_32[index];
}

// 0x0075CA3C slot 0x2AC
IWord* Insert::GetWord2AC(u32 index, u32 subIndex)
{
    return &m_Word2AC[index][subIndex];
}

// 0x0075CC04 slot 0x2B4
IWord* Insert::GetWord2B4(u32 index)
{
    return &m_Word2B4[index];
}

// 0x0075CBEC slot 0x2B8
IWord* Insert::GetWord2B8(u32 index)
{
    return &m_Word2B8[index];
}

// 0x0075CCC4 slot 0x2BC
IWord* Insert::GetWord2BC(u32 index)
{
    return &m_Word2BC[index];
}

// 0x0075CCAC slot 0x2C0
IWord* Insert::GetWord2C0(u32 index)
{
    return &m_Word2C0[index];
}

// 0x0075CD34 slot 0x2C4
IWord* Insert::GetTag17_4(u32 index)
{
    return &m_Tag17_4[index];
}

// 0x0075CD60 slot 0x2C8
IWord* Insert::GetTag17_5()
{
    return &m_Tag17_5;
}

// 0x0075CC94 slot 0x2CC
IWord* Insert::GetWord2CC(u32 index)
{
    return &m_Word2CC[index];
}

// 0x0075CC70 slot 0x2D0
IWord* Insert::GetTag0_4()
{
    return &m_Tag0_4;
}

// 0x0075CC88 slot 0x2D4
IWord* Insert::GetTag17_8()
{
    return &m_Tag17_8;
}

// 0x0075CD48 slot 0x2D8
IWord* Insert::GetWord2D8(u32 index)
{
    return &m_Word2D8[index];
}

// 0x0075CD20 slot 0x2DC
IWord* Insert::GetWord2DC(u32 index)
{
    return &m_Word2DC[index];
}

// 0x0075CD84 slot 0x2E0
IWord* Insert::GetTag20_0(u32 index)
{
    return &m_Tag20_0[index];
}

// 0x0075CDA8 slot 0x2E4
IWord* Insert::GetTag20_1(u32 index)
{
    return &m_Tag20_1[index];
}

// 0x0075CD6C slot 0x2E8
IWord* Insert::GetTag20_2(u32 index)
{
    return &m_Tag20_2[index];
}

// 0x0075CD9C slot 0x2EC
IWord* Insert::GetTag20_3()
{
    return &m_Tag20_3;
}

} // namespace script
