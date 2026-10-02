#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class StationIdTable
{
public:
    struct Entry { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Finalize(); // 0x0044F510 | fefates:bytes [tier B]
    StationIdTable(); // 0x0044F5A8 | fefates:bytes [tier B]
    ~StationIdTable(); // 0x0044F5F0 | fefates:bytes [tier B]
    void GetEntryNum() const; // 0x00734EE0 | fefates:bytes [tier B]
    void Find(nn::pia::transport::StationIdTable::Entry*, nn::pia::StationIndex) const; // 0x00734F10 | fefates:bytes [tier B]
    void Find(nn::pia::transport::StationIdTable::Entry*, nn::pia::StationId) const; // 0x00734F74 | fefates:bytes [tier B]
    void Find(nn::pia::transport::StationIdTable::Entry*, unsigned int) const; // 0x00734FE0 | fefates:bytes [tier B]
    void FindCore(nn::pia::StationIndex) const; // 0x00735074 | fefates:bytes-fuzzy [tier B]
    void FindCore(nn::pia::StationId) const; // 0x007350B0 | fefates:bytes [tier B]
    void FindCore(unsigned int) const; // 0x00735104 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
