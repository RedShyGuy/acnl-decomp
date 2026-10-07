#include "nn/nex/nex__DDL_MatchmakeParam.h"
#include "nn/nex/nex_MatchmakeParam.h"

namespace nn {
namespace nex {
// 0x003718CC slot 0x00 | slot vf_0x00 of nn::nex::_DDL_MatchmakeParam
nn::nex::MatchmakeParam::~MatchmakeParam()
{
}

// 0x0037181C | fefates:bytes [tier B]
nn::nex::MatchmakeParam::MatchmakeParam()
{
}

// 0x003D3924 (name is ours)
void nn::nex::MatchmakeParam::SetParam(const String&, const Variant&)
{
}

// 0x0096B104
const wchar_t* g_MatchmakeParamKeySI = L"@SI";
// 0x0096B108
const wchar_t* g_MatchmakeParamKeyNCC = L"@NCC";
// 0x0096B10C
const wchar_t* g_MatchmakeParamKeyOIA = L"@OIA";
// 0x0096B110
const wchar_t* g_MatchmakeParamKeyUsGI = L"@UsGI";
// 0x0096B114
const wchar_t* g_MatchmakeParamKeyRV = L"@RV";
// 0x0096B118
const wchar_t* g_MatchmakeParamKeyDR = L"@DR";
// 0x0096B11C
const wchar_t* g_MatchmakeParamKeyVR = L"@VR";
// 0x0096B120
const wchar_t* g_MatchmakeParamKeyUpGI = L"@UpGI";
// 0x0096B124
const wchar_t* g_MatchmakeParamKeyLGFPC = L"@LGFPC";

// 0x007FCA80 (the erase of the RW tree)
template <>
nn::nex::qMap<nn::nex::String, nn::nex::Variant>::Iterator nn::nex::qMap<nn::nex::String, nn::nex::Variant>::Erase(
    nn::nex::qMap<nn::nex::String, nn::nex::Variant>::Iterator first, nn::nex::qMap<nn::nex::String, nn::nex::Variant>::Iterator)
{
}

// 0x0072B124 (name is ours)
bool nn::nex::MatchmakeParam::GetParamLGFPC(u32*) const
{
}

} // namespace nex
} // namespace nn
