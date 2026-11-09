#pragma once

#include "decomp.h"
#include "Other/ddLoadSplit.h"
#include "ssys/ma/dFileData.h"

namespace sead {
template <typename T>
class SafeStringBase;
class Heap;
} // namespace sead

namespace script {
// Loads a file of the scripts (a message binary) at once or in pieces (no RTTI of its own; the class name is from the
// RTTI of its nested LoadSplit, the member names are ours).
class Loader
{
public:
    // a load in pieces that may keep the buffer of someone else
    // RTTI N6script6Loader9LoadSplitE @ 0x008D3A98
    // vtable 0x0090A8C4 (vptr 0x0090A8CC), offset_to_top 0, 4 entries
    class LoadSplit : public ::dLoadSplit
    {
    public:
        // (inline; the base is value-initialized: cleared first)
        LoadSplit() : dLoadSplit(), m_IsBorrowed(0) {}
        virtual ~LoadSplit(); // 0x005F1C04 slot 0x00
        // 0x005F1BF4 slot 0x04 (deleting dtor)
        // the buffer is forgotten when it is not ours
        virtual void OnReset(); // 0x005F1BD8 slot 0x0C

        u8 m_IsBorrowed;  // 0xEB the buffer is not ours
    };

    // the file data with a flag
    struct Data : public ssys::ma::FileData
    {
        // (inline)
        Data() : m_IsKept(false) {}

        bool m_IsKept;  // 0x0D the data stays: Load reports success at once
    };

    explicit Loader(bool isSplit); // 0x005F1C08

    // starts or goes on with loading path; true when it has finished
    bool Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment); // 0x005F1B80
    // (inline) frees the file
    void Unload()
    {
        if (m_IsSplit) {
            m_Split.Reset();
        } else {
            m_Data.m_IsKept = false;
            m_Data.Unload();
        }
        m_IsLoaded = 0;
    }
    // (inline) the loaded data
    const void* GetData() const { return m_IsSplit ? m_Split.m_pData : m_Data.m_pData; }

    u8 m_IsSplit;        // 0x00 loaded in pieces (m_Split, else m_Data)
    u8 m_IsLoaded;       // 0x01
    Data m_Data;         // 0x04
    LoadSplit m_Split;   // 0x14
};
ASSERT_SIZE(Loader::LoadSplit, 0xEC);
ASSERT_SIZE(Loader, 0x100);
} // namespace script
