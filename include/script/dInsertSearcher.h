#pragma once

#include "decomp.h"
#include "script/dMsgEngine.h"

namespace script {
class Insert;
class IWord;

// Searches a text for the next tag of an inserted word (with an attribute of HasTagAttribute) and gives the word of the
// Insert for it; the grammar tags (articles, particles, plural and gender forms) look at the word that follows them.
// Member names are ours.
// RTTI N6script14InsertSearcherE @ 0x008D38D0
// vtable 0x0090A238 (vptr 0x0090A240), offset_to_top 0, 8 entries
class InsertSearcher : public ::script::MsgEngine
{
public:
    typedef void (InsertSearcher::*TagFunc)(const Tag* tag);

    static const s32 GROUP_ANY = 0x7FFFFFFF;
    static const s32 INDEX_ANY = -1;
    static const u8 ARG_ANY = 10;   // m_ArgIndex: any index of the word lists

    // (inline)
    explicit InsertSearcher(Insert* pInsert)
        : m_pInsert(pInsert), m_Kind(10), m_Group(GROUP_ANY), m_Index(INDEX_ANY), m_pResult(NULL), m_ArgIndex(ARG_ANY)
    {
    }
    virtual ~InsertSearcher();
    virtual bool IsContinue(); // 0x005E6E48 slot 0x10
    virtual void OnTag(const Tag* tag); // 0x005E6DA8 slot 0x18

    // the word of the first tag in the text from *text with the attribute kind (and the given group/index and index
    // of the word lists, if not ANY); NULL: none
    IWord* Search(const char16* const* text, u8 kind, u8 argIndex, s32 group, s32 index); // 0x005E6D3C

private:
    // (inline) the index of the word lists of a tag is searched
    bool IsIndex(u32 index, u32 num) const { return m_ArgIndex < ARG_ANY ? m_ArgIndex == index : index < num; }

