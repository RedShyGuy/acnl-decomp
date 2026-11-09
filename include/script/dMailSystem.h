#pragma once

#include "decomp.h"
#include "libms/libms.h"
#include "script/dLoader.h"
#include "sead/seadFixedSafeString.h"

namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn

namespace script {
class IMailRecept;
class MailInsert;
class MailMeasure;
class MailPhrase;
class MailSeq;

// A message file of the scripts in a directory (Script/<directory>/<name>.umsbt; names are ours).
struct MessageFile
{
    // (inline)
    MessageFile() : m_Loader(false), m_Path() {}

    // the path of the file name in the directory
    void Set(const sead::SafeStringBase<char>& name, const char* directory); // 0x005FDBBC
    // (inline) the file is freed
    void Clear()
    {
        m_Loader.Unload();
        m_Path.clear();
    }

    Loader m_Loader;                    // 0x000
    sead::FixedSafeString<64> m_Path;   // 0x100
};
ASSERT_SIZE(MessageFile, 0x14C);

// The writer of the letters (MailMgr +0x14; no RTTI, the name is ours, after TalkSystem): the parts of a letter (the
// addressee, the text and the sender) come from the message files of the parts with the labels of the recept and are
// written with the words of the insert into the words of the recept. A state machine with four states per part (PMF
// table 0x008926B4): the file, its loading, the text, the release. The directories: "MailN" (the normal letters:
// To/Msg/From), "MailR" (the random letters: To, MsgA/B/C, From) and "Bbs" (the bulletin board).
class MailSystem
{
public:
    typedef void (MailSystem::*StateFunc)();
    static const u8 STATE_NONE = 36;
    static const u8 STATE_NORMAL = 0;
    static const u8 STATE_RANDOM = 12;
    static const u8 STATE_BBS = 32;

    MailSystem(); // 0x00601F5C
    ~MailSystem(); // 0x00133EB8

    // back to no letter; the date is the current one (the original enters it from MailMgr::Begin)
    void Reset(); // 0x00601E4C
    DECOMP_EMIT_COPY MailPhrase* GetPhrase() { return m_pPhrase; } // 0x00601F04
    // the messages of the language of the console from the loaded file
    void SetupMessage(); // 0x00601F0C

    // (inline) the file of the current part is loaded; true when done
    bool LoadFile();
    // (inline) the message of the label is written by func of the sequence; true: done
    DECOMP_ALWAYS_INLINE bool MakeText(s32 label, bool (MailSeq::*func)(const char16* const* text));
    // (inline) the file of the part is freed
    void ReleaseFile()
    {
        if (m_Message.IsInitialized()) {
            m_Message.Finalize();
        }
        m_File.Clear();
    }

    // (the state functions)
    void State0(); // 0x00601814
    void State1(); // 0x00601864
    void State2(); // 0x00601774
    void State3(); // 0x00601CE4
    void State4(); // 0x0060194C
    void State5(); // 0x0060199C
    void State6(); // 0x006018C0
    void State7(); // 0x00601D50
    void State8(); // 0x00601C34
    void State9(); // 0x00601C88
    void State10(); // 0x00601BA8
    void State11(); // 0x00601DBC
    void State12(); // 0x00601158
    void State13(); // 0x006011AC
    void State14(); // 0x006010B8
    void State15(); // 0x00601708
    void State16(); // 0x006013D4
    void State17(); // 0x0060142C
    void State18(); // 0x00601348
    void State19(); // 0x00601A64
    void State20(); // 0x00601514
    void State21(); // 0x0060156C
    void State22(); // 0x00601488
    void State23(); // 0x00601AD0
    void State24(); // 0x00601654
    void State25(); // 0x006016AC
    void State26(); // 0x006015C8
    void State27(); // 0x00601B3C
    void State28(); // 0x00601294
    void State29(); // 0x006012EC
    void State30(); // 0x00601208
    void State31(); // 0x006019F8
    void State32(); // 0x00600FAC
    void State33(); // 0x00600FF0
    void State34(); // 0x00600F20
    void State35(); // 0x0060104C

    MailMeasure* m_pMeasure;              // 0x000
    MailInsert* m_pInsert;                // 0x004
    MailPhrase* m_pPhrase;                // 0x008
    MailSeq* m_pSeq;                      // 0x00C
    nn::fnd::DateTimeParameters* m_pDate; // 0x010 the date of the letter
    u8 m_State;                           // 0x014 (STATE_NONE: done)
    u8 m_IsSuccess;                       // 0x015 all parts were written
    MessageFile m_File;                   // 0x018 the file of the current part
    IMailRecept* m_pRecept;               // 0x164 (during MailMgr::Run)
    libms::Message m_Message;             // 0x168
    sead::FixedSafeString<8> m_Label;     // 0x170 the label of the part ("%03d")

    static const StateFunc s_StateFuncs[STATE_NONE];
};
ASSERT_SIZE(MailSystem, 0x184);
} // namespace script
