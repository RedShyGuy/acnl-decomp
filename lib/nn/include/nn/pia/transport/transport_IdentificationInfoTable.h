#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Station.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport23IdentificationInfoTableE @ 0x008D0260
// vtable 0x00901F98 (vptr 0x00901FA0), offset_to_top 0, 3 entries
class IdentificationInfoTable : public ::nn::pia::common::RootObject
{
public:
    IdentificationInfoTable(); // ctor candidate(s) 0x0045C304 (unverified)
    virtual void vf_0x00(); // 0x0045C670 slot 0x00 | virtual slot, introduced by nn::pia::transport::IdentificationInfoTable
    virtual ~IdentificationInfoTable(); // 0x0045C62C slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x007366BC slot 0x08 | virtual slot, introduced by nn::pia::transport::IdentificationInfoTable
    void AddToTable(const nn::pia::transport::Station*, const nn::pia::transport::Station::IdentificationInfo*); // 0x0045C0D0 | fefates:bytes [tier B]
    void ClearTable(); // 0x0045C1EC | fefates:bytes-fuzzy [tier B]
    void Initialize(unsigned int); // 0x0045C240 | fefates:bytes [tier B]
    void CreateInstance(); // 0x0045C304 | fefates:bytes [tier B]
    void EraseFromTable(const nn::pia::transport::Station*); // 0x0045C3A0 | fefates:bytes [tier B]
    void GetIdentificationInfo(const nn::pia::transport::Station*, nn::pia::transport::Station::IdentificationInfo*); // 0x0045C440 | fefates:bytes [tier B]
    void GetLocalIdentificationInfo(nn::pia::transport::Station::IdentificationInfo*); // 0x0045C5CC | fefates:bytes [tier B]
    void SetLocalIdentificationInfo(const nn::pia::transport::Station::IdentificationInfo*); // 0x0045C608 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
