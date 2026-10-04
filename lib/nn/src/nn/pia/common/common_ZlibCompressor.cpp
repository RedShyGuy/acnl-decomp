#include "nn/pia/common/common_ZlibCompressor.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// deflateBound of this much data, less the data, is the overhead of the compression
const unsigned long BOUND_SOURCE_SIZE = 128;
// what a sync flush adds to the output
const unsigned int SYNC_FLUSH_SIZE = 6;
// the version of zlib the stream belongs to
const char ZLIB_VERSION_STRING[] = "1.2.7.f-NINTENDO-CTR-SDK-v1";
} // namespace

// 0x00426F70 | fefates:bytes [tier B]
nn::Result nn::pia::common::ZlibCompressor::Initialize(void* pWorkBuffer, unsigned int workBufferSize)
{
    if (!IsValidPointer(pWorkBuffer) || (reinterpret_cast<uptr>(pWorkBuffer) & 3) != 0 ||
        workBufferSize < WORK_BUFFER_SIZE_MIN) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (IsValidPointer(m_pWorkBuffer)) {
        return RESULT_ALREADY_INITIALIZED;
    }
    m_pWorkBuffer = static_cast<u8*>(pWorkBuffer);
    m_WorkBufferSize = workBufferSize;
    return nn::Result();
}

// 0x00426FDC | fefates:bytes [tier B]
nn::Result nn::pia::common::ZlibCompressor::FinishDeflate(unsigned int* pSize)
{
    if (!IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (m_Stream.total_out == 0 || m_Stream.avail_in != 0) {
        return RESULT_INVALID_STATE;
    }
    if (deflate(&m_Stream, Z_FINISH) != Z_STREAM_END) {
        Trace(1);
        return RESULT_INTERNAL_ERROR;
    }
    *pSize = m_Stream.total_out;
    return nn::Result();
}

// 0x0042706C | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::myFree(void* opaque, void*)
{
    ZlibCompressor* pCompressor = static_cast<ZlibCompressor*>(opaque);
    if (pCompressor->m_AllocCount != 0) {
        pCompressor->m_AllocCount--;
    }
}

// 0x00427080 | fefates:bytes [tier B]
void nn::pia::common::ZlibCompressor::Cleanup()
{
    deflateEnd(&m_Stream);
    m_AllocOffset = 0;
}

// 0x0042709C | fefates:bytes [tier B]
nn::Result nn::pia::common::ZlibCompressor::Deflate(const unsigned char* pData, unsigned int size)
{
    if (!IsValidPointer(pData) || size == 0) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (m_OverheadSize + size + SYNC_FLUSH_SIZE > m_Stream.avail_out) {
        return RESULT_BUFFER_SHORTAGE;
    }
    m_Stream.next_in = pData;
    m_Stream.avail_in = size;
    if (deflate(&m_Stream, Z_SYNC_FLUSH) != Z_OK || m_Stream.avail_in != 0) {
        return RESULT_INTERNAL_ERROR;
    }
    return nn::Result();
}

// 0x0042711C | fefates:bytes [tier B]
nn::Result nn::pia::common::ZlibCompressor::Startup(unsigned char* pOutputBuffer, unsigned int outputBufferSize, int level, int windowBits, int memLevel)
{
    if (!IsValidPointer(pOutputBuffer) || outputBufferSize == 0) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (static_cast<unsigned int>(level) >= 10 || static_cast<unsigned int>(windowBits - 8) >= 8) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (static_cast<unsigned int>(memLevel - 1) >= 9) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (!IsValidPointer(m_pWorkBuffer)) {
        return RESULT_INVALID_STATE;
    }
    m_Stream.zalloc = myAlloc;
    m_Stream.zfree = myFree;
    m_Stream.opaque = this;
    int ret = deflateInit2_(&m_Stream, level, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY, ZLIB_VERSION_STRING,
                            sizeof(z_stream));
    if (ret != Z_OK) {
        return ret == Z_MEM_ERROR ? RESULT_OUT_OF_MEMORY : RESULT_INTERNAL_ERROR;
    }
    m_OverheadSize = deflateBound(&m_Stream, BOUND_SOURCE_SIZE) - BOUND_SOURCE_SIZE;
    m_Stream.next_out = pOutputBuffer;
    m_Stream.avail_out = outputBufferSize;
    return nn::Result();
}

// 0x0042723C | fefates:bytes [tier B]
void* nn::pia::common::ZlibCompressor::myAlloc(void* opaque, unsigned int items, unsigned int size)
{
    ZlibCompressor* pCompressor = static_cast<ZlibCompressor*>(opaque);
    if (pCompressor->m_AllocCount >= ALLOC_COUNT_MAX) {
        return nullptr;
    }
    u32 offset = items * size + pCompressor->m_AllocOffset;
    if (pCompressor->m_WorkBufferSize < offset) {
        return nullptr;
    }
    void* p = pCompressor->m_pWorkBuffer + pCompressor->m_AllocOffset;
    pCompressor->m_AllocOffset = offset;
    pCompressor->m_AllocCount++;
    return p;
}

// 0x00427284 (name is ours)
void nn::pia::common::ZlibCompressor::Finalize()
{
    m_pWorkBuffer = nullptr;
    m_WorkBufferSize = 0;
}

// 0x00427294 | fefates:bytes [tier B]
nn::pia::common::ZlibCompressor::ZlibCompressor()
    : m_pWorkBuffer(nullptr), m_WorkBufferSize(0), m_AllocCount(0), m_AllocOffset(0), m_OverheadSize(0)
{
}

// 0x00731AEC (name after StepSequenceJob::Trace)
void nn::pia::common::ZlibCompressor::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn
