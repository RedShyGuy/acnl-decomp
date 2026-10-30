#include <cstring>

#include "imgdb/JpegMpBaseEncoder.h"

namespace imgdb {

// 0x005AB054 | nintendogs:bytes [tier B]
bool imgdb::JpegMpBaseEncoder::SetSysMakerNote(const void* data)
{
    SysMakerNote note;
    std::memcpy(&note, data, sizeof(note));
    if (note.m_Magic != SysMakerNote::MAGIC)
    {
        return false;
    }
    m_SysMakerNote = note;
    return true;
}

// (the same code as SetSysMakerNote(const void*))
// 0x005AB0B4 (name is ours)
bool imgdb::JpegMpBaseEncoder::SetSysMakerNote(const SysMakerNote& source)
{
    SysMakerNote note;
    std::memcpy(&note, &source, sizeof(note));
    if (note.m_Magic != SysMakerNote::MAGIC)
    {
        return false;
    }
    m_SysMakerNote = note;
    return true;
}

// (falls into SysMakerNote::SetShootingType)
// 0x005A5BA4 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetSysMakerNoteShootingType(ShootingType type)
{
    m_SysMakerNote.SetShootingType(type);
}

// 0x005AB114 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetUserMakerNoteData(const void* data, u32 size)
{
    m_pUserMakerNote = data;
    m_UserMakerNoteSize = size;
}

// 0x005AB120 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetSysMakerNoteBodyId(BodyIdType type, u32 id)
{
    m_SysMakerNote.m_BodyIds[type] = id;
    m_SysMakerNote.m_BodyIdValidBits |= 1 << type;
}

// (the size is not checked: 6 characters with the terminator)
// 0x005AB144 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::ConvTitleUniqueIdToString(char* buffer, int size, u32 titleUniqueId)
{
    for (int i = 4; i >= 0; i--)
    {
        u32 digit = titleUniqueId & 0xF;
        buffer[i] = digit <= 9 ? '0' + digit : 'a' + digit - 10;
        titleUniqueId >>= 4;
    }
    buffer[5] = '\0';
}

// 0x005AB1D8 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetSysMakerNoteCreateSecond(const DateTimeSeconds& seconds)
{
    m_SysMakerNote.m_CreateSecond = seconds;
}

// 0x005AB1E8 | nintendogs:bytes [tier B]
void imgdb::JpegMpBaseEncoder::SetSysMakerNoteTitleUniqueId(u32 titleUniqueId)
{
    m_SysMakerNote.m_TitleUniqueId = titleUniqueId;
    m_HasTitleUniqueId = true;
}

} // namespace imgdb
