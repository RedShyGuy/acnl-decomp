#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x003D1300 (symbols.json: nn::boss::TaskIdList::TaskIdList)
nn::nex::String::String()
{
}

// 0x003D135C slot 0x00 | fefates:bytes
nn::nex::String::~String()
{
}

// 0x003D1140 | fefates:bytes [tier B]
void nn::nex::String::ReleaseCopy(char*)
{
}

// 0x003D1168 | fefates:bytes [tier B]
void nn::nex::String::Format(const wchar_t*, ...)
{
}

// 0x003D11A4 | fefates:bytes [tier B]
void nn::nex::String::IsEqual(const wchar_t*, const wchar_t*)
{
}

// 0x003D11F4 | fefates:bytes [tier B]
void nn::nex::String::Reserve(int)
{
}

// 0x003D1250 | mk7dlp:bytes-fuzzy [tier A]
nn::nex::String::String(const char*)
{
}

// 0x003D12AC | mk7dlp:callgraph [tier A]
nn::nex::String::String(const wchar_t*)
{
}

// 0x003D12D8 | fefates:bytes [tier B]
nn::nex::String::String(const nn::nex::String&)
{
}

// 0x003D13A4 | mk7dlp:callseq [tier A]
void nn::nex::String::operator =(const char*)
{
}

// 0x003D1424 | fefates:bytes [tier B]
void nn::nex::String::operator=(const wchar_t*)
{
}

// 0x003D1484 | fefates:bytes [tier B]
void nn::nex::String::operator=(const nn::nex::String&)
{
}

// 0x003D14DC | fefates:bytes [tier B]
void nn::nex::String::operator+=(const nn::nex::String&)
{
}

// 0x0072E178 | fefates:bytes [tier B]
void nn::nex::String::CreateCopy(char**) const
{
}

// 0x0072E1C8 | fefates:bytes [tier B]
void nn::nex::String::FindSubstringNoCase(const wchar_t*) const
{
}

// 0x0072E2CC | fefates:bytes [tier B]
void nn::nex::String::ToUInt64() const
{
}

// 0x0072E360 | mk7dlp:callgraph [tier A]
u32 nn::nex::String::GetLength() const
{
}

} // namespace nex
} // namespace nn
