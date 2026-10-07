#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15AttendanceTableE @ 0x008D0194
// vtable 0x00901DD4 (vptr 0x00901DDC), offset_to_top 0, 1 entries
//
// Which stations are present: one bit per station index (0..11). Layout from CreateInstance; the
// member names are ours. The destructor is not virtual (the vtable only has Trace).
class AttendanceTable : public ::nn::pia::common::RootObject
{
public:
    // (inline in CreateInstance)
    AttendanceTable() : m_AttendanceBitmap(0), m_IsInitialized(false), m_IsStarted(false) {}
    virtual void Trace(u64 flag) const; // 0x007352E8 slot 0x00

    static nn::Result CreateInstance(); // 0x0044FEF0 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x0044FF6C | fefates:callseq [tier C]
    nn::Result Initialize(); // 0x0044FEC0 | fefates:bytes [tier B]
    void Finalize(); // 0x0045003C | fefates:callseq [tier C]
    nn::Result Startup(); // 0x0044FFFC | fefates:bytes [tier B]
    void Cleanup(); // 0x0044FFF0 | fefates:callseq [tier C]

    // the station is present or not
    nn::Result Update(bool isAttending, nn::pia::StationIndex stationIndex); // 0x0044FF94 | fefates:bytes [tier B]

    static AttendanceTable* s_pInstance;

    u32 m_AttendanceBitmap; // 0x4
    bool m_IsInitialized;   // 0x8
    bool m_IsStarted;       // 0x9
};
ASSERT_SIZE(AttendanceTable, 0xC);
} // namespace transport
} // namespace pia
} // namespace nn
