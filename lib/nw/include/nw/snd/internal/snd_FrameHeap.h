#pragma once

#include "decomp.h"
#include "nw/ut/ut_FrameHeap.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
// A frame heap in sections: SaveState starts a new section, LoadState frees the sections above a
// level. A block can have a callback that is called when it is freed (the class and function names
// are from the reference symbols, the rest is ours).
class FrameHeap
{
public:
    // called when a block is freed: its memory, size and the argument
    typedef void (*FreeCallback)(void* pBuffer, u32 size, void* pArg);

    // a block of the heap (the memory follows, aligned to 32)
    struct Block
    {
        ut::LinkListNode m_Link;  // 0x00
        void* m_pBuffer;          // 0x08
        u32 m_Size;               // 0x0C
        FreeCallback m_Callback;  // 0x10
        void* m_pCallbackArg;     // 0x14
    };
    static const u32 BLOCK_HEADER_SIZE = 32;

    // the blocks between two states
    struct Section
    {
        // (inline)
        Section() : m_IsUseCallback(true) {}

        ut::LinkListNode m_Link;               // 0x00
        ut::LinkList<Block, 0> m_BlockList;    // 0x08
        bool m_IsUseCallback;                  // 0x14 (false: one callback for several sections was called)
    };

    FrameHeap(); // 0x001385C4 | nintendogs:bytes [tier A]
    ~FrameHeap(); // 0x004D4BAC | nintendogs:bytes [tier A]

    // a heap in the memory (the old one destroyed)
    bool Create(void* pStartAddress, u32 size); // 0x00138540 | nintendogs:bytes [tier A]
    void Destroy(); // 0x00141B20 | nintendogs:bytes [tier A]
    // everything freed
    void Clear(); // 0x00141AFC | nintendogs:bytes-fuzzy [tier A]
    // a block of size bytes in the current section (NULL: no memory)
    void* Alloc(u32 size, FreeCallback callback, void* pCallbackArg); // 0x004D4B3C | nintendogs:bytes [tier A]
    // a new section; its level (-1: no memory)
    int SaveState(); // 0x001327AC | nintendogs:bytes [tier A]
    // the sections above the level freed (0: all)
    void LoadState(int level); // 0x0013F3FC | nintendogs:bytes-fuzzy [tier B]

    // (inline)
    bool IsValid() const { return m_pFrameHeap != nullptr; }

    bool NewSection(); // 0x0013F4D8 | nintendogs:bytes [tier A]
    void ClearSection(); // 0x00141964 | nintendogs:bytes [tier A]
    // if all blocks of the sections above the level have the same callback, it is called once for
    // all of them; true if so
    bool ProcessCallback(int level); // 0x001419FC | nintendogs:callgraph [tier A]

    ut::FrameHeap* m_pFrameHeap;            // 0x0
    ut::LinkList<Section, 0> m_SectionList; // 0x4
};
ASSERT_SIZE(FrameHeap, 0x10);
} // namespace internal
} // namespace snd
} // namespace nw
