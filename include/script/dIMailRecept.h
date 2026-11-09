#pragma once

#include "decomp.h"
#include "script/dIRecept.h"
#include "sead/seadFixedSafeString.h"

namespace script {
class IWord;

// The recept of a letter: the object the game gives MailMgr with the files and labels of the parts of the letter and
// the words the parts are written into; it answers the words of the insert (IRecept) and the names of the tags 5/0 and
// 5/1. Slot and member names are ours.
// RTTI N6script11IMailReceptE @ 0x008D37D0
// vtable 0x00909E54 (vptr 0x00909E5C), offset_to_top 0, 39 entries
class IMailRecept : public ::script::IRecept
{
public:
    // the kinds of letters (m_Mode)
    enum Mode : u8
    {
        MODE_NORMAL = 0,  // MailN
        MODE_RANDOM = 1,  // MailR
        MODE_BBS = 2,     // Bbs
        MODE_NONE = 3,
    };
    // the states of the writing (m_Status)
    enum Status : u8
    {
        STATUS_RUNNING = 1,
        STATUS_DONE = 2,
        STATUS_FAILED = 3,
        STATUS_NONE = 4,
    };

    IMailRecept(); // 0x005DDF60 (name after the RTTI)
    virtual ~IMailRecept(); // 0x005DDFDC slot 0x00
    // 0x005DDFCC slot 0x04 (deleting dtor)
    // the sex of the speaker for the sex tags (as ITalkRecept)
    virtual s32 GetSpeakerSex() = 0; // slot 0x90
    // the names of the tags 5/0 and 5/1 (also the word of the insert slot 0x1D8: 5/0)
    virtual void GetTag5_0(IWord* word) = 0; // slot 0x94
    virtual void GetTag5_1(IWord* word) = 0; // slot 0x98

    IWord* m_pToWord;                   // 0x04 the addressee is written here
    IWord* m_pBodyWord;                 // 0x08 the text
    IWord* m_pFromWord;                 // 0x0C the sender
    s32* m_pToResult;                   // 0x10 (MailSeq::m_Result of the addressee)
    sead::FixedSafeString<32> m_ToFile;   // 0x14 the message files of the parts
    sead::FixedSafeString<32> m_BodyFile; // 0x40
    sead::FixedSafeString<32> m_FromFile; // 0x6C
    s32 m_Labels[5];                    // 0x98 the labels: addressee, texts A/B/C (Bbs, MailN: A), sender (-1: none)
    u8 m_Mode;                          // 0xAC Mode
    u8 m_Status;                        // 0xAD Status
};
ASSERT_SIZE(IMailRecept, 0xB0);
} // namespace script
