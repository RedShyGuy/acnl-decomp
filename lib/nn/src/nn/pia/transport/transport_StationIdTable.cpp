#include "nn/pia/transport/transport_StationIdTable.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0044F510 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::Finalize()
{
}

// 0x0044F5A8 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::StationIdTable()
{
}

// 0x0044F5F0 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::~StationIdTable()
{
}

// 0x00734EE0 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::GetEntryNum() const
{
}

// 0x00734F10 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry*, nn::pia::StationIndex) const
{
}

// 0x00734F74 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry*, nn::pia::StationId) const
{
}

// 0x00734FE0 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry*, unsigned int) const
{
}

// 0x00735074 | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::StationIdTable::FindCore(nn::pia::StationIndex) const
{
}

// 0x007350B0 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::FindCore(nn::pia::StationId) const
{
}

// 0x00735104 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::FindCore(unsigned int) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
