#include "nn/nex/nex_ZLibCompression.h"
#include "nn/nex/nex_MemoryManager.h"

#include <cstring>
#include <new>

namespace nn {
namespace nex {
// 0x0037EC3C (name is ours)
nn::nex::ZLibCompression::ZLibCompression()
{
    InitializeStreams(DEFAULT_WINDOW_BITS, DEFAULT_MEMORY_LEVEL);
}

// 0x0037EB10 | fefates:bytes-fuzzy [tier B]
nn::nex::ZLibCompression::ZLibCompression(int windowBits, int memoryLevel)
{
    if (windowBits == -1) {
        windowBits = DEFAULT_WINDOW_BITS;
    }
    if (memoryLevel == -1) {
        memoryLevel = DEFAULT_MEMORY_LEVEL;
    }
    InitializeStreams(windowBits, memoryLevel);
}

// 0x0037EE08 slot 0x00
// 0x0037ED54 (deleting dtor)
nn::nex::ZLibCompression::~ZLibCompression()
{
    nexZlib_inflateEnd(&m_pStreams->m_Inflate);
    nexZlib_deflateEnd(&m_pStreams->m_Deflate);
    if (m_pStreams != nullptr) {
        MemoryManager::FreeInline(m_pStreams);
    }
    m_pStreams = nullptr;
}

// 0x0037E804 slot 0x08 | slot vf_0x08 of nn::nex::CompressionAlgorithm
int nn::nex::ZLibCompression::CompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    z_stream& stream = m_pStreams->m_Deflate;
    nexZlib_deflateReset(&stream);
    // the ratio byte comes first
    if (pOutput->m_Size - pOutput->m_HeaderSize < input.m_ContentSize + 1) {
        pOutput->ResizeByRealSize(input.m_ContentSize + 1 + pOutput->m_HeaderSize);
    }
    stream.avail_out = pOutput->m_Size - pOutput->m_HeaderSize;
    stream.next_out = pOutput->m_pData + pOutput->m_HeaderSize;
    stream.avail_in = input.m_ContentSize;
    stream.next_in = input.m_pData + input.m_HeaderSize;
    stream.next_out++;
    stream.avail_out--;
    if (nexZlib_deflate(&stream, Z_FINISH) == Z_STREAM_END && stream.total_out < input.m_ContentSize) {
        f32 ratio = static_cast<f32>(input.m_ContentSize) / static_cast<f32>(stream.total_out) + 1.0f;
        pOutput->m_ContentSize = stream.total_out + 1;
        pOutput->m_pData[pOutput->m_HeaderSize] = ratio > 255.0f ? RATIO_MAX : static_cast<u8>(ratio);
    } else {
        pOutput->m_ContentSize = input.m_ContentSize + 1;
        u8* pData = pOutput->m_pData + pOutput->m_HeaderSize;
        *pData = RATIO_UNCOMPRESSED;
        memcpy(pData + 1, input.m_pData + input.m_HeaderSize, input.m_ContentSize);
    }
    return 0;
}

// 0x0037E974 slot 0x0C | fefates:bytes-fuzzy
int nn::nex::ZLibCompression::DecompressImpl(const nn::nex::Buffer& input, nn::nex::Buffer* pOutput)
{
    u8 ratio = input.m_pData[input.m_HeaderSize];
    int result = 0;
    if (ratio == RATIO_UNCOMPRESSED) {
        pOutput->m_ContentSize = input.m_ContentSize - 1;
        memcpy(pOutput->m_pData + pOutput->m_HeaderSize, input.m_pData + input.m_HeaderSize + 1, input.m_ContentSize - 1);
        return result;
    }
    bool isFailed = false;
    if (ratio == RATIO_MAX) {
        pOutput->ResizeByRealSize(input.m_ContentSize * MAX_RATIO_FACTOR + pOutput->m_HeaderSize);
    } else {
        u32 size = input.m_ContentSize * ratio;
        if (size > pOutput->m_Size - pOutput->m_HeaderSize) {
            pOutput->ResizeByRealSize(pOutput->m_HeaderSize + size * 2);
        }
    }
    // a larger output buffer when the estimate was too small (once)
    u32 count = 0;
    int zResult;
    do {
        z_stream& stream = m_pStreams->m_Inflate;
        nexZlib_inflateReset(&stream);
        stream.avail_out = pOutput->m_Size - pOutput->m_HeaderSize;
        stream.next_out = pOutput->m_pData + pOutput->m_HeaderSize;
        stream.avail_in = input.m_ContentSize;
        stream.next_in = input.m_pData + input.m_HeaderSize;
        stream.next_in = input.m_pData + input.m_HeaderSize + 1;
        stream.avail_in--;
        zResult = nexZlib_inflate(&stream, Z_FINISH);
        if ((stream.avail_out != 0 || zResult != Z_OK) && zResult != Z_BUF_ERROR) {
            isFailed = true;
            break;
        }
        pOutput->ResizeByRealSize(pOutput->m_HeaderSize + (pOutput->m_Size - pOutput->m_HeaderSize) * 2);
        count++;
    } while (count < 2 && zResult == Z_OK);
    if (zResult == Z_STREAM_END) {
        pOutput->m_ContentSize = m_pStreams->m_Inflate.total_out;
    } else {
        result = (zResult == Z_OK && !isFailed) ? 2 : 1;
    }
    return result;
}

} // namespace nex
} // namespace nn
