#include "nn/nex/nex_StringConverter.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::StringConverter::StringConverter()
{
}

// 0x0037DC64 | fefates:bytes [tier B]
void nn::nex::StringConverter::Decode(wchar_t*, unsigned short, bool)
{
}

// 0x0037DC94 | mk7dlp:callgraph [tier A]
void nn::nex::StringConverter::Encode(const wchar_t*, bool)
{
}

// 0x0037DD58 | mk7dlp:bytes [tier A]
nn::nex::StringConverter::StringConverter(int)
{
}

// 0x0037DD90 | mk7dlp:callgraph [tier A]
nn::nex::StringConverter::~StringConverter()
{
}

} // namespace nex
} // namespace nn
