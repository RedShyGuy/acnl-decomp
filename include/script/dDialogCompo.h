#pragma once

#include "decomp.h"
#include "script/dTalkSystem.h"
#include "script/dWordFix.h"

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
        s32 values[2];  // 0x00 the arguments of 19/1 and 19/2
        s32 unknown8;   // 0x08
        s32 kind;       // 0x0C the index of the last tag 19/0..2 (-1: none)
        s32 flag;       // 0x10 from the flag of 19/1 and 19/2 (-1: none)
        u8 value;       // 0x14 the argument of 19/3 (< 5; 5: none)
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
    ~DialogCompo(); // 0x005D5EE4

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
};
ASSERT_SIZE(DialogCompo::Info, 0x18);
ASSERT_SIZE(DialogCompo, 0x1008);
} // namespace script
