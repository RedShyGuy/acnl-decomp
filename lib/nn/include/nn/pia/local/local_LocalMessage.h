#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local12LocalMessageE @ 0x008CFAC8
// vtable 0x00900868 (vptr 0x00900870), offset_to_top 0, 4 entries
class LocalMessage : public ::nn::pia::common::RootObject
{
public:
    LocalMessage(); // ctor candidate(s) 0x00414A74 (unverified)
    virtual void vf_0x00(); // 0x00414AB0 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00414AAC slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00414980 slot 0x08 | fefates:bytes
    virtual void ParseMessageHeader(); // 0x00414940 slot 0x0C | fefates:callseq-callee
    void SetData(const void*, int, unsigned short); // 0x004149C0 | fefates:bytes [tier B]
    void SetData(const void*, unsigned short); // 0x00414A2C | fefates:bytes [tier B]
    void GetData(void*, int, unsigned short) const; // 0x0072FBB0 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
