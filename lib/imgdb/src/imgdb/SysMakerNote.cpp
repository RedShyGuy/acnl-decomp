#include <cstring>

#include "imgdb/ImageDbRecordParam.h"
#include "imgdb/SysMakerNote.h"

namespace imgdb {

// (the attributes of a new database record)
// 0x005A5AE4 | nintendogs:bytes [tier B]
void imgdb::SysMakerNote::Initialize()
{
    std::memset(this, 0, sizeof(*this));
    m_Magic = MAGIC;
    _28 = 0.0f;
    m_CreateSecond.SetCurrentDateTime();
    ImageDbRecordParam param;
    param.Initialize(IMAGE_KIND_NONE);
    m_SaveProcessType = param.m_SaveProcessType;
    m_HandleTypeBit = param.m_HandleTypeBit;
    SetShootingType(static_cast<ShootingType>(param.m_ShootingType));
    SetDistinctionTypeBit(static_cast<DistinctionTypeBit>(param.m_DistinctionTypeBit));
}

// 0x005A5BAC | nintendogs:bytes [tier B]
void imgdb::SysMakerNote::SetShootingType(ShootingType type)
{
    m_ShootingType = type;
}

// 0x005A5C18 | nintendogs:bytes [tier B]
void imgdb::SysMakerNote::SetDistinctionTypeBit(DistinctionTypeBit bit)
{
    m_DistinctionTypeBit = bit;
}

} // namespace imgdb
