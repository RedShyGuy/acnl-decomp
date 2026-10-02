#include "nn/cec/CTR/cec_Message.h"

namespace nn {
namespace cec {
namespace CTR {
// 0x003504E4 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::NewMessage(unsigned, unsigned, unsigned char, unsigned char, unsigned char, unsigned char)
{
}

// 0x00350558 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::SetExHeader(unsigned, unsigned, const void*)
{
}

// 0x00350648 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::InputMessage(const void*, unsigned)
{
}

// 0x003507B4 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::SetMessageBody(const void*, unsigned)
{
}

// 0x003508D8 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::InitializeMessage()
{
}

// 0x0035093C | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::SetExHeaderWithoutCalc(unsigned, unsigned, const void*)
{
}

// 0x003509F4 | nintendogs:bytes [tier A]
nn::cec::CTR::Message::Message()
{
}

// 0x00350A40 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::GetMessageId(nn::cec::CTR::MessageId*) const
{
}

// 0x00729358 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::GetExHeader(unsigned, unsigned*, void**) const
{
}

// 0x00729454 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::GetMessageBody(void*, unsigned) const
{
}

// 0x007294C8 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::MakeMessageBinary(void*) const
{
}

// 0x00729580 | nintendogs:bytes [tier A]
void nn::cec::CTR::Message::OutputMessageHeader(void*) const
{
}

} // namespace CTR
} // namespace cec
} // namespace nn
