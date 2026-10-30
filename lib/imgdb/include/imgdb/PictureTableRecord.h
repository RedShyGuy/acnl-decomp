#pragma once

#include "imgdb/FaceInfo.h"
#include "imgdb/ImageInfo.h"

namespace imgdb {

// a record of the picture table: the image and its faces
class PictureTableRecord
{
public:
    static const int FACE_NUM = 4;

    void Initialize(ImageKind kind);

    ImageInfo m_ImageInfo;        // +0x00
    FaceInfo m_Faces[FACE_NUM];   // +0x50
};
ASSERT_SIZE(PictureTableRecord, 0x98);

} // namespace imgdb
