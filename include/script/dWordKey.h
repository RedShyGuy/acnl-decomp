#pragma once

#include "decomp.h"
#include "script/dWordFix.h"

namespace script {
// a fixed word with a key (member names are ours). Instances: <9u, int>, <29u, int>, <45u, int>, <57u, int>, <85u, int>
template <u32 N, typename T>
class WordKey : public WordFix<N>
{
public:
    WordKey() : m_Key(0) {}
    virtual ~WordKey() {}
    virtual void Clear()
    {
        WordPtr::Clear();
        this->m_Info.Reset();
        m_Key = 0;
        m_Flag = 0;
    }

    T m_Key;
    u8 m_Flag;
};
} // namespace script
