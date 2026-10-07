#include "nn/pia/pia_Types.h"

// The special station ids. They lie between the inet functions in the binary (the file name is
// ours); every getter has its own guard.

namespace nn {
namespace pia {
// 0x003E24AC (name is ours)
const StationId& GetStationIdOfIndex255()
{
    // 0x00975A58 (guard 0x00975A44)
    static const StationId s_Id(0xFFFFFFFF, 0xFFFFFFFF);
    return s_Id;
}

// 0x003E24FC (name is ours)
const StationId& GetStationIdOfIndex254()
{
    // 0x00975A50 (guard 0x00975A40)
    static const StationId s_Id(0xFFFFFFFE, 0xFFFFFFFF);
    return s_Id;
}

// 0x003E254C (name is ours)
const StationId& GetStationIdOfIndex253()
{
    // 0x00975A48 (guard 0x00975A3C)
    static const StationId s_Id(0xFFFFFFFD, 0xFFFFFFFF);
    return s_Id;
}

} // namespace pia
} // namespace nn
