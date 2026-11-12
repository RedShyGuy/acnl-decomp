#pragma once

#include "decomp.h"
#include "state/dBase.h"

namespace state {
// A sequence of steps: the current step (a member function of T1) is called each frame; the steps of a sequence are a
// table of named entries, Next goes to the entry after the current one. Class name from the RTTI; member names are ours
// (after state::Mode).
// Instantiations found in the binary:
//   state::Step<BsSvMgr, BsSvMgr::SaveStep>  typeinfo 0x008D3668  vtable 0x00909324
template <typename T0, typename T1>
class Step : public ::state::Base<T1>
{
public:
    typedef typename ::state::Base<T1>::Func Func;

    // a named step (the names are debug strings)
    struct Info
    {
        const char* name;
        Func func;
    };

    Step() : m_Func(NULL), m_Counter(0), m_pOwner(NULL), m_pInfos(NULL), m_Count(0) {}
    virtual ~Step() {}
    // the name of a step (the last match; none when disabled)
    virtual const char* GetName(Func func) const
    {
        const char* name = NULL;
        if (this->m_Enabled) {
            for (u32 i = 0; i < m_Count; i++) {
                if (m_pInfos[i].func == func) {
                    name = m_pInfos[i].name;
                }
            }
        }
        return name;
    }

    // (inline) a sequence starts with its first step (enabled: the names are looked up)
    void Set(T0* owner, const char* name, const Info* infos, u32 count, bool enabled)
    {
        m_pOwner = owner;
        m_Count = count;
        m_pInfos = infos;
        this->m_pName = name;
        this->m_Enabled = enabled;
        m_Func = infos[0].func;
    }
    // the step after the current one (nothing at the end)
    DECOMP_NOINLINE void Next()
    {
        for (u32 i = 0; i < m_Count; i++) {
            if (m_pInfos[i].func == m_Func && i + 1 < m_Count) {
                m_Func = m_pInfos[i + 1].func;
                return;
            }
        }
    }
    // the current step is called; the counter counts the frames in the same step
    DECOMP_NOINLINE void Exec()
    {
        Func func = m_Func;
        if (func != NULL) {
            (static_cast<T1*>(this)->*func)();
        }
        if (m_Func == func) {
            m_Counter++;
        } else {
            m_Counter = 0;
        }
    }

    Func m_Func;            // 0x0C
    s32 m_Counter;          // 0x14
    T0* m_pOwner;           // 0x18
    const Info* m_pInfos;   // 0x1C
    u32 m_Count;            // 0x20
};
} // namespace state
