#pragma once

#include "decomp.h"
#include "nw/ut/ut_LinkList.h"
#include "script/dLoader.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadFixedSafeString.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

namespace sead {
class Heap;
} // namespace sead

namespace script {
class IWord;
class WordRes;
struct TalkAttribute;

// The manager of the string lists of the scripts (Script/Str/<name>.umsbt): every list is a message binary per
// language; the one of the console is copied into a heap and read with libms under a lock. The lists of the main group
// are loaded once (StrMgr::Load), the lists of the scene group depending on the scene (LoadScene). Singleton; the
// names are ours.
class StrMgr
{
public:
    class SingletonDisposer_;

    // a string list: the file is loaded with a temporary heap, the message binary of the language is copied to m_pHeap
    struct File
    {
        // (inline)
        File() : m_Loader(true), m_pHeap(NULL), m_pData(NULL), m_Size(0) {}

        // (inline) the name, its path and the heap of the copy
        void Set(const sead::SafeStringBase<char>& name, sead::Heap* heap);
        // (inline) loads the file if it has no copy yet; true: the copy is there
        bool Load(sead::Heap* tempHeap);

        Loader m_Loader;                  // 0x000
        sead::FixedSafeString<32> m_Name; // 0x100
        sead::FixedSafeString<64> m_Path; // 0x12C
        sead::Heap* m_pHeap;              // 0x178 the heap of the copy
        void* m_pData;                    // 0x17C the copy of the message binary of the language
        u32 m_Size;                       // 0x180
        nw::ut::LinkListNode m_Node;      // 0x184
    };
    typedef nw::ut::LinkList<File, 0x184> FileList;

    // the string lists of the game with the scenes that need them (0: always)
    struct ListInfo
    {
        const char* name;
        u32 mask;
    };

    // the lists that are always loaded (mask 0)
    struct MainGroup
    {
        // (inline)
        MainGroup() : m_State(0), m_pTempHeap(NULL) {}
        void AddFiles(); // 0x005DFFBC
        bool LoadFiles(); // 0x005DFE70

        u8 m_State;              // 0x00 0 start, 1 add, 2 heap, 3 load, 4 loaded, 5 done
        FileList m_Files;        // 0x04
        sead::Heap* m_pTempHeap; // 0x10 the heap of the file loads
    };

    // the lists of the scene (mask bits; 1 item search in the Nookling stores, 2 the minigames, 4 island, 8 its panes,
    // 0x10..0x80 the staff list of the region at Club LOL's live show, 0x100 the island shack, 0x200 the RVs)
    struct SceneGroup
    {
        // (inline)
        SceneGroup() : m_State(0), m_Mask(0), m_pTempHeap(NULL) {}
        // the lists of the scene are loaded step by step; true when they are (or there are none)
        bool Load(const oml::framework::Process* scene); // 0x005D8008
        void AddFiles(); // 0x005D8370
        bool LoadFiles(); // 0x005D8224

        u8 m_State;              // 0x00 as MainGroup
        FileList m_Files;        // 0x04
        u32 m_Mask;              // 0x10
        sead::Heap* m_pTempHeap; // 0x14
    };

    // the groups under their lock
    struct Table
    {
        // the list of the name (the main group first)
        File* Find(const char* name); // 0x0075BDE8

        sead::CriticalSection m_CS; // 0x00
        MainGroup m_Main;           // 0x1C
        SceneGroup m_Scene;         // 0x30
    };

    // (inline)
    StrMgr() : m_IsLoaded(0) {}

    static StrMgr* createInstance(sead::Heap* heap); // 0x005F3DC8 (name after sead)

    // the main lists are loaded step by step; true when they are
    bool Load(); // 0x005F3F18
    // the lists of the scene; true when they are loaded
    bool LoadScene(const oml::framework::Process* scene); // 0x005D8000

    // the string of the list and index into the word (not found: the word is emptied); true: found
    DECOMP_EMIT_COPY bool GetString(IWord* word, const char* name, u32 index); // 0x00312610
    // the string of the list and index as a resource word; true: found
    bool Find(WordRes* word, const char* name, s32 index); // 0x0075CE3C
    // the string of the list and label as a resource word; true: found
    bool FindByLabel(WordRes* word, const char* name, const char* label); // 0x0075D364
    // the string of the list with the label "%03d" of the number (< 1000) as a resource word; true: found
    bool FindNumbered(WordRes* word, const char* name, u32 number); // 0x0075D41C
    // the string of the numbered label into the word (not found: the word is emptied); true: found
    bool GetStringNumbered(IWord* word, const char* name, u32 number); // 0x0075D108
    // the index of the numbered label (number > 0); true: found
    bool FindNumberedIndex(s32* index, const char* name, s32 number); // 0x0075D174
    // the text of the index (NULL: not found)
    const void* GetText(const char* name, s32 index); // 0x0075CFDC
    // the text of the label (NULL: not found)
    const void* GetTextByLabel(const char* name, const char* label); // 0x0075D514
    // the number of strings of the list
    u32 GetCount(const char* name); // 0x0075CDC8
    // the line of the string of the numbered label into the word (StrRemakeColor; not found: the word is emptied); true:
    // found
    bool GetLine(IWord* word, const char* name, u32 number, s32 line); // 0x0075D058
    // the label of the index into label; true: found
    bool GetLabel(sead::BufferedSafeStringBase<char>* label, const char* name, s32 index); // 0x0075D590
    // the number of lines of the string of the numbered label (0: not found)
    s32 CountLines(const char* name, u32 number); // 0x0075D2B0
    // the attribute of the string of the numbered label (speaker kind left to the attribute); true: found
    bool GetAttribute(TalkAttribute* attribute, const char* name, s32 number); // 0x0075BEB0
    // the nickname STR_Nickname[index] made from name into nickname; true: made and different from the name
    bool MakeNickname(IWord* nickname, s32 index, const IWord* name); // 0x0075CEF8

    u32 m_SingletonDisposerBuffer[4]; // 0x00 the SingletonDisposer_
    u8 m_IsLoaded;                    // 0x10
    Table m_Table;                    // 0x14

    static StrMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    // the string lists (end: NULL)
    static const ListInfo s_Lists[];
};
ASSERT_SIZE(StrMgr::File, 0x18C);
ASSERT_SIZE(StrMgr, 0x5C);
} // namespace script
