#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMessage.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local30LocalStartHostMigrationMessageE @ 0x008CFD60
// vtable 0x00901340 (vptr 0x00901348), offset_to_top 0, 4 entries
class LocalStartHostMigrationMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalStartHostMigrationMessage(); // ctor candidate(s) 0x0042394C (unverified)
    virtual void vf_0x00(); // 0x00423980 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x0042397C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00423900 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x004238C0 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};
} // namespace local
} // namespace pia
} // namespace nn
