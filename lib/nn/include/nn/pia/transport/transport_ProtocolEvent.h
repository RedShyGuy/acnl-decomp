#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport13ProtocolEventE @ 0x008D0170
// vtable 0x00901DBC (vptr 0x00901DC4), offset_to_top 0, 1 entries
//
// A station joined or left; ProtocolManager passes it to the protocols. The member and type names
// are ours.
class ProtocolEvent : public ::nn::pia::common::RootObject
{
public:
    enum Type : u8
    {
        TYPE_JOIN = 0,
        TYPE_LEAVE = 1,
    };

    ProtocolEvent(Type type, StationIndex stationIndex) : m_Type(type), m_StationIndex(stationIndex) {}
    virtual void Trace(u64 flag) const; // 0x00734D54 slot 0x00 (name after StepSequenceJob::Trace)

    Type m_Type;                 // 0x4
    StationIndex m_StationIndex; // 0x5
};
ASSERT_SIZE(ProtocolEvent, 0x8);
} // namespace transport
} // namespace pia
} // namespace nn
