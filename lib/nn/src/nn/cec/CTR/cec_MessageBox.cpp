#include "nn/cec/CTR/cec_MessageBox.h"

namespace nn {
namespace cec {
namespace CTR {
// 0x001367C0 slot 0x00 | nintendogs:bytes
void nn::cec::CTR::MessageBox::vf_0x00()
{
}

// 0x0034F3D0 slot 0x04 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x04()
{
}

// 0x007291A0 slot 0x08 | nintendogs:bytes
void nn::cec::CTR::MessageBox::OpenFile(unsigned, unsigned, unsigned, unsigned*) const
{
}

// 0x007292A4 slot 0x0C | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x0C()
{
}

// 0x00729300 slot 0x10 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x10()
{
}

// 0x0034E248 slot 0x14 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x14()
{
}

// 0x0034F010 slot 0x18 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x18()
{
}

// 0x0034EB44 slot 0x1C | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x1C()
{
}

// 0x0034F180 slot 0x20 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x20()
{
}

// 0x0034F210 slot 0x24 | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x24()
{
}

// 0x0034F294 slot 0x28 | nintendogs:bytes
void nn::cec::CTR::MessageBox::SetData(unsigned, const unsigned char*, unsigned, unsigned)
{
}

// 0x00729220 slot 0x2C | virtual slot, introduced by nn::cec::CTR::MessageBox
void nn::cec::CTR::MessageBox::vf_0x2C()
{
}

// 0x00728F3C slot 0x30 | nintendogs:bytes
void nn::cec::CTR::MessageBox::OpenAndWriteFile(const unsigned char*, unsigned, unsigned, unsigned, unsigned) const
{
}

// 0x00728EC4 slot 0x34 | nintendogs:bytes
void nn::cec::CTR::MessageBox::OpenAndReadFile(unsigned char*, unsigned, unsigned*, unsigned, unsigned, unsigned) const
{
}

// 0x0013EA84 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::CloseMessageBox(bool)
{
}

// 0x001408A0 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::WriteBoxInfo(nn::cec::CTR::CecBoxType, nn::cec::CTR::CecBoxInfoHeader&, nn::cec::CTR::CecMessageHeader**)
{
}

// 0x00140978 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::WriteMessageBoxInfo()
{
}

// 0x00143264 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::ReadBoxInfo(nn::cec::CTR::CecBoxInfoHeader*, nn::cec::CTR::CecMessageHeader**, unsigned char*, nn::cec::CTR::CecBoxType)
{
}

// 0x0034D668 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::ReadMessage(void*, unsigned, nn::cec::CTR::CecBoxType, const nn::cec::CTR::MessageId&)
{
}

// 0x0034D720 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::ReadMessage(nn::cec::CTR::Message&, void*, unsigned, nn::cec::CTR::CecBoxType, const nn::cec::CTR::MessageId&)
{
}

// 0x0034D784 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::WriteMessage(const nn::cec::CTR::Message&, nn::cec::CTR::CecBoxType, nn::cec::CTR::MessageId&, bool)
{
}

// 0x0034DD68 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::DeleteMessage(nn::cec::CTR::CecBoxType, const nn::cec::CTR::MessageId&, bool)
{
}

// 0x0034E05C | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::OpenMessageBox(unsigned, unsigned)
{
}

// 0x0034E2D8 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::ReadOutBoxIndex()
{
}

// 0x0034EA4C | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::GetMessageBoxNum(unsigned char)
{
}

// 0x0034EBD0 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::DeleteAllMessages(nn::cec::CTR::CecBoxType)
{
}

// 0x0034ED24 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::SetMessageBoxData(unsigned, const void*, unsigned)
{
}

// 0x0034EE38 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::ReadMessageBoxList()
{
}

// 0x0034EF7C | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::WriteMessageBoxList()
{
}

// 0x0034F0A0 | nintendogs:callgraph [tier A]
void nn::cec::CTR::MessageBox::CheckEulaParentalControl()
{
}

// 0x0034F30C | fefates:bytes [tier B]
nn::cec::CTR::MessageBox::MessageBox()
{
}

// 0x00728CCC | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::GetMessageId(nn::cec::CTR::MessageId*, nn::cec::CTR::CecBoxType, unsigned) const
{
}

// 0x00728D24 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::GetMessHeader(nn::cec::CTR::CecBoxType, unsigned) const
{
}

// 0x00728DE0 | fefates:bytes [tier B]
void nn::cec::CTR::MessageBox::GetMessageTag(nn::cec::CTR::CecBoxType, unsigned int) const
{
}

// 0x00728DF4 | nintendogs:bytes [tier A]
void nn::cec::CTR::MessageBox::GetMessageSize(nn::cec::CTR::CecBoxType, unsigned) const
{
}

// 0x00728E08 | nintendogs:bytes-fuzzy [tier A]
void nn::cec::CTR::MessageBox::GetMessageIndex(nn::cec::CTR::CecBoxType, unsigned char*) const
{
}

// 0x00728FB8 | nintendogs:bytes [tier B]
void nn::cec::CTR::MessageBox::GetMessageRecvDate(nn::cec::CTR::CecBoxType, unsigned) const
{
}

// 0x0072901C | nintendogs:callgraph [tier A]
void nn::cec::CTR::MessageBox::ReadMessageBoxInfo(nn::cec::CTR::MessageBoxInfo*, unsigned) const
{
}

// 0x007290AC | nintendogs:bytes [tier B]
void nn::cec::CTR::MessageBox::GetMessageSendCount(nn::cec::CTR::CecBoxType, unsigned) const
{
}

// 0x007290C0 | fefates:bytes [tier B]
void nn::cec::CTR::MessageBox::IsAgreeEulaAppRequired() const
{
}

} // namespace CTR
} // namespace cec
} // namespace nn
