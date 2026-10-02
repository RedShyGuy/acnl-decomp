#pragma once

#include "decomp.h"

namespace imgdb {
class FaceInfo
{
public:
    void Initialize(); // 0x005B0978 | nintendogs:bytes [tier B]
    void Validate() const; // 0x00756210 | nintendogs:bytes [tier B]
};
} // namespace imgdb
