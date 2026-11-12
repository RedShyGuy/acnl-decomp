#pragma once

#include "decomp.h"

class SvStepInner;
namespace sead {
class Heap;
} // namespace sead

// The work of the save thread for one request (SvStepInner): mount, load, take the loaded data over, save, commit. The
// class name is from the RTTI; the other names are ours.
// RTTI 11SaveProcess @ 0x008CB328
// vtable 0x008ECCC4 (vptr 0x008ECCCC), offset_to_top 0, 2 entries
class SaveProcess
{
public:
    // the file kinds of Load (flags & SvStepInner::FLAG_LOAD_FILE_MASK)
    enum LoadFile
    {
        LOAD_FILE_GARDEN_PLUS = 2,
        LOAD_FILE_EXHIBITION = 4,
        LOAD_FILE_MY_DESIGN = 6,
        LOAD_FILE_MAIL = 8,
        LOAD_FILE_BEST_FRIEND_LIST = 10,
        LOAD_FILE_MOVE_PLAYER_PLS = 12,
        LOAD_FILE_INCLUDE_SAVE_14 = 14, // (ModuleIncSave)
        LOAD_FILE_IDENTITY = 16,
        LOAD_FILE_INCLUDE_SAVE_18 = 18, // (ModuleIncSave)
        LOAD_FILE_AMIIBO = 20,
    };

    // (inline in ProcThread::calc_ and SvRunSaveProcess)
    explicit SaveProcess(SvStepInner* request) : m_pRequest(request) {}
    // the request forgets the process
    virtual ~SaveProcess(); // 0x001D49D4
    // 0x001D49B4 (deleting dtor)

    // all steps of the request; at the end the callbacks of the result
    void Run(); // 0x001D3CDC
    // the file of flags & FLAG_LOAD_FILE_MASK is read (and checked with FLAG_CHECK); false: failed (the result is set)
    DECOMP_NOINLINE bool Load(u64 flags); // 0x001D42D0
    // the access ends (endAccess), the callback of the result, the request is done
    void Finish(bool endAccess); // 0x001D3B44

    SvStepInner* m_pRequest;    // 0x04
};
ASSERT_SIZE(SaveProcess, 0x8);

// the request is worked on here, without the save thread (name is ours)
bool SvRunSaveProcess(SvStepInner* request); // 0x005CDA94

// (game code around taking the data over, not decompiled yet; names after the addresses) an object with a guarded
// local static, its wait for the heap, a Thumb function before and one after
void* Func3044DC(); // 0x003044DC
bool Func1C2530(void* object, sead::Heap* heap); // 0x001C2530
void Func110CF0(); // 0x00110CF0
void Func139580(void* object); // 0x00139580
