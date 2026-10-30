#include "imgdb/JpegDecoder.h"

namespace imgdb {

// 0x005A4A20 | nintendogs:bytes [tier B]
bool imgdb::JpegDecoder::ExtractExif(bool isThumbnail)
{
    ReleaseOutput();
    m_DecodedSize = 0;
    if (!m_Decoder.ExtractExif(m_pData, m_DataSize, isThumbnail))
    {
        return false;
    }
    m_ShrinkLevel = 0;
    if (isThumbnail ? m_IsThumbnailShrinkable : m_IsShrinkable)
    {
        int width = m_Decoder.GetLastWidth();
        int height = m_Decoder.GetLastHeight();
        m_ShrinkLevel = ComputeShrinkLevel(width, height, isThumbnail ? m_ThumbnailMaxWidth : m_MaxWidth,
                                           isThumbnail ? m_ThumbnailMaxHeight : m_MaxHeight);
    }
    return true;
}

// 0x005A4B18 | nintendogs:bytes [tier B]
bool imgdb::JpegDecoder::ExtractExif()
{
    return ExtractExif(false);
}

// (falls into Decode(format, false))
// 0x005A4BFC | nintendogs:bytes [tier B]
bool imgdb::JpegDecoder::Decode(nn::jpeg::CTR::PixelFormat format)
{
    return Decode(format, false);
}

// (the image must lie within the limits after shrinking)
// 0x005A4C04 | nintendogs:bytes [tier B]
bool imgdb::JpegDecoder::Decode(nn::jpeg::CTR::PixelFormat format, bool isThumbnail)
{
    ReleaseOutput();
    m_Format = format;
    m_DecodedSize = 0;
    if (m_Decoder.ExtractExif(m_pData, m_DataSize, isThumbnail))
    {
        int realWidth = m_Decoder.GetLastWidth();
        int realHeight = m_Decoder.GetLastHeight();
        int width = realWidth;
        int height = realHeight;
        m_ShrinkLevel = 0;
        if (isThumbnail ? m_IsThumbnailShrinkable : m_IsShrinkable)
        {
            m_ShrinkLevel = ComputeShrinkLevel(width, height, isThumbnail ? m_ThumbnailMaxWidth : m_MaxWidth,
                                               isThumbnail ? m_ThumbnailMaxHeight : m_MaxHeight);
        }
        u32 decodeWidth = GetWidth();
        u32 decodeHeight = GetHeight();
        if ((isThumbnail ? m_ThumbnailMinWidth : m_MinWidth) <= realWidth &&
            (isThumbnail ? m_ThumbnailMaxWidth : m_MaxWidth) >= width &&
            (isThumbnail ? m_ThumbnailMinHeight : m_MinHeight) <= realHeight &&
            (isThumbnail ? m_ThumbnailMaxHeight : m_MaxHeight) >= height &&
            m_ShrinkLevel <= (isThumbnail ? m_ThumbnailMaxShrinkLevel : m_MaxShrinkLevel))
        {
            bool isBufferReady = true;
            if (m_IsAllocateOutput)
            {
                m_OutputSize = nn::jpeg::CTR::JpegMpDecoder::GetDstBufferSize(decodeWidth, decodeHeight, format);
                m_pOutput = GetAllocator()->Allocate(m_OutputSize, 32);
            }
            else if (m_OutputSize < nn::jpeg::CTR::JpegMpDecoder::GetDstBufferSize(decodeWidth, decodeHeight, format))
            {
                isBufferReady = false;
            }
            if (isBufferReady)
            {
                m_Decoder.SetDstStride(m_Stride);
                if (m_ShrinkLevel != 0)
                {
                    m_DecodedSize = m_Decoder.StartJpegDecoderShrink(
                        m_pOutput, m_OutputSize, m_pData, m_DataSize, decodeWidth << m_ShrinkLevel,
                        decodeHeight << m_ShrinkLevel, format, isThumbnail, m_ShrinkLevel);
                }
                else
                {
                    m_DecodedSize = m_Decoder.StartJpegDecoder(m_pOutput, m_OutputSize, m_pData, m_DataSize, width,
                                                               height, format, isThumbnail);
                }
            }
        }
    }
    if (m_DecodedSize == 0 && m_IsAllocateOutput)
    {
        GetAllocator()->Free(m_pOutput);
        m_pOutput = NULL;
        m_OutputSize = 0;
    }
    return m_DecodedSize != 0;
}

// 0x005A4F00 | nintendogs:bytes [tier B]
imgdb::JpegDecoder::JpegDecoder(const void* data, u32 size)
    : JpegMpBaseDecoder(&m_Decoder), m_pWork(NULL), m_pData(static_cast<const u8*>(data)), m_DataSize(size),
      m_MinWidth(160), m_MinHeight(120), m_MaxWidth(1024), m_MaxHeight(768), m_ThumbnailMinWidth(0),
      m_ThumbnailMinHeight(0), m_ThumbnailMaxWidth(320), m_ThumbnailMaxHeight(240), m_Stride(0), m_MaxShrinkLevel(2),
      m_ThumbnailMaxShrinkLevel(2), m_IsShrinkable(true), m_IsThumbnailShrinkable(true), m_IsAllocateOutput(true),
      m_pOutput(NULL), m_OutputSize(0), m_DecodedSize(0), m_Format(nn::jpeg::CTR::PIXEL_FORMAT_COUNT), m_ShrinkLevel(0)
{
    size_t workSize = nn::jpeg::CTR::JpegMpDecoder::GetWorkBufferSize();
    m_pWork = GetAllocator()->Allocate(workSize, 32);
    m_Decoder.Initialize(m_pWork, workSize);
}

// 0x005A5044 | nintendogs:bytes
// 0x005A4FD4 (deleting dtor)
imgdb::JpegDecoder::~JpegDecoder()
{
    if (m_IsAllocateOutput && m_pOutput != NULL)
    {
        GetAllocator()->Free(m_pOutput);
    }
    m_Decoder.Finalize();
    GetAllocator()->Free(m_pWork);
}

} // namespace imgdb
