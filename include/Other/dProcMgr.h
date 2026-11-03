#pragma once

#include "decomp.h"

namespace oml {
namespace framework {
class Process;
} // namespace framework
} // namespace oml

// the process classes of the game: the index into the tables of the ProcessManager (name after libgarden; the
// constants of the classes are variables of another file, see stage)
enum ProcName : u16
{
    PROC_NAME_BEGIN = 0,
    PROC_NAME_USED_COUNT = 0x27F,
    PROC_NAME_COUNT = 0x29D,
};

// the process classes (variables of another file; names after the process table of the binary)
extern const ProcName PROC_SC_STAGE;
extern const ProcName PROC_SC_STAGE_MINI_GAME_0;
extern const ProcName PROC_SC_STAGE_MINI_GAME_1;
extern const ProcName PROC_BS_PLAYER_MGR;
extern const ProcName PROC_BS_INDOOR_DOOR_SE;

// the kind of a process (names after libgarden)
enum ProcType : u8
{
    PROC_TYPE_BASE = 0,
    PROC_TYPE_SCENE = 1,
    PROC_TYPE_ACTOR = 2,
};

class ProcMgr
{
public:
    // NULL for a process class out of range or a stopped parent
    static oml::framework::Process* Create(ProcName name, oml::framework::Process* parent, u32 arg, ProcType type); // 0x0052A32C (name after libgarden)
    void Create(ProcName, unsigned long, ProcType); // 0x0052A3B4 | libgarden [tier A]
};