    void TagYear(const Tag* tag); // 0x005E60B4
    void TagMonthName(const Tag* tag); // 0x005E6234
    void TagDay(const Tag* tag); // 0x005E5B5C
    void TagHour(const Tag* tag); // 0x005E6094
    void TagMinute(const Tag* tag); // 0x005E5F14
    void TagSecond(const Tag* tag); // 0x005E5F34
    void TagNumber1(const Tag* tag); // 0x005E5B14
    void TagReceptYear(const Tag* tag); // 0x005E611C
    void TagReceptMonth(const Tag* tag); // 0x005E6254
    void TagReceptDay(const Tag* tag); // 0x005E5F54
    void Tag4_16(const Tag* tag); // 0x005E60D4
    void Tag4_17(const Tag* tag); // 0x005E5F9C
    void Tag4_18(const Tag* tag); // 0x005E5EF4
    void Tag4_19(const Tag* tag); // 0x005E604C
    void Tag4_20(const Tag* tag); // 0x005E63A0
    void OnTagGroup4(const Tag* tag); // 0x005E5948
    void Tag5_0(const Tag* tag); // 0x005E5D20
    void Tag5_1(const Tag* tag); // 0x005E5CB8
    void Tag5_2(const Tag* tag); // 0x005E6308
    void Tag5_3(const Tag* tag); // 0x005E62E4
    void Tag5_4(const Tag* tag); // 0x005E6624
    void Tag5_5(const Tag* tag); // 0x005E5BA0
    void Tag5_6(const Tag* tag); // 0x005E5D40
    void Tag5_7(const Tag* tag); // 0x005E5C08
    void Tag5_8(const Tag* tag); // 0x005E6188
    void TagWeekday(const Tag* tag); // 0x005E5AB0
    void TagImpression(const Tag* tag); // 0x005E65DC
    void TagConstellation(const Tag* tag); // 0x005E6A0C
    void Tag5_12(const Tag* tag); // 0x005E69EC
    void TagGenderText(const Tag* tag); // 0x005E6304
    void TagDrink(const Tag* tag); // 0x005E5BC0
    void TagObject(const Tag* tag); // 0x005E5E38
    void TagSweets(const Tag* tag); // 0x005E5DA8
    void TagSports(const Tag* tag); // 0x005E5D60
    void TagHobby(const Tag* tag); // 0x005E5C28
    void TagFood(const Tag* tag); // 0x005E5A68
    void TagMusic(const Tag* tag); // 0x005E5C70
    void Tag5_21(const Tag* tag); // 0x005E61A8
    void TagAmPm(const Tag* tag); // 0x005E65BC
    void TagPartOfDay(const Tag* tag); // 0x005E6AE4
    void Tag5_24(const Tag* tag); // 0x005E602C
    void Tag5_25(const Tag* tag); // 0x005E666C
    void TagTownFruit(const Tag* tag); // 0x005E640C
    void Tag5_27(const Tag* tag); // 0x005E6A2C
    void Tag5_28(const Tag* tag); // 0x005E69A4
    void TagBook(const Tag* tag); // 0x005E5A20
    void TagSeason(const Tag* tag); // 0x005E5E80
    void Tag5_31(const Tag* tag); // 0x005E5CD8
    void Tag5_32(const Tag* tag); // 0x005E6328
    void Tag5_33(const Tag* tag); // 0x005E5DF0
    void Tag5_34(const Tag* tag); // 0x005E629C
    void Tag5_35(const Tag* tag); // 0x005E6738
    void Tag5_36(const Tag* tag); // 0x005E684C
    void Tag5_37(const Tag* tag); // 0x005E668C
    void OnTagGroup5(const Tag* tag); // 0x005E598C
    void Tag7(const Tag* tag); // 0x005E5EC8
    void Tag17_0(const Tag* tag); // 0x005E64A0
    void Tag17_1(const Tag* tag); // 0x005E6454
    void Tag17_2(const Tag* tag); // 0x005E6924
    void Tag17_3(const Tag* tag); // 0x005E68D8
    void Tag17_4(const Tag* tag); // 0x005E6B14
    void Tag17_5(const Tag* tag); // 0x005E6BF0
    void Tag17_6(const Tag* tag); // 0x005E688C
    void Tag17_7(const Tag* tag); // 0x005E6758
    void Tag17_8(const Tag* tag); // 0x005E686C
    void Tag17_9(const Tag* tag); // 0x005E6BA4
    void Tag17_10(const Tag* tag); // 0x005E6A74
    void OnTagGroup17(const Tag* tag); // 0x005E61F0
    void Tag18_0(const Tag* tag); // 0x005E67E4
    void Tag18_1(const Tag* tag); // 0x005E6980
    void Tag18_2(const Tag* tag); // 0x005E679C
    void Tag18_3(const Tag* tag); // 0x005E6B70
    void Tag18_4(const Tag* tag); // 0x005E6B60
    void Tag18_5(const Tag* tag); // 0x005E64EC
    void Tag18_6(const Tag* tag); // 0x005E67F4
    void Tag18_7(const Tag* tag); // 0x005E6970
    void Tag18_8(const Tag* tag); // 0x005E6B80
    void Tag18_9(const Tag* tag); // 0x005E6C20
    void Tag18_10(const Tag* tag); // 0x005E6B04
    void Tag18_11(const Tag* tag); // 0x005E6C10
    void Tag18_12(const Tag* tag); // 0x005E59D0
    void Tag18_13(const Tag* tag); // 0x005E6804
    void Tag18_14(const Tag* tag); // 0x005E6AC0
    void Tag18_15(const Tag* tag); // 0x005E6008
    void Tag18_16(const Tag* tag); // 0x005E5B7C
    void Tag18_17(const Tag* tag); // 0x005E6828
    void Tag18_18(const Tag* tag); // 0x005E6728
    void Tag18_19(const Tag* tag); // 0x005E64FC
    void Tag18_20(const Tag* tag); // 0x005E6164
    void Tag18_21(const Tag* tag); // 0x005E6598
    void Tag18_22(const Tag* tag); // 0x005E6718
    void Tag18_23(const Tag* tag); // 0x005E6530
    void Tag18_24(const Tag* tag); // 0x005E6540
    void Tag18_25(const Tag* tag); // 0x005E6550
    void Tag18_26(const Tag* tag); // 0x005E6574
    void Tag18_27(const Tag* tag); // 0x005E66D0
    void Tag18_28(const Tag* tag); // 0x005E650C
    void Tag18_29(const Tag* tag); // 0x005E66F4
    void Tag18_30(const Tag* tag); // 0x005E67C0
    void Tag18_31(const Tag* tag); // 0x005E5FE4
    void Tag18_32(const Tag* tag); // 0x005E63E8
    void Tag18_33(const Tag* tag); // 0x005E6778
    void Tag18_34(const Tag* tag); // 0x005E66AC
    void OnTagGroup18(const Tag* tag); // 0x005E5AD0
    void Tag20_0(const Tag* tag); // 0x005E6C8C
    void Tag20_1(const Tag* tag); // 0x005E6CF4
    void Tag20_2(const Tag* tag); // 0x005E6C44
    void Tag20_3(const Tag* tag); // 0x005E6CD4
    void OnTagGroup20(const Tag* tag); // 0x005E634C
    DECOMP_NOINLINE void SearchWord2AC(u32 kind, u32 index); // 0x005E59E0
    static const TagFunc s_Group4Funcs[21];
    static const TagFunc s_Group5Funcs[38];
    static const TagFunc s_Group17Funcs[11];
    static const TagFunc s_Group18Funcs[35];
    static const TagFunc s_Group20Funcs[5];
    static const TagFunc s_GroupFuncs[21];

public:
    Insert* m_pInsert;   // 0x60
    u8 m_Kind;           // 0x64 the attribute (HasTagAttribute)
    s32 m_Group;         // 0x68 (GROUP_ANY: any)
    s32 m_Index;         // 0x6C (INDEX_ANY: any)
    IWord* m_pResult;    // 0x70
    u8 m_ArgIndex;       // 0x74 (ARG_ANY: any)
};
ASSERT_SIZE(InsertSearcher, 0x78);
} // namespace script
