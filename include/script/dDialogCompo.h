#pragma once

#include "decomp.h"
#include "script/dTalkSystem.h"
#include "script/dWordFix.h"
#include "script/dStrMgr.h"
#include "Sound/dSoundLabel.h"

namespace script {
class UiMgr;

// A dialog text of the string lists (SYS_2D_Dialog_Text by index or number) with its inserted words, cut into pages of
// lines (a page ends after Arg::lines line feeds or at the tag 0/4); tag group 19 sets the values of the dialog.
// Names are ours.
class DialogCompo
{
public:
    class PageAnalyze;
    class PagePut;
    class TextRecept;

    static const u32 PAGE_NUM = 16;

    // called after the start of the UI texts (to set inserted words)
    typedef void (*Callback)(UiMgr* ui, void* arg);

    // the text of the dialog
    struct Arg
    {
        const char* name;   // 0x00 the string list
        s32 index;          // 0x04 the string by index (-1: by number)
        s32 number;         // 0x08 the string by number (label "%03d"; -1: none)
        Callback callback;  // 0x0C
        void* callbackArg;  // 0x10
        s32 lines;          // 0x14 the lines of a page
    };

    // the values of the dialog from the tags 19/0..3 (meaning unknown)
    struct Info
    {
        s32 values[3];  // 0x00 the arguments of 19/1, 19/2 (and a third): the labels of the buttons (SYS_2D_Dialog_Button
                        //      by number)
        s32 kind;       // 0x0C the index of the last tag 19/0..2 (-1: none)
        s32 flag;       // 0x10 from the flag of 19/1 and 19/2 (-1: none)
        u8 value;       // 0x14 the argument of 19/3 (< 5; 5: none): the sounds of the buttons (s_ButtonSes)
    };

    // the text of the index / of the number
    void ComposeByIndex(s32 index, Callback callback, void* callbackArg); // 0x005D5EA4
    void ComposeByNumber(s32 number, Callback callback, void* callbackArg); // 0x005D5E44
    // the text is written and cut into pages; the first page is put
    void Compose(const Arg& arg); // 0x005D950C
    // the next page (if there is one)
    void NextPage(); // 0x005D5E80
    // the current page into m_PageText
    void PutPage(); // 0x005D9210
    DialogCompo(); // 0x005D9998
    ~DialogCompo(); // 0x005D5EE4

    // (copies for the game, names are ours) the current page is the last / the first one
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsLastPage() const { return m_Page >= m_PageNum - 1; } // 0x0075ABC4
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsFirstPage() const { return m_Page <= 0; } // 0x0075ABE0
    // the label of the button of a page that is not the last / of the last page without own labels
    DECOMP_EMIT_COPY static s32 GetNextLabel() // 0x0075ABF4
    {
        s32 index = -1;
        StrMgr::s_pInstance->FindNumberedIndex(&index, "SYS_2D_Dialog_Button", 2);
        return index;
    }
    DECOMP_EMIT_COPY static s32 GetCloseLabel() // 0x0075AC28
    {
        s32 index = -1;
        StrMgr::s_pInstance->FindNumberedIndex(&index, "SYS_2D_Dialog_Button", 1);
        return index;
    }
    // the label of button i of the info (-1: none)
    DECOMP_EMIT_COPY s32 GetButtonLabel(u32 i) const // 0x0075AC5C
    {
        if (i >= 3) {
            return -1;
        }
        s32 index = -1;
        StrMgr::s_pInstance->FindNumberedIndex(&index, "SYS_2D_Dialog_Button", m_Info.values[i]);
        return index;
    }
    // the ok sound of button i (0: none)
    DECOMP_EMIT_COPY SeID GetButtonSe(u32 i) const // 0x0075ACA4
    {
        if (m_Info.value >= 5 || i >= 2) {
            return static_cast<SeID>(0);
        }
        return s_ButtonSes[m_Info.value][i];
    }

    u32 m_Unknown[4];                         // 0x000 (not used by these functions)
    const char16* m_PageStarts[PAGE_NUM];     // 0x010
    const char16* m_PageEnds[PAGE_NUM];       // 0x050
    s32 m_PageNum;                            // 0x090
    s32 m_Page;                               // 0x094 the current page
    Info m_Info;                              // 0x098
    TalkAttribute m_Attribute;                // 0x0B0 the attribute of the string
    WordFix<1536> m_Text;                     // 0x0B8 the whole text
    WordFix<400> m_PageText;                  // 0xCD0 the current page

    static const Arg s_IndexArg;
    static const Arg s_NumberArg;
    // the ok sounds of the two buttons per Info::value
    static const SeID s_ButtonSes[5][2];
};
ASSERT_SIZE(DialogCompo::Info, 0x18);
ASSERT_SIZE(DialogCompo, 0x1008);
} // namespace script
