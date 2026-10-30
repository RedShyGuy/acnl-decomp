#include "imgdb/JpegEncoder.h"

namespace imgdb {

namespace {

const u32 OUTPUT_SIZE = 204800;
const u32 TWL_OUTPUT_SIZE = 204799;
// nn::jpeg::CTR::JpegMpEncoder::GetLastError: the output buffer is too small
const s32 ENCODER_ERROR_OUTPUT_TOO_SMALL = -9;

} // namespace

// 0x005A50B0 | nintendogs:bytes-fuzzy [tier B]
bool imgdb::JpegEncoder::Encode(bool isTwl)
{
    m_Result = RESULT_NOT_READY;
    if (m_pOutput != NULL)
    {
        GetAllocator()->Free(m_pOutput);
        m_pOutput = NULL;
    }
    m_OutputSize = 0;
    m_EncodedSize = 0;
    m_OutputSize = isTwl ? TWL_OUTPUT_SIZE : OUTPUT_SIZE;
    m_pOutput = GetAllocator()->Allocate(m_OutputSize, 32);

    int quality = m_Quality;
    bool isRetry;
    do
    {
        isRetry = false;
        nn::jpeg::CTR::JpegMpEncoder encoder;
        size_t workSize = nn::jpeg::CTR::JpegMpEncoder::GetWorkBufferSize(0);
        void* work = GetAllocator()->Allocate(workSize, 32);
        encoder.Initialize(work, workSize, 0);
        SetDateTime(encoder);
        u8 nintendoData[8] = {};
        bool isAddThumbnail;
        if (isTwl)
        {
            encoder.SetNintendoData(nintendoData);
            isAddThumbnail = true;
        }
        else
        {
            SetMakerNotes(encoder);
            if (m_IsAddThumbnail)
            {
                SetThumbnail(encoder, m_Width, m_Height);
            }
            isAddThumbnail = m_IsAddThumbnail;
        }
        m_EncodedSize = encoder.StartJpegEncoder(static_cast<u8*>(m_pOutput), m_OutputSize, m_pData, m_Width, m_Height,
                                                 quality, m_Sampling, m_Format, isAddThumbnail);
        if (m_EncodedSize != 0)
        {
            m_Result = RESULT_SUCCESS;
        }
        else if (encoder.GetLastError() == ENCODER_ERROR_OUTPUT_TOO_SMALL)
        {
            if (quality > 1)
            {
                quality -= 10;
                if (quality < 1)
                {
                    quality = 1;
                }
                if (m_IsRetryable)
                {
                    isRetry = true;
                }
            }
            if (!isRetry)
            {
                GetAllocator()->Free(m_pOutput);
                m_pOutput = NULL;
                m_OutputSize = 0;
                m_Result = RESULT_FILE_TOO_LARGE;
            }
        }
        else
        {
            GetAllocator()->Free(m_pOutput);
            m_pOutput = NULL;
            m_OutputSize = 0;
            m_Result = RESULT_INVALID_PARAM;
        }
        encoder.Finalize();
        GetAllocator()->Free(work);
    } while (isRetry);
    if (m_EncodedSize != 0)
    {
        m_Result = RESULT_SUCCESS;
    }
    return m_EncodedSize != 0;
}

// 0x005A5478 | nintendogs:bytes [tier B]
imgdb::JpegEncoder::JpegEncoder(const void* data, nn::jpeg::CTR::PixelFormat format, int width, int height)
    : m_Result(RESULT_NOT_READY), m_IsRetryable(true), m_pData(data), m_Format(format), m_Width(width),
      m_Height(height), m_pOutput(NULL), m_OutputSize(0), m_EncodedSize(0)
{
}

// 0x005A5594 | nintendogs:bytes
// 0x005A5550 (deleting dtor)
imgdb::JpegEncoder::~JpegEncoder()
{
    if (m_pOutput != NULL)
    {
        GetAllocator()->Free(m_pOutput);
    }
}

} // namespace imgdb
