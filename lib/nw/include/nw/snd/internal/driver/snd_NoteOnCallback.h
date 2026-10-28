#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Channel.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class SequenceSoundPlayer;

// a note of a sequence track for the bank (the struct name is from the reference symbols, the members
// are ours)
struct NoteOnInfo
{
    int prgNo;                               // 0x00
    int key;                                 // 0x04
    int velocity;                            // 0x08
    s32 length;                              // 0x0C (-1: until the note off)
    int initPan;                             // 0x10
    int priority;                            // 0x14
    Channel::ChannelCallback channelCallback; // 0x18
    u32 channelCallbackData;                 // 0x1C
};
ASSERT_SIZE(NoteOnInfo, 0x20);

// Makes the channel of a note of a sequence (the class name and NoteOn are from the reference symbols).
// RTTI N2nw3snd8internal6driver14NoteOnCallbackE @ 0x008D0A74
class NoteOnCallback
{
public:
    // (inline)
    NoteOnCallback() {}
    virtual ~NoteOnCallback() {} // slot 0x00

    // the channel of the note (NULL: none)
    virtual Channel* NoteOn(SequenceSoundPlayer* pPlayer, unsigned char bankIndex, const NoteOnInfo& info) = 0; // slot 0x08
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
