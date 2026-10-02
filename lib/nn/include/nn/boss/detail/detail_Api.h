#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
namespace detail {
void CheckTaskIdOk(const char*); // 0x0046DA44 | nintendogs:bytes [tier A]
void IsFgOnlyTaskId(const char*); // 0x0046DA5C | nintendogs:bytes [tier A]
void FinalizeUserIpc(); // 0x0046DA94 | nintendogs:callgraph [tier A]
void InitilizeUserIpc(); // 0x0046DAA0 | nintendogs:callgraph [tier A]
void GetUserIpcInstance(nn::boss::detail::User*&); // 0x0046DAAC | nintendogs:bytes [tier A]
void SendUserTaskAction(nn::boss::TaskActionConfig*); // 0x0046DAE0 | nintendogs:bytes-fuzzy [tier A]
void SendUserTaskOption(nn::boss::TaskOptionConfig*); // 0x0046DCB4 | nintendogs:bytes [tier A]
void SendUserTaskPolicy(nn::boss::TaskPolicyConfig*); // 0x0046DD24 | nintendogs:bytes [tier A]
void ReceiveUserTaskStatus(nn::boss::TaskStatusInfo*); // 0x0046DDA0 | fefates:bytes-fuzzy [tier B]
void VerifyTaskActionConfig(nn::boss::TaskActionConfig*); // 0x0046DEF0 | nintendogs:callgraph [tier A]
void VerifyTaskOptionConfig(nn::boss::TaskOption*); // 0x0046DFE4 | nintendogs:bytes [tier A]
void VerifyTaskPolicyConfig(nn::boss::TaskPolicy*); // 0x0046E00C | nintendogs:bytes [tier A]
void GetPrivilegedIpcInstance(nn::boss::detail::Privileged*&); // 0x0046E108 | nintendogs:bytes [tier A]
void SendPropertyUserInternal(nn::boss::PropertyType, nn::Handle); // 0x0046E13C | fefates:bytes [tier B]
void SendPropertyUserInternal(nn::boss::PropertyType, unsigned char*, unsigned int); // 0x0046E1E8 | fefates:bytes [tier B]
void ChangeBossRetCodeToResult(nn::boss::ResultCode); // 0x0046E2A0 | nintendogs:callgraph [tier A]
void strnlen(const char*, unsigned); // 0x0046F134 | nintendogs:bytes [tier A]
void wcsnlen(const wchar_t*, unsigned int); // 0x0046F154 | fefates:bytes [tier B]
} // namespace detail
} // namespace boss
} // namespace nn
