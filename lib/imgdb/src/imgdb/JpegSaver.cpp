#include "imgdb/ImageDatabase.h"
#include "imgdb/JpegDecoder.h"
#include "imgdb/JpegEncoder.h"
#include "imgdb/JpegSaver.h"
#include "imgdb/util/util_Api.h"

namespace imgdb {

namespace {

// the estimated size of a JPEG photo for the free space check
const u32 PICTURE_SIZE = 204800;

} // namespace

// 0x005B11AC | nintendogs:bytes-fuzzy [tier B]
Result imgdb::JpegSaver::Save()
{
    ImageDatabase* database = ImageDatabase::GetInstance();
    util::ConvertRealToVirtualPath(m_pPath, PATH_SIZE, L"");
    Result result = ValidateParam();
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    const SysMakerNote* note = m_pSysMakerNote;
    DateTimeSeconds now = DateTimeSeconds::GetCurrentDateTime();
    JpegEncoder encoder(m_pRaw, m_Format, m_Width, m_Height);
    encoder.m_IsRetryable = m_IsRetryable;
    encoder.m_Sampling = m_Sampling;
    encoder.m_Quality = m_Quality;
    encoder.SetUserMakerNoteData(m_pUserMakerNote, m_UserMakerNoteSize);
    encoder.SetSysMakerNote(*note);
    encoder.m_DateTime = now.ConvertToNNDateTime();
    char titleUniqueId[6];
    JpegMpBaseEncoder::ConvTitleUniqueIdToString(titleUniqueId, sizeof(titleUniqueId), note->m_TitleUniqueId);
    encoder.m_pSoftware = titleUniqueId;
    if (!encoder.Encode(false))
    {
        return encoder.m_Result;
    }
    result = CheckSdCard();
    if (result != RESULT_SUCCESS)
    {
        return result;
    }
    ImageDbRecordLink link;
    result = database->LoadDatabase(STORAGE_TYPE_SD) == RESULT_SUCCESS ? RESULT_SUCCESS
                                                                         : database->CreateDatabase(STORAGE_TYPE_SD);
    if (result == RESULT_SUCCESS)
    {
        if (database->GetEmptyImageNum(STORAGE_TYPE_SD) <= 0)
        {
            result = RESULT_DATABASE_FULL;
        }
        else if (util::EstimateRemainPictureNum(STORAGE_TYPE_SD, PICTURE_SIZE, false) == 0)
        {
            result = RESULT_FS_NOT_ENOUGH_SPACE;
        }
        else
        {
            ImageDbRecordParam param;
            StoreRecordParam(param, now);
            result = database->RegisterJpeg(STORAGE_TYPE_SD, encoder.m_pOutput, encoder.m_EncodedSize, param, &link);
            if (result == RESULT_SUCCESS)
            {
                wchar_t path[PATH_SIZE];
                const ImageDbRecord* record = database->FindRecord(link);
                util::CreateImagePath(path, PATH_SIZE, STORAGE_TYPE_SD, *record->GetImageInfo(), false);
                util::ConvertRealToVirtualPath(m_pPath, PATH_SIZE, path);
                return RESULT_SUCCESS;
            }
        }
    }
    return ConvertSaveResult(result);
}

// 0x005B14FC | nintendogs:bytes [tier B]
void imgdb::JpegSaver::SetRaw(const void* data, int width, int height, nn::jpeg::CTR::PixelFormat format)
{
    m_pRaw = data;
    m_Width = width;
    m_Height = height;
    m_Format = format;
}

// 0x005B1510 | nintendogs:bytes [tier B]
imgdb::JpegSaver::JpegSaver()
    : m_Quality(80), m_Sampling(nn::jpeg::CTR::PIXEL_SAMPLING_YUV422), m_IsRetryable(true), m_pRaw(NULL), m_Width(0),
      m_Height(0), m_Format(nn::jpeg::CTR::PIXEL_FORMAT_COUNT), m_pPath(NULL)
{
    m_pPath = static_cast<wchar_t*>(GetAllocator()->Allocate(PATH_SIZE * sizeof(wchar_t), 32));
    util::ConvertRealToVirtualPath(m_pPath, PATH_SIZE, L"");
    m_IsRetryable = true;
    m_Quality = 80;
    m_Sampling = nn::jpeg::CTR::PIXEL_SAMPLING_YUV422;
}

// 0x005B15FC
// 0x005B15A0 (deleting dtor)
imgdb::JpegSaver::~JpegSaver()
{
    GetAllocator()->Free(m_pPath);
}

// (the image must fit the limits of JpegDecoder without shrinking more than twice)
// 0x007567A4 | nintendogs:bytes [tier B]
Result imgdb::JpegSaver::ValidateParam() const
{
    if (m_pRaw == NULL)
    {
        return RESULT_INVALID_PARAM;
    }
    int width = m_Width;
    int height = m_Height;
    int level = JpegMpBaseDecoder::ComputeShrinkLevel(width, height, 1024, 768);
    if (width < 160 || height < 120 || width > 1024 || height > 768 || level > 2)
    {
        return RESULT_INVALID_PARAM;
    }
    if ((m_Width & 7) != 0 || (m_Height & 7) != 0)
    {
        return RESULT_INVALID_PARAM;
    }
    if ((m_pSysMakerNote->m_HandleTypeBit & (1 << 4)) != 0 &&
        (m_Width < 400 || m_Height < 240 || m_Width > 1024 || m_Height > 768))
    {
        return RESULT_INVALID_PARAM;
    }
    if (m_Quality < 1 || m_Quality > 100)
    {
        return RESULT_INVALID_PARAM;
    }
    if ((m_pSysMakerNote->m_BodyIdValidBits & 1) == 0 || !m_HasTitleUniqueId)
    {
        return RESULT_INVALID_PARAM;
    }
    return RESULT_SUCCESS;
}

} // namespace imgdb
