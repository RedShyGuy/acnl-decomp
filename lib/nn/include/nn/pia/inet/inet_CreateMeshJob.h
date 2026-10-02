#pragma once

#include "decomp.h"
#include "nn/pia/session/session_CreateMeshJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet13CreateMeshJobE @ 0x008CF86C
// vtable 0x008FFF20 (vptr 0x008FFF28), offset_to_top 0, 9 entries
class CreateMeshJob : public ::nn::pia::session::CreateMeshJob
{
public:
    CreateMeshJob(); // ctor address unknown
    virtual ~CreateMeshJob(); // 0x004310C8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x003E6B74 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void StartupImpl(); // 0x003E6A9C slot 0x18 | fefates:bytes
    virtual void CleanupImpl(); // 0x003E6A98 slot 0x1C | slot vf_0x1C of nn::pia::session::CreateMeshJob
    virtual void vf_0x20(); // 0x003E6B44 slot 0x20 | virtual slot, introduced by nn::pia::session::CreateMeshJob
};
} // namespace inet
} // namespace pia
} // namespace nn
