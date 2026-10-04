#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/cx/cx_Zlib.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common14ZlibCompressorE @ 0x008CFE44
// vtable 0x00901538 (vptr 0x00901540), offset_to_top 0, 1 entries
//
// zlib deflate into an output buffer; zlib allocates from a work buffer the application gives.
// Layout from the constructor and Startup; the member names are ours.
class ZlibCompressor : public ::nn::pia::common::RootObject
{
public:
    static const unsigned int WORK_BUFFER_SIZE_MIN = 0x16C4;
    static const u32 ALLOC_COUNT_MAX = 5;

    ZlibCompressor(); // 0x00427294 | fefates:bytes [tier B]
    virtual void Trace(u64 flag) const; // 0x00731AEC slot 0x00 (name after StepSequenceJob::Trace)

    // the work buffer (4-aligned)
    nn::Result Initialize(void* pWorkBuffer, unsigned int workBufferSize); // 0x00426F70 | fefates:bytes [tier B]
    void Finalize(); // 0x00427284 (name is ours)
    nn::Result Startup(unsigned char* pOutputBuffer, unsigned int outputBufferSize, int level, int windowBits, int memLevel); // 0x0042711C | fefates:bytes [tier B]
    void Cleanup(); // 0x00427080 | fefates:bytes [tier B]
    nn::Result Deflate(const unsigned char* pData, unsigned int size); // 0x0042709C | fefates:bytes [tier B]
    // the compressed size
    nn::Result FinishDeflate(unsigned int* pSize); // 0x00426FDC | fefates:bytes [tier B]

    static void* myAlloc(void* opaque, unsigned int items, unsigned int size); // 0x0042723C | fefates:bytes [tier B]
    static void myFree(void* opaque, void* address); // 0x0042706C | fefates:bytes [tier B]

    u8* m_pWorkBuffer;            // 0x04
    unsigned int m_WorkBufferSize; // 0x08
    u32 m_AllocCount;             // 0x0C
    u32 m_AllocOffset;            // 0x10
    unsigned int m_OverheadSize;  // 0x14, what deflate may add to the data
    z_stream m_Stream;            // 0x18
};
ASSERT_SIZE(ZlibCompressor, 0x50);
} // namespace common
} // namespace pia
} // namespace nn
