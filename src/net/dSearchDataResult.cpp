#include "net/dSearchDataResult.h"

#include "script/dWordCPtrSv.h"
#include "script/dWordPtrSv.h"

namespace net {
// 0x0050D0C4 (name is ours)
net::SearchDataParam::SearchDataParam()
{
    Clear();
}

// 0x0050D06C (name is ours)
void CopyName(char16* buffer, const char16* word, u32 length)
{
    script::WordPtrSv(buffer, SearchDataResult::NAME_LENGTH + 1).Copy(script::WordCPtrSv(word, length + 1));
}

// 0x0050D110 (name is ours)
void net::SearchDataResult::SetName(const char16* name, u32 length)
{
    script::WordPtrSv(m_Name, NAME_LENGTH + 1).Copy(script::WordCPtrSv(name, length + 1));
}

// 0x0050D168 (name is ours)
net::SearchDataResult::SearchDataResult()
{
    Clear();
}

// 0x0050D1B4 (name is ours)
void net::SearchDataResultList::Reset()
{
    for (u32 i = 0; i < m_Num; i++) {
        SearchDataResult* result = m_pResults[i];
        result->Clear();
        result->OnClear();
    }
    m_UsedNum = 0;
}

// 0x0050D224 (name is ours)
void net::SearchDataResultList::Register(SearchDataResult* result)
{
    if (m_Num < MAX_NUM) {
        m_pResults[m_Num] = result;
        m_Num++;
    }
}

// 0x0050D244 (name is ours)
net::SearchDataResult* net::SearchDataResultList::Add(u32 ownerId, u64 dataId, u16 dataType)
{
    if (m_UsedNum >= m_Num) {
        return NULL;
    }
    SearchDataResult* result = m_pResults[m_UsedNum];
    if (!result->OnSet(ownerId, dataId, dataType)) {
        return NULL;
    }
    result->Set(ownerId, dataId, dataType);
    m_UsedNum++;
    return result;
}

// 0x0050D2B4 (name is ours)
net::SearchDataResultList::SearchDataResultList()
{
    m_Num = 0;
    m_UsedNum = 0;
    for (u32 i = 0; i < MAX_NUM; i++) {
        m_pResults[i] = NULL;
    }
}
// 0x0050D108 (name is ours)
void* net::SearchDataResult::GetMetaBinaryBuffer()
{
    return NULL;
}

// 0x0050D1B0 (name is ours)
// 0x0050D1A8 (deleting dtor)
net::SearchDataResult::~SearchDataResult()
{
    // empty (in the original too)
}

// 0x00747AFC (name is ours)
u32 net::SearchDataResult::GetMetaBinarySize()
{
    return 0;
}
} // namespace net
