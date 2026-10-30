#pragma once

#include "imgdb/FaceInfo.h"
#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the maker note of the camera application in the Exif data of its photos: the faces it found (names are ours)
class UserMakerNote
{
public:
    static const u16 VERSION = 1;
    static const int FACE_NUM = 4;
    // the title unique id of the application that writes this note
    static const u32 TITLE_UNIQUE_ID = 0x204;

    // the part that tells the version
    struct Header
    {
        u16 m_Version;  // +0x00
        u8 _02[6];      // +0x02
    };

    FaceInfo* GetFaceInfo(int index);

    Header m_Header;             // +0x00
    FaceInfo m_Faces[FACE_NUM];  // +0x08
};
ASSERT_SIZE(UserMakerNote, 0x50);

} // namespace imgdb
