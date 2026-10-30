#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// a face found in an image: an id (0 = none) and its rectangle
class FaceInfo
{
public:
    static const u16 ID_MAX = 1000;

    void Initialize();
    bool Validate() const;

    u16 m_Id : 10;  // +0x00
    u16 : 6;
    u16 m_Left;     // +0x02
    u16 m_Top;      // +0x04
    u16 m_Right;    // +0x06
    u16 m_Bottom;   // +0x08
    u16 _0A[4];     // +0x0A
};
ASSERT_SIZE(FaceInfo, 0x12);

} // namespace imgdb
