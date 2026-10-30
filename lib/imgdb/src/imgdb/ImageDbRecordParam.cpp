#include <cstring>

#include "imgdb/ImageDbRecordParam.h"

namespace imgdb {

namespace {

const u8 DEFAULT_HANDLE_TYPE_BIT = 4;
const u8 SHOOTING_TYPE_MOVIE = 9;

} // namespace

// (the shooting type of a still image is 0, of a movie 9)
// 0x005AB260 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::Initialize(ImageKind kind)
{
    std::memset(this, 0, sizeof(*this));
    m_DateTime.Initialize();
    m_HandleTypeBit = DEFAULT_HANDLE_TYPE_BIT;
    if (kind == IMAGE_KIND_JPEG || kind == IMAGE_KIND_MP)
    {
        m_ShootingType = 0;
    }
    else if (kind == IMAGE_KIND_AVI)
    {
        m_ShootingType = SHOOTING_TYPE_MOVIE;
    }
}

// 0x005AB2A8 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::Initialize()
{
    std::memset(this, 0, sizeof(*this));
    m_DateTime.Initialize();
    m_HandleTypeBit = DEFAULT_HANDLE_TYPE_BIT;
    m_ShootingType = 0;
}

// 0x005AB2D4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::ResetBodyId(BodyIdType type)
{
    m_BodyIdValidBits &= ~(1 << type);
    m_BodyIds[type] = 0;
}

// 0x005AB2FC | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordParam::SetBodyId(BodyIdType type, u32 id)
{
    m_BodyIdValidBits |= 1 << type;
    m_BodyIds[type] = id;
}

// 0x00755CDC | nintendogs:bytes [tier B]
bool imgdb::ImageDbRecordParam::IsValidBodyId(BodyIdType type) const
{
    return (m_BodyIdValidBits & (1 << type)) != 0;
}

// 0x00755CF0 | nintendogs:callgraph [tier C]
u32 imgdb::ImageDbRecordParam::GetBodyId(BodyIdType type) const
{
    return m_BodyIds[type];
}

} // namespace imgdb
