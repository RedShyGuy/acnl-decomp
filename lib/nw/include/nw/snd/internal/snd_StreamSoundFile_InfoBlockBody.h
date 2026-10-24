#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

class nw::snd::internal::StreamSoundFile::InfoBlockBody
{
public:
    // the parts (NULL: another type)
    const TrackInfoTable* GetTrackInfoTable() const; // 0x0073FBB8 | nintendogs:bytes [tier A]
    const StreamSoundInfo* GetStreamSoundInfo() const; // 0x0073FBD4 | nintendogs:bytes [tier A]
    const ChannelInfoTable* GetChannelInfoTable() const; // 0x0073FBEC | nintendogs:bytes [tier A]

    BinaryReference streamSoundInfo;  // 0x00
    BinaryReference trackInfoTable;   // 0x08
    BinaryReference channelInfoTable; // 0x10
};

// a BinaryReferenceTable of TrackInfo
class nw::snd::internal::StreamSoundFile::TrackInfoTable
{
public:
    // the track of an index (NULL: none)
    const TrackInfo* GetTrackInfo(u32 index) const; // 0x0073FC08 | nintendogs:bytes [tier A]

    BinaryReferenceTable table; // 0x0
};

// a BinaryReferenceTable of ChannelInfo
class nw::snd::internal::StreamSoundFile::ChannelInfoTable
{
public:
    // the channel of an index (NULL: none)
    const ChannelInfo* GetChannelInfo(u32 index) const; // 0x0073FC3C (name after GetTrackInfo)

    BinaryReferenceTable table; // 0x0
};
