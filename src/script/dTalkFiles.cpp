#include "script/dTalkSystem.h"

namespace script {
// 0x005E45E8 (name is ours)
void TalkFiles::Select(const sead::SafeStringBase<char>& name, sead::Heap* heap)
{
    for (s32 i = 0; i < m_Num; i++) {
        TalkFile* file = &m_Files[i];
        if (file->m_Name.isEqual(name)) {
            if (file != NULL) {
                m_pCurrent = file;
                return;
            }
            break;
        }
    }
    m_pCurrent = &m_Main;
    m_Main.Set(name, heap);
}

// 0x005E474C (name is ours)
bool TalkFiles::Load()
{
    bool isDone = true;
    for (s32 i = 0; i < m_Num; i++) {
        TalkFile* file = &m_Files[i];
        if (!file->m_Loader.Load(file->m_Path, file->m_pHeap, 0)) {
            isDone = false;
        }
    }
    return isDone;
}

// 0x005E47A8 (name is ours)
void TalkFiles::Add(const sead::SafeStringBase<char>& name, sead::Heap* heap)
{
    if (m_Num >= FILE_NUM) {
        return;
    }
    TalkFile* file = &m_Files[m_Num];
    m_Num++;
    file->Set(name, heap);
}
} // namespace script
