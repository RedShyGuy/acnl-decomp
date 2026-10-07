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
    virtual ~MatchmakeParam(); // 0x003718CC slot 0x00 | slot vf_0x00 of nn::nex::_DDL_MatchmakeParam
    // 0x003718BC slot 0x04 | slot vf_0x04 of nn::nex::_DDL_MatchmakeParam (deleting dtor)
    MatchmakeParam(); // 0x0037181C | fefates:bytes [tier B]

    // the parameter of the key is set (the insert of the RW tree is inline; name is ours)
    void SetParam(const String& key, const Variant& value); // 0x003D3924
    // the value of @LGFPC; false if there is none (the lookup is inline; armlink placed it at
    // the end of the code; name is ours)
    bool GetParamLGFPC(u32* pValue) const; // 0x0072B124
    // the value of the key as u32; false if there is none (inline; not decompiled yet)
    bool GetParam(const String& key, u32* pValue) const;
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
