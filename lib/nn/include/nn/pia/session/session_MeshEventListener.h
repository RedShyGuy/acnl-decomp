#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/session/session_Mesh.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session17MeshEventListenerE @ 0x008D0008
// vtable 0x0090194C (vptr 0x00901954), offset_to_top 0, 2 entries
//
// What happens when a station joins or leaves the mesh (MeshEventListenerForSession adds the
// session's part). The function names are ours.
class MeshEventListener : public ::nn::pia::common::RootObject
{
public:
    MeshEventListener(); // 0x00439010
    // an event of the mesh (Mesh::m_pEventListener)
    virtual void OnEvent(const nn::pia::session::Mesh::Event& event); // 0x00438F54 slot 0x00
    // the station is removed from the mesh and from the transport
    virtual void OnLeave(nn::pia::StationIndex stationIndex); // 0x00438F58 slot 0x04
};
} // namespace session
} // namespace pia
} // namespace nn
