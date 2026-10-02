#include "nn/nex/nex_SocketDriver.h"
#include "nn/nex/nex_BerkeleySocketDriver.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::BerkeleySocketDriver::BerkeleySocketDriver()
{
}

// 0x00396214 slot 0x00 | slot vf_0x00 of nn::nex::BerkeleySocketDriver
nn::nex::BerkeleySocketDriver::~BerkeleySocketDriver()
{
}

// 0x003961FC slot 0x04 | virtual slot, introduced by nn::nex::BerkeleySocketDriver
void nn::nex::BerkeleySocketDriver::vf_0x04()
{
}

// 0x0039615C slot 0x08 | fefates:callseq
void nn::nex::BerkeleySocketDriver::vf_0x08()
{
}

// 0x003961E4 slot 0x0C | fefates:bytes
void nn::nex::BerkeleySocketDriver::Delete(nn::nex::SocketDriver::Socket*)
{
}

// 0x00395FDC slot 0x10 | fefates:bytes-fuzzy
void nn::nex::BerkeleySocketDriver::Poll(nn::nex::SocketDriver::PollInfo*, unsigned int, unsigned int)
{
}

} // namespace nex
} // namespace nn
