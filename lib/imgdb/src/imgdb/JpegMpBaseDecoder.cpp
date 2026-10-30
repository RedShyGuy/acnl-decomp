#include <cstring>

#include "imgdb/JpegMpBaseDecoder.h"
#include "imgdb/SysMakerNote.h"

namespace imgdb {

namespace {

// the index of the maker note of the system in the Exif data
const u32 SYS_MAKER_NOTE_INDEX = 1;

} // namespace

// 0x005AAFF4 | nintendogs:bytes [tier B]
int imgdb::JpegMpBaseDecoder::ComputeShrinkLevel(int& width, int& height, int maxWidth, int maxHeight)
{
    int level = 0;
    const int baseWidth = width;
    const int baseHeight = height;
    while (width > maxWidth || height > maxHeight)
    {
        level++;
        width = (baseWidth + (1 << level) - 1) >> level;
        height = (baseHeight + (1 << level) - 1) >> level;
    }
    return level;
}

// "YYYY:MM:DD hh:mm:ss" of the Exif data
// 0x00755B94 | nintendogs:bytes [tier B]
bool imgdb::JpegMpBaseDecoder::GetDateTime(nn::fnd::DateTime& dateTime) const
{
    char string[20];
    if (m_pDecoder->GetLastDateTime(string) != sizeof(string))
    {
        return false;
    }
    DateTimeSeconds seconds;
    if (!seconds.ConvertFromDateTimeString(string))
    {
        return false;
    }
    dateTime = seconds.ConvertToNNDateTime();
    return true;
}

// 0x00755BEC | nintendogs:bytes [tier B]
bool imgdb::JpegMpBaseDecoder::GetSysMakerNote(SysMakerNote& note) const
{
    if (m_pDecoder->GetLastMakerNotePointer(SYS_MAKER_NOTE_INDEX) == NULL ||
        m_pDecoder->GetLastMakerNoteSize(SYS_MAKER_NOTE_INDEX) > sizeof(SysMakerNote))
    {
        return false;
    }
    std::memcpy(&note, m_pDecoder->GetLastMakerNotePointer(SYS_MAKER_NOTE_INDEX), sizeof(SysMakerNote));
    return note.m_Magic == SysMakerNote::MAGIC;
}

// 0x00755C5C | nintendogs:bytes [tier B]
const SysMakerNote* imgdb::JpegMpBaseDecoder::GetSysMakerNote() const
{
    SysMakerNote note;
    if (!GetSysMakerNote(note))
    {
        return NULL;
    }
    return reinterpret_cast<const SysMakerNote*>(m_pDecoder->GetLastMakerNotePointer(SYS_MAKER_NOTE_INDEX));
}

} // namespace imgdb
