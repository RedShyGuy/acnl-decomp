#pragma once

#include "decomp.h"

namespace nn {
namespace cec {
namespace CTR {
class Message
{
public:
    void NewMessage(unsigned, unsigned, unsigned char, unsigned char, unsigned char, unsigned char); // 0x003504E4 | nintendogs:bytes [tier A]
    void SetExHeader(unsigned, unsigned, const void*); // 0x00350558 | nintendogs:bytes [tier A]
    void InputMessage(const void*, unsigned); // 0x00350648 | nintendogs:bytes [tier A]
    void SetMessageBody(const void*, unsigned); // 0x003507B4 | nintendogs:bytes [tier A]
    void InitializeMessage(); // 0x003508D8 | nintendogs:bytes [tier A]
    void SetExHeaderWithoutCalc(unsigned, unsigned, const void*); // 0x0035093C | nintendogs:bytes [tier A]
    Message(); // 0x003509F4 | nintendogs:bytes [tier A]
    void GetMessageId(nn::cec::CTR::MessageId*) const; // 0x00350A40 | nintendogs:bytes [tier A]
    void GetExHeader(unsigned, unsigned*, void**) const; // 0x00729358 | nintendogs:bytes [tier A]
    void GetMessageBody(void*, unsigned) const; // 0x00729454 | nintendogs:bytes [tier A]
    void MakeMessageBinary(void*) const; // 0x007294C8 | nintendogs:bytes [tier A]
    void OutputMessageHeader(void*) const; // 0x00729580 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace cec
} // namespace nn
