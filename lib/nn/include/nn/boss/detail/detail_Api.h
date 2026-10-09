#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/boss/boss_Types.h"

namespace nn {
namespace boss {
class TaskOption;
class TaskPolicy;
namespace detail {
class IpcManager;
class Privileged;
class User;

DECOMP_NOINLINE bool CheckTaskIdOk(const char* pTaskId); // 0x0046DA44 | nintendogs:bytes [tier A]
DECOMP_NOINLINE bool IsFgOnlyTaskId(const char* pTaskId); // 0x0046DA5C | nintendogs:bytes [tier A]
nn::Result FinalizeUserIpc(); // 0x0046DA94 | nintendogs:callgraph [tier A]
// (the spelling is the one of the symbols)
nn::Result InitilizeUserIpc(); // 0x0046DAA0 | nintendogs:callgraph [tier A]
// the session objects; a failure (result 43) when the session is not open
nn::Result GetUserIpcInstance(nn::boss::detail::User*& pUser); // 0x0046DAAC | nintendogs:bytes [tier A]
nn::Result GetPrivilegedIpcInstance(nn::boss::detail::Privileged*& pPrivileged); // 0x0046E108 | nintendogs:bytes [tier A]
// send / read the properties of a task; the first failure is kept in s_PropertyResult
DECOMP_NOINLINE nn::Result SendUserTaskAction(nn::boss::TaskActionConfig* pConfig); // 0x0046DAE0 | nintendogs:bytes-fuzzy [tier A]
DECOMP_NOINLINE nn::Result SendUserTaskOption(nn::boss::TaskOptionConfig* pConfig); // 0x0046DCB4 | nintendogs:bytes [tier A]
DECOMP_NOINLINE nn::Result SendUserTaskPolicy(nn::boss::TaskPolicyConfig* pConfig); // 0x0046DD24 | nintendogs:bytes [tier A]
DECOMP_NOINLINE nn::Result ReceiveUserTaskStatus(nn::boss::TaskStatusInfo* pInfo); // 0x0046DDA0 | fefates:bytes-fuzzy [tier B]
DECOMP_NOINLINE nn::Result VerifyTaskActionConfig(nn::boss::TaskActionConfig* pConfig); // 0x0046DEF0 | nintendogs:callgraph [tier A]
DECOMP_NOINLINE nn::Result VerifyTaskOptionConfig(nn::boss::TaskOption* pOption); // 0x0046DFE4 | nintendogs:bytes [tier A]
DECOMP_NOINLINE nn::Result VerifyTaskPolicyConfig(nn::boss::TaskPolicy* pPolicy); // 0x0046E00C | nintendogs:bytes [tier A]
DECOMP_NOINLINE nn::Result SendPropertyUserInternal(nn::boss::PropertyType type, nn::Handle handle); // 0x0046E13C | fefates:bytes [tier B]
DECOMP_NOINLINE nn::Result SendPropertyUserInternal(nn::boss::PropertyType type, unsigned char* pValue, unsigned int size); // 0x0046E1E8 | fefates:bytes [tier B]
DECOMP_NOINLINE nn::Result ReceivePropertyUserInternal(nn::boss::PropertyType type, unsigned char* pValue, unsigned int size); // 0x0046E860 (name is ours)
// the result of a code of the service (module 62); unknown codes give 77
nn::Result ChangeBossRetCodeToResult(nn::boss::ResultCode code); // 0x0046E2A0 | nintendogs:bytes [tier A]
DECOMP_NOINLINE size_t strnlen(const char* pString, unsigned maxLength); // 0x0046F134 | nintendogs:bytes [tier A]
DECOMP_NOINLINE size_t wcsnlen(const wchar_t* pString, unsigned int maxLength); // 0x0046F154 | fefates:bytes [tier B]

// the sessions (0x00AF6190)
extern nn::boss::detail::IpcManager s_IpcManager;
} // namespace detail
} // namespace boss
} // namespace nn
