#include "nn/nex/nex__DDL_MatchmakeParam.h"
#include "nn/nex/nex_MatchmakeParam.h"

namespace nn {
namespace nex {
// 0x0037181C | fefates:bytes [tier B]
nn::nex::MatchmakeParam::MatchmakeParam()
{
    m_Params.clear();
}

// 0x003718CC slot 0x00
// 0x003718BC (deleting dtor)
nn::nex::MatchmakeParam::~MatchmakeParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D3924 (name is ours)
void nn::nex::MatchmakeParam::SetParam(const String& key, const Variant& value)
{
    m_Params[key] = value;
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

// 0x0072B124 (name is ours)
bool nn::nex::MatchmakeParam::GetParamLGFPC(u32* pValue) const
{
    Variant value;
    bool isFound = GetParam(String(g_MatchmakeParamKeyLGFPC), &value);
    if (isFound && pValue != nullptr) {
        *pValue = value.GetUInt32Value();
    }
    return isFound;
}

} // namespace nex
} // namespace nn
