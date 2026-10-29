#include "sead/seadProcessMeter_SingletonDisposer_.h"

// 0x005410DC slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00541098 (deleting dtor)
sead::ProcessMeter::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != ProcessMeter::s_pSingletonDisposer) {
        return;
    }
    ProcessMeter::s_pSingletonDisposer = nullptr;
    ProcessMeter::s_pInstance->~ProcessMeter();
    ProcessMeter::s_pInstance = nullptr;
}
