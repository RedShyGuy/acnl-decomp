#pragma once

#include "decomp.h"

namespace oml {
namespace framework {
class ProcessManager
{
public:
    void CreateProcess(unsigned short, unsigned long, unsigned char); // 0x005202C0 | libgarden [tier A]
    void Get(); // 0x005204DC | libgarden [tier A]
    void GetInfo(unsigned short); // 0x00521394 | libgarden [tier A]
    void CallCreate(unsigned short); // 0x005213C8 | libgarden [tier A]
    void CallDestroy(unsigned short, oml::framework::Process*); // 0x005213DC | libgarden [tier A]
};
} // namespace framework
} // namespace oml
