#include "imgdb/UserMakerNote.h"

namespace imgdb {

// 0x005A7734 | nintendogs:callgraph [tier C]
imgdb::FaceInfo* imgdb::UserMakerNote::GetFaceInfo(int index)
{
    return &m_Faces[index];
}

} // namespace imgdb
