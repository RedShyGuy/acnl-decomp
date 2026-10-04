#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Watermark.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common16WatermarkManagerE @ 0x008CFE94
// vtable 0x009015AC (vptr 0x009015B4), offset_to_top 0, 1 entries
//
// Ten watermarks. One instance; the member names are ours.
class WatermarkManager : public ::nn::pia::common::RootObject
{
public:
    static const int WATERMARK_NUM = 10;

    // (inline in DestroyInstance)
    ~WatermarkManager() {}
    virtual void Trace(u64 flag) const; // 0x00731AFC slot 0x00 (name after StepSequenceJob::Trace)

    static void DestroyInstance(); // 0x00427F84 | fefates:bytes [tier B]
    // null for an index out of range
    Watermark* GetWatermark(int index); // 0x00427F64 | fefates:bytes [tier B]

    Watermark m_Watermarks[WATERMARK_NUM]; // 0x08

    static WatermarkManager* s_pInstance; // 0x0097E404
};
ASSERT_SIZE(WatermarkManager, 0x418);
} // namespace common
} // namespace pia
} // namespace nn
