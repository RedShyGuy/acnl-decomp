#pragma once

#include "decomp.h"
#include "sead/seadTaskParameter.h"

namespace sead {
// RTTI N4sead21AudioSettingParameterE @ 0x008D1E58
// vtable 0x0090651C (vptr 0x00906524), offset_to_top 0, 4 entries
class AudioSettingParameter : public ::sead::TaskParameter
{
public:
    virtual void vf_0x00(); // 0x0074D4D8 slot 0x00 | virtual slot, introduced by sead::AudioSettingParameter
    virtual void vf_0x04(); // 0x0074D48C slot 0x04 | virtual slot, introduced by sead::AudioSettingParameter
    virtual void vf_0x08(); // 0x0054B178 slot 0x08 | virtual slot, introduced by sead::AudioSettingParameter
    virtual void vf_0x0C(); // 0x0054B174 slot 0x0C | virtual slot, introduced by sead::AudioSettingParameter
    void appendSubset(sead::AudioSubsetBase*); // 0x00125C20 | nintendogs:bytes [tier A]
    AudioSettingParameter(); // 0x0012CBD0 | nintendogs:bytes [tier A]
};
} // namespace sead
