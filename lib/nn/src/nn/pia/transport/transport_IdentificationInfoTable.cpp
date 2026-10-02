#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"

namespace nn {
namespace pia {
namespace transport {
// ctor candidate(s) 0x0045C304 (unverified)
nn::pia::transport::IdentificationInfoTable::IdentificationInfoTable()
{
}

// 0x0045C670 slot 0x00 | virtual slot, introduced by nn::pia::transport::IdentificationInfoTable
void nn::pia::transport::IdentificationInfoTable::vf_0x00()
{
}

// 0x0045C62C slot 0x04 | fefates:bytes
nn::pia::transport::IdentificationInfoTable::~IdentificationInfoTable()
{
}

// 0x007366BC slot 0x08 | virtual slot, introduced by nn::pia::transport::IdentificationInfoTable
void nn::pia::transport::IdentificationInfoTable::vf_0x08()
{
}

// 0x0045C0D0 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::AddToTable(const nn::pia::transport::Station*, const nn::pia::transport::Station::IdentificationInfo*)
{
}

// 0x0045C1EC | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::IdentificationInfoTable::ClearTable()
{
}

// 0x0045C240 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::Initialize(unsigned int)
{
}

// 0x0045C304 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::CreateInstance()
{
}

// 0x0045C3A0 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::EraseFromTable(const nn::pia::transport::Station*)
{
}

// 0x0045C440 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::GetIdentificationInfo(const nn::pia::transport::Station*, nn::pia::transport::Station::IdentificationInfo*)
{
}

// 0x0045C5CC | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::GetLocalIdentificationInfo(nn::pia::transport::Station::IdentificationInfo*)
{
}

// 0x0045C608 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::SetLocalIdentificationInfo(const nn::pia::transport::Station::IdentificationInfo*)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
