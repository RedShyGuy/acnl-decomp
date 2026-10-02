#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_Key.h"

namespace nn {
namespace nex {
// 0x003CCA8C slot 0x00 | fefates:bytes
nn::nex::Key::~Key()
{
}

// 0x003CC824 | fefates:bytes [tier B]
void nn::nex::Key::PrepareContentPtr(unsigned int)
{
}

// 0x003CC840 | mk7dlp:callseq-callee [tier A]
nn::nex::Key::Key(const unsigned char*, unsigned)
{
}

// 0x003CC8A4 | fefates:bytes [tier B]
nn::nex::Key::Key(const nn::nex::String&)
{
}

// 0x003CC95C | fefates:bytes [tier B]
nn::nex::Key::Key(const nn::nex::Key&)
{
}

// 0x003CCA2C | fefates:bytes [tier B]
nn::nex::Key::Key()
{
}

// 0x003CCB14 | fefates:bytes [tier B]
void nn::nex::Key::operator=(const nn::nex::Key&)
{
}

// 0x0072DF34 | fefates:bytes [tier B]
void nn::nex::Key::GetContentPtr() const
{
}

// 0x0072DF4C | fefates:bytes [tier B]
void nn::nex::Key::ExtractToString(nn::nex::String*) const
{
}

// 0x0072E040 | fefates:bytes [tier B]
void nn::nex::Key::ToString() const
{
}

} // namespace nex
} // namespace nn
