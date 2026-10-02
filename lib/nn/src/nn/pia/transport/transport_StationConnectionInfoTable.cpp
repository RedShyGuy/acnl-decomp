#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x0045E644 (unverified)
nn::pia::transport::StationConnectionInfoTable::StationConnectionInfoTable()
{
}

// 0x0045E848 slot 0x00 | virtual slot, introduced by nn::pia::transport::StationConnectionInfoTable
void nn::pia::transport::StationConnectionInfoTable::vf_0x00()
{
}

// 0x0045E788 slot 0x04 | fefates:bytes
nn::pia::transport::StationConnectionInfoTable::~StationConnectionInfoTable()
{
}

// 0x00736D08 slot 0x08 | virtual slot, introduced by nn::pia::transport::StationConnectionInfoTable
void nn::pia::transport::StationConnectionInfoTable::vf_0x08()
{
}

// 0x0045E444 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::AddToTable(const nn::pia::transport::Station*, const nn::pia::transport::StationConnectionInfo&)
{
}

// 0x0045E544 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::ClearTable()
{
}

// 0x0045E598 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::Initialize(unsigned int)
{
}

// 0x0045E644 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::CreateInstance()
{
}

// 0x0045E6E8 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::EraseFromTable(const nn::pia::transport::Station*)
{
}

// 0x007367EC | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStation(const nn::pia::transport::StationConnectionInfo&) const
{
}

// 0x007368B4 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStationKey(nn::pia::StationIndex, unsigned int*) const
{
}

// 0x00736974 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStationKey(nn::pia::transport::Station*, unsigned int*) const
{
}

// 0x00736A24 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetLocalStationKey(unsigned int*) const
{
}

// 0x00736A4C | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStationPartialMatch(const nn::pia::transport::StationLocation&, nn::pia::transport::StationConnectionInfoTable::PartialMatchMode) const
{
}

// 0x00736B34 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetPrincipalIdByStation(const nn::pia::transport::Station*) const
{
}

// 0x00736BC4 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStationConnectionInfo(const nn::pia::transport::Station*, nn::pia::transport::StationConnectionInfo*) const
{
}

// 0x00736C80 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::GetStationIndexByPrincipalID(unsigned int) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
