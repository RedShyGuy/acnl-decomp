#include <cstring>

#include "imgdb/MpDecoder.h"

namespace imgdb {

// (falls into ExtractExif(false))
// 0x005B1654 | nintendogs:bytes [tier B]
bool imgdb::MpDecoder::ExtractExif()
{
    return ExtractExif(false);
}

// 0x005B165C | nintendogs:bytes [tier B]
bool imgdb::MpDecoder::ExtractExif(bool isThumbnail)
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

// (the JPEG is put together from two parts of the file)
// 0x005B177C | nintendogs:bytes [tier B]
bool imgdb::MpDecoder::ExtractJpeg(int index, void* buffer, u32 size, u32& extractedSize)
{
    nn::jpeg::CTR::MpIndex mpIndex;
    if (!IsMultiPicture(mpIndex))
    {
        return false;
    }
    nn::jpeg::CTR::MpEntry entry;
    if (!m_Decoder.GetMpEntry(&entry, &mpIndex, index))
    {
        return false;
    }
    if (entry.m_Offset + entry.m_Size > m_DataSize)
    {
        return false;
    }
    nn::jpeg::CTR::MpRegionsToBuildJpegData regions;
    if (!m_Decoder.GetMpRegionsToBuildJpegData(&regions, m_pData + entry.m_Offset, entry.m_Size))
    {
        return false;
    }
    extractedSize = regions.m_Size1 + regions.m_Size2;
    if (extractedSize > size)
    {
        return false;
    }
    std::memcpy(buffer, regions.m_Data1, regions.m_Size1);
    std::memcpy(static_cast<u8*>(buffer) + regions.m_Size1, regions.m_Data2, regions.m_Size2);
    return true;
}

// 0x005B1864 | nintendogs:bytes [tier B]
bool imgdb::MpDecoder::ExtractJpegL(void* buffer, u32 size, u32& extractedSize)
{
    nn::jpeg::CTR::MpIndex mpIndex;
    if (!IsMultiPicture(mpIndex))
    {
        return false;
    }
    return ExtractJpeg(0, buffer, size, extractedSize);
}

// 0x005B18D4 | nintendogs:bytes-fuzzy [tier B]
u32 imgdb::MpDecoder::ComputeExtractJpegSize()
{
    nn::jpeg::CTR::MpIndex mpIndex;
    if (!IsMultiPicture(mpIndex))
    {
        return 0;
    }
    u32 sizes[2];
    for (int i = 0; i < 2; i++)
    {
        nn::jpeg::CTR::MpEntry entry;
        if (!m_Decoder.GetMpEntry(&entry, &mpIndex, i))
        {
            return 0;
        }
        if (entry.m_Offset + entry.m_Size > m_DataSize)
        {
            return 0;
        }
        nn::jpeg::CTR::MpRegionsToBuildJpegData regions;
        if (!m_Decoder.GetMpRegionsToBuildJpegData(&regions, m_pData + entry.m_Offset, entry.m_Size))
        {
            return 0;
        }
        sizes[i] = regions.m_Size1 + regions.m_Size2;
    }
    return sizes[0] > sizes[1] ? sizes[0] : sizes[1];
}

// 0x005B19E4 | nintendogs:bytes [tier B]
imgdb::MpDecoder::MpDecoder(const void* data, u32 size)
    : JpegMpBaseDecoder(&m_Decoder), m_pWork(NULL), m_pData(static_cast<const u8*>(data)), m_DataSize(size),
      m_MinWidth(160), m_MinHeight(120), m_MaxWidth(1024), m_MaxHeight(768), m_ThumbnailMinWidth(0),
      m_ThumbnailMinHeight(0), m_ThumbnailMaxWidth(320), m_ThumbnailMaxHeight(240), m_Stride(0), m_MaxShrinkLevel(2),
      m_ThumbnailMaxShrinkLevel(2), m_IsShrinkable(true), m_IsThumbnailShrinkable(true), m_IsAllocateOutput(true),
      m_pOutputL(NULL), m_pOutputR(NULL), m_OutputSize(0), m_DecodedSize(0),
      m_Format(nn::jpeg::CTR::PIXEL_FORMAT_COUNT), m_ShrinkLevel(0)
{
    size_t workSize = nn::jpeg::CTR::JpegMpDecoder::GetWorkBufferSize();
    m_pWork = GetAllocator()->Allocate(workSize, 32);
    m_Decoder.Initialize(m_pWork, workSize);
}

// 0x005B1B54 | nintendogs:bytes
// 0x005B1ABC (deleting dtor)
imgdb::MpDecoder::~MpDecoder()
{
    if (m_IsAllocateOutput)
    {
        if (m_pOutputL != NULL)
        {
            GetAllocator()->Free(m_pOutputL);
        }
        if (m_pOutputR != NULL)
        {
            GetAllocator()->Free(m_pOutputR);
        }
    }
    m_Decoder.Finalize();
    GetAllocator()->Free(m_pWork);
}

} // namespace imgdb
