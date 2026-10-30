#include "imgdb/PictureTableRecord.h"

namespace imgdb {

// (the kind is not used)
// 0x005AB330 | nintendogs:bytes [tier B]
void imgdb::PictureTableRecord::Initialize(ImageKind kind)
{
    m_ImageInfo.Initialize();
    for (int i = 0; i < FACE_NUM; i++)
    {
        m_Faces[i].Initialize();
    }
}

} // namespace imgdb
