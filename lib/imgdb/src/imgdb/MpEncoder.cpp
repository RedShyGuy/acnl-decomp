#include "imgdb/MpEncoder.h"

namespace imgdb {

namespace {

const u32 OUTPUT_SIZE = 409600;
const u32 IMAGE_NUM = 2;
// nn::jpeg::CTR::JpegMpEncoder::GetLastError: the output buffer is too small
const s32 ENCODER_ERROR_OUTPUT_TOO_SMALL = -9;

} // namespace

// (when the file gets too large, the quality is halved for the left image, at least by 10)
// 0x005B1BE8 | nintendogs:bytes-fuzzy [tier B]
bool imgdb::MpEncoder::Encode()
{
    m_Result = RESULT_NOT_READY;
    if (m_pOutput != NULL)
    {
        GetAllocator()->Free(m_pOutput);
        m_pOutput = NULL;
    }
    m_OutputSize = OUTPUT_SIZE;
    m_EncodedSize = 0;
    m_pOutput = GetAllocator()->Allocate(m_OutputSize, 32);

    int quality = m_Quality;
    bool isRetry;
    do
    {
        isRetry = false;
        nn::jpeg::CTR::JpegMpEncoder encoder;
        size_t workSize = nn::jpeg::CTR::JpegMpEncoder::GetWorkBufferSize(IMAGE_NUM);
        void* work = GetAllocator()->Allocate(workSize, 32);
        encoder.Initialize(work, workSize, IMAGE_NUM);
        for (u32 i = 0; i < IMAGE_NUM; i++)
        {
            SetDateTime(encoder);
            SetMakerNotes(encoder);
            if (m_IsAddThumbnail)
            {
                SetThumbnail(encoder, m_Width, m_Height);
            }
            if (i == 0)
            {
                m_EncodedSize = encoder.StartMpEncoderFirst(
                    static_cast<u8*>(m_pOutput), m_OutputSize, m_pDataL, m_Width, m_Height, quality, m_Sampling, m_Format,
                    true, IMAGE_NUM, nn::jpeg::CTR::MP_TYPE_CODE_MULTI_FRAME_DISPARITY, false, false);
            }
            else
            {
                m_EncodedSize = encoder.StartMpEncoderNext(m_pDataR, m_Width, m_Height, quality, m_Sampling, m_Format,
                                                           true, nn::jpeg::CTR::MP_TYPE_CODE_MULTI_FRAME_DISPARITY,
                                                           false);
            }
            if (m_EncodedSize == 0)
            {
                if (encoder.GetLastError() == ENCODER_ERROR_OUTPUT_TOO_SMALL)
                {
                    if (quality > 1)
                    {
                        quality -= i == 0 && quality / 2 >= 10 ? quality / 2 : 10;
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
                break;
            }
            m_Result = RESULT_SUCCESS;
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

// 0x005B200C | nintendogs:bytes [tier B]
imgdb::MpEncoder::MpEncoder(const void* dataL, const void* dataR, nn::jpeg::CTR::PixelFormat format, int width,
                            int height)
    : m_Result(RESULT_NOT_READY), m_IsRetryable(true), m_pDataL(dataL), m_pDataR(dataR), m_Format(format),
      m_Width(width), m_Height(height), m_pOutput(NULL), m_OutputSize(0), m_EncodedSize(0)
{
}

// 0x005B212C | nintendogs:bytes
// 0x005B20E8 (deleting dtor)
imgdb::MpEncoder::~MpEncoder()
{
    if (m_pOutput != NULL)
    {
        GetAllocator()->Free(m_pOutput);
    }
}

} // namespace imgdb
