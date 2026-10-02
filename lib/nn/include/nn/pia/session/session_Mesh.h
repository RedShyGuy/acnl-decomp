#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session4MeshE @ 0x008D0140
// vtable 0x00901D28 (vptr 0x00901D30), offset_to_top 0, 3 entries
class Mesh : public ::nn::pia::common::RootObject
{
public:
    struct EventType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    Mesh(); // ctor candidate(s) 0x00449D4C (unverified)
    virtual void vf_0x00(); // 0x00734334 slot 0x00 | virtual slot, introduced by nn::pia::session::Mesh
    virtual ~Mesh(); // 0x00449E50 slot 0x04 | slot vf_0x04 of nn::pia::session::Mesh
    // 0x00449E40 slot 0x08 | slot vf_0x08 of nn::pia::session::Mesh (deleting dtor)
    void CleanupJobs(); // 0x004480D8 | fefates:bytes-fuzzy [tier B]
    void FixConnectedId(nn::pia::StationIndex); // 0x00448AB4 | fefates:bytes-fuzzy [tier B]
    void NoticeMeshEvent(nn::pia::session::Mesh::EventType, nn::pia::StationIndex); // 0x00448CD8 | fefates:bytes-fuzzy [tier B]
    void CleanupStationsJobs(); // 0x004491D8 | fefates:bytes [tier B]
    void UnfixDisconnectedId(nn::pia::StationIndex); // 0x0044929C | fefates:bytes-fuzzy [tier B]
    void NotifyLeaveStationAddress(const nn::pia::common::StationAddress&); // 0x004494E8 | fefates:bytes [tier B]
    void StartUse(nn::pia::StationIndex); // 0x00449C04 | fefates:bytes-fuzzy [tier B]
    void GetTime() const; // 0x00734338 | fefates:bytes-fuzzy [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
