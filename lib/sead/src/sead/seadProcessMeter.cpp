#include "sead/seadProcessMeter.h"
#include "sead/seadNew.h"
#include "sead/seadProcessMeter_SingletonDisposer_.h"

#include <new>

namespace sead {
// 0x0097F86C
ProcessMeter* ProcessMeter::s_pInstance = nullptr;
// 0x0097F870
ProcessMeter::SingletonDisposer_* ProcessMeter::s_pSingletonDisposer = nullptr;

// 0x0054103C (name is ours)
sead::ProcessMeter* sead::ProcessMeter::createInstance(Heap* heap)
{
    if (s_pInstance == nullptr) {
        u8* buffer = new (heap, 4) u8[sizeof(ProcessMeter)];
        s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
        s_pInstance = new (buffer) ProcessMeter;
    }
    return s_pInstance;
}
} // namespace sead
