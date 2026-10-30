#include <cstring>

#include "imgdb/FaceInfo.h"

namespace imgdb {

// 0x005B0978 | nintendogs:bytes [tier B]
void imgdb::FaceInfo::Initialize()
{
    std::memset(this, 0, sizeof(*this));
}

// 0x00756210 | nintendogs:bytes [tier B]
bool imgdb::FaceInfo::Validate() const
{
    if (m_Id == 0)
    {
        return true;
    }
    if (static_cast<u32>(m_Id - 1) >= ID_MAX || m_Right < m_Left || m_Top > m_Bottom)
    {
        return false;
    }
    return true;
}

} // namespace imgdb
