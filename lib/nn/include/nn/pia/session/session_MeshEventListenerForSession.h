#pragma once

#include "decomp.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/session/session_MeshEventListener.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session27MeshEventListenerForSessionE @ 0x008D011C
// vtable 0x00901CC8 (vptr 0x00901CD0), offset_to_top 0, 2 entries
//
// Session's listener of the mesh: it keeps the station ids of Session, the station id table and
// the StationIdStatusTable up to date, tells the application of the joins and leaves
// (Session::NotifyEvent) and passes some events to the running session jobs.
class MeshEventListenerForSession : public ::nn::pia::session::MeshEventListener
{
public:
    MeshEventListenerForSession(); // 0x00447900
    virtual void OnEvent(const nn::pia::session::Mesh::Event& event); // 0x004463CC slot 0x00

    // (names are ours)
    // the station left: the application is told (if it was) and the tables forget it
    void RemoveLeftStation(StationId stationId); // 0x00446514
    // the events without the station id table (the station ids are the indices)
    void OnEventWithoutStationIdTable(const nn::pia::session::Mesh::Event& event); // 0x0044659C
    // the events with the station id table (the principal ids; joint sessions)
    void OnEventWithStationIdTable(const nn::pia::session::Mesh::Event& event); // 0x00446868
};
} // namespace session
} // namespace pia
} // namespace nn
