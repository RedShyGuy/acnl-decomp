#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport26StationConnectionInfoTableE @ 0x008D0290
// vtable 0x00902010 (vptr 0x00902018), offset_to_top 0, 3 entries
class StationConnectionInfoTable : public ::nn::pia::common::RootObject
{
public:
    struct PartialMatchMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    StationConnectionInfoTable(); // ctor candidate(s) 0x0045E644 (unverified)
    virtual void vf_0x00(); // 0x0045E848 slot 0x00 | virtual slot, introduced by nn::pia::transport::StationConnectionInfoTable
    virtual ~StationConnectionInfoTable(); // 0x0045E788 slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x00736D08 slot 0x08 | virtual slot, introduced by nn::pia::transport::StationConnectionInfoTable
    void AddToTable(const nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&); // 0x0045E444 | fefates:bytes [tier B]
    void ClearTable(); // 0x0045E544 | fefates:bytes [tier B]
    void Initialize(unsigned int); // 0x0045E598 | fefates:bytes [tier B]
    void CreateInstance(); // 0x0045E644 | fefates:bytes [tier B]
    void EraseFromTable(const nn::pia::transport::Station*); // 0x0045E6E8 | fefates:bytes [tier B]
    void GetStation(const nn::pia::transport::StationConnectionInfo&) const; // 0x007367EC | fefates:bytes [tier B]
    void GetStationKey(nn::pia::StationIndex, unsigned int*) const; // 0x007368B4 | fefates:bytes [tier B]
    void GetStationKey(nn::pia::transport::Station*, unsigned int*) const; // 0x00736974 | fefates:bytes [tier B]
    void GetLocalStationKey(unsigned int*) const; // 0x00736A24 | fefates:bytes [tier B]
    void GetStationPartialMatch(const nn::pia::transport::StationLocation&, nn::pia::transport::StationConnectionInfoTable::PartialMatchMode) const; // 0x00736A4C | fefates:bytes [tier B]
    void GetPrincipalIdByStation(const nn::pia::transport::Station*) const; // 0x00736B34 | fefates:bytes [tier B]
    void GetStationConnectionInfo(const nn::pia::transport::Station*, nn::pia::transport::StationConnectionInfo*) const; // 0x00736BC4 | fefates:bytes [tier B]
    void GetStationIndexByPrincipalID(unsigned int) const; // 0x00736C80 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
