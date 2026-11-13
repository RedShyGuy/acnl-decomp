#pragma once

#include "decomp.h"
#include "script/dIMailRecept.h"
#include "script/dWordFix.h"
#include "script/dWordPtr.h"

struct SvBulletinBoardMessage;
namespace nn {
namespace fnd {
struct DateTimeParameters;
} // namespace fnd
} // namespace nn
namespace script {
class MailPhrase;
} // namespace script

// The receiver of the mail system that writes a message of the bulletin board from messages (the class name is from
// the RTTI; all other names are ours): the text is the body word, the tag 5/1 is a name (empty: cleared), the tag 5/0
// is always empty; the sexes are 4.
// RTTI 15NoticeLoadAdapt @ 0x008CC0D4
// vtable 0x008F1B2C (vptr 0x008F1B34), offset_to_top 0, 39 entries
class NoticeLoadAdapt : public ::script::IMailRecept
{
public:
    NoticeLoadAdapt(); // 0x0030272C
    virtual ~NoticeLoadAdapt(); // 0x003028E8 slot 0x00
    // 0x0029CED8 slot 0x04 (deleting dtor)
    // (the bodies of the sexes lie elsewhere in the program)
    s32 GetSex() override; // 0x0071C100 slot 0x8C
    s32 GetSpeakerSex() override; // 0x0071C108 slot 0x90
    void GetTag5_0(script::IWord* word) override; // 0x0029CE84 slot 0x94
    void GetTag5_1(script::IWord* word) override; // 0x0029CE94 slot 0x98

    // the mail system works with this receiver / ends / its phrase (the words of the tags)
    void Begin(); // 0x003027BC
    void End(); // 0x003028D0
    script::MailPhrase* GetPhrase(); // 0x00302924
    // the message is cleared with the date, then written from the label of the file; the name (also as the tag 5/1)
    // and the town of the writer may be NULL; written: the names go into the message (the bit 0x20 in the regions
    // 0 and 3), else the message is cleared
    void Load(SvBulletinBoardMessage* message, const char* file, s32 label, const nn::fnd::DateTimeParameters* date,
              const script::IWord* name, const script::IWord* town); // 0x003027D0

    script::WordPtr m_Body;   // 0x0B0 the text of the message
    script::WordFix<9> m_Name; // 0x0BC the name of the tag 5/1
};
