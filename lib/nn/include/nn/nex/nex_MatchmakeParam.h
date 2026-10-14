#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_MatchmakeParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14MatchmakeParamE @ 0x008CE360
// vtable 0x008FCBE0 (vptr 0x008FCBE8), offset_to_top 0, 2 entries
class MatchmakeParam : public ::nn::nex::_DDL_MatchmakeParam
{
public:
    MatchmakeParam(); // 0x0037181C | fefates:bytes [tier B]
    virtual ~MatchmakeParam(); // 0x003718CC slot 0x00
    // 0x003718BC slot 0x04 (deleting dtor)

    // the parameter of the key is set (the insert of the RW tree is inline; name is ours)
    void SetParam(const String& key, const Variant& value); // 0x003D3924
    // the value of @LGFPC; false if there is none (the lookup is inline; armlink placed it at
    // the end of the code; name is ours)
    bool GetParamLGFPC(u32* pValue) const; // 0x0072B124
    // (inline) the parameter of the key; false if there is none
    bool GetParam(const String& key, Variant* pValue) const
    {
        qMap<String, Variant>::iterator it = m_Params.find(key);
        if (it == m_Params.end()) {
            return false;
        }
        *pValue = it->second;
        return true;
    }
    // (inline) the value of the key as u32 (pValue may be null); false if there is none
    bool GetParam(const String& key, u32* pValue) const
    {
        Variant value;
        bool isFound = GetParam(key, &value);
        if (isFound && pValue != nullptr) {
            *pValue = value.GetUInt32Value();
        }
        return isFound;
    }
};

// the keys of the parameters that pia sets (names are ours)
extern const wchar_t* g_MatchmakeParamKeySI;   // "@SI"
extern const wchar_t* g_MatchmakeParamKeyNCC;  // "@NCC"
extern const wchar_t* g_MatchmakeParamKeyOIA;  // "@OIA"
extern const wchar_t* g_MatchmakeParamKeyUsGI; // "@UsGI"
extern const wchar_t* g_MatchmakeParamKeyRV;   // "@RV"
extern const wchar_t* g_MatchmakeParamKeyDR;   // "@DR"
extern const wchar_t* g_MatchmakeParamKeyVR;   // "@VR"
extern const wchar_t* g_MatchmakeParamKeyUpGI; // "@UpGI"
extern const wchar_t* g_MatchmakeParamKeyLGFPC; // "@LGFPC"
} // namespace nex
} // namespace nn
