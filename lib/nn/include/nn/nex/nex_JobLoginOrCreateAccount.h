#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class JobLoginOrCreateAccount
{
public:
    void CreateAccount(); // 0x003B0D74 | fefates:bytes [tier B]
    void ProcessRVLoginResult(); // 0x003B0E28 | fefates:bytes [tier B]
    void ProcessCreateAccountResult(); // 0x003B0F0C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
