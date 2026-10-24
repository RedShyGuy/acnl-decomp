#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// A command from the game threads to the sound thread (the class and ProcessCommandList are from the
// reference symbols, the members and the names of the commands are ours). The commands of a flush
// are a list in the memory ring of the DriverCommandManager; the derived commands follow at 0x10.
struct DriverCommand
{
    // the commands (by what ProcessCommandList does with them; only those used so far are named)
    enum Id : u8
    {
        ID_DUMMY = 0,            // nothing (a flush without commands)
        ID_VOICE_SYNC = 58,      // VoiceManager::UpdateAllVoicesSync
    };

    // runs the commands of a list (in the sound thread)
    static void ProcessCommandList(DriverCommand* pList); // 0x004C7300 | nintendogs:callgraph [tier A]

    DriverCommand* next; // 0x0
    Id id;               // 0x4
    u8 padding[3];       // 0x5
    u32 tag;             // 0x8 the tag of the flush (the first command of a list)
    u32 memoryNext;      // 0xC the write index of the memory ring behind the command
};
ASSERT_SIZE(DriverCommand, 0x10);

// the voices update their parameters (name is ours)
struct DriverCommandVoiceSync : DriverCommand
{
    u32 syncFlag; // 0x10 the parameters to update (Voice)
};
ASSERT_SIZE(DriverCommandVoiceSync, 0x14);
} // namespace internal
} // namespace snd
} // namespace nw
