#include "nn/pia/inet/inet_InetLeaveWithHostMigrationJob.h"
#include "nn/pia/session/session_Mesh.h"

namespace nn {
namespace pia {
namespace inet {
// 0x004110C0 | slot DecideNextHost of nn::pia::session::LeaveWithHostMigrationJob
nn::pia::StationIndex nn::pia::inet::InetLeaveWithHostMigrationJob::DecideNextHost()
{
    // the host hands the mesh over to the station with the smallest valid index
    StationIndex localIndex = session::Mesh::s_pInstance->m_LocalStationIndex;
    if (localIndex <= STATION_INDEX_MAX && session::Mesh::s_pInstance->m_HostStationIndex == localIndex) {
        for (s32 i = 0; i <= STATION_INDEX_MAX; i++) {
            if (static_cast<StationIndex>(i) != localIndex &&
                session::Mesh::s_pInstance->CheckStationIndexIsValid(static_cast<StationIndex>(i))) {
                return static_cast<StationIndex>(i);
            }
        }
    }
    return STATION_INDEX_UNIDENTIFIED;
}

// 0x00411128
nn::pia::inet::InetLeaveWithHostMigrationJob::InetLeaveWithHostMigrationJob()
{
    // only the vptr (in the original too)
}

// 0x004435D8 | slot vf_0x00 of nn::pia::common::Job
// 0x00411140 (deleting dtor)
nn::pia::inet::InetLeaveWithHostMigrationJob::~InetLeaveWithHostMigrationJob()
{
    // empty (in the original too)
}

// 0x0072FA60 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::inet::InetLeaveWithHostMigrationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
