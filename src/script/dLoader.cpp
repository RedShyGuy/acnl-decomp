#include "script/dLoader.h"
#include "sead/seadSafeStringBase.h"

namespace script {
// 0x005F1C08 (name is ours)
script::Loader::Loader(bool isSplit) : m_IsSplit(isSplit), m_IsLoaded(0), m_Data(), m_Split() {}

// 0x005F1B80 (name is ours)
bool script::Loader::Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, u32 alignment)
{
    if (m_IsSplit) {
        bool isFinished = m_Split.Read(path, heap, alignment);
        if (isFinished) {
            m_IsLoaded = m_Split.IsSucceeded();
        }
        return isFinished;
    }
    m_IsLoaded = m_Data.m_IsKept ? true : m_Data.Load(path, heap, alignment);
    return true;
}

// 0x005F1C04 slot 0x00 (name after the RTTI)
// 0x005F1BF4 (deleting dtor)
script::Loader::LoadSplit::~LoadSplit()
{
    // (only the bases are destroyed)
}

// 0x005F1BD8 slot 0x0C (name is ours)
void script::Loader::LoadSplit::OnReset()
{
    if (m_IsBorrowed) {
        m_pData = NULL;
        m_IsBorrowed = 0;
    }
}
} // namespace script
