#include "script/dProject_SingletonDisposer_.h"

// 0x005F7DC0 slot 0x00 (name after sead)
// 0x005F7D5C (deleting dtor)
script::Project::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != Project::s_pSingletonDisposer) {
        return;
    }
    Project::s_pSingletonDisposer = NULL;
    Project::s_pInstance->~Project();
    Project::s_pInstance = NULL;
}
