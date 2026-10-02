#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
namespace detail {
class Privileged
{
public:
    void ReadNsData(unsigned, long long, unsigned char*, unsigned, unsigned*, unsigned*); // 0x0046D0D4 | nintendogs:callgraph [tier A]
    void GetErrorCode(unsigned*, nn::boss::TaskResultCode); // 0x0046D168 | nintendogs:callgraph [tier A]
    void RegisterTask(const unsigned char*, unsigned, unsigned char, unsigned char); // 0x0046D214 | nintendogs:callgraph [tier A]
    void SendProperty(nn::boss::PropertyType, const unsigned char*, unsigned int); // 0x0046D268 | fefates:callgraph [tier A]
    void GetTaskResult(const unsigned char*, unsigned, nn::boss::TaskResultCode*, unsigned*, unsigned char*); // 0x0046D2E8 | nintendogs:callgraph [tier A]
    void SetOptoutFlag(bool); // 0x0046D3A8 | nintendogs:callgraph [tier A]
    void GetStorageInfo(unsigned*); // 0x0046D3E0 | nintendogs:callgraph [tier A]
    void UnregisterTask(const unsigned char*, unsigned, unsigned char); // 0x0046D414 | nintendogs:callgraph [tier A]
    void GetNsDataIdList(unsigned, unsigned*, unsigned, unsigned short*, unsigned short, unsigned, unsigned short*); // 0x0046D460 | nintendogs:callgraph [tier A]
    void RegisterStorage(unsigned long long, unsigned, nn::fs::MediaType); // 0x0046D524 | nintendogs:callgraph [tier A]
    void UpdateTaskCount(const unsigned char*, unsigned, unsigned); // 0x0046D564 | nintendogs:callgraph [tier A]
    void GetNsDataNewFlag(unsigned, bool*); // 0x0046D5A4 | nintendogs:callgraph [tier A]
    void SetNsDataNewFlag(unsigned, bool); // 0x0046D5E0 | nintendogs:callgraph [tier A]
    void SendPropertyHandle(nn::boss::PropertyType, nn::Handle); // 0x0046D644 | fefates:callgraph [tier A]
    void StartTaskImmediate(const unsigned char*, unsigned); // 0x0046D688 | nintendogs:callgraph [tier A]
    void GetNsDataHeaderInfo(unsigned, nn::boss::HeaderInfoType, unsigned char*, unsigned); // 0x0046D6C8 | nintendogs:callgraph [tier A]
    void GetNsDataLastUpdated(unsigned, long long*); // 0x0046D750 | nintendogs:callgraph [tier A]
    void GetTaskServiceStatus(const unsigned char*, unsigned int, nn::boss::TaskServiceStatus*); // 0x0046D790 | fefates:callgraph [tier A]
    void ReadNsDataPrivileged(unsigned long long, unsigned, long long, unsigned char*, unsigned, unsigned*, unsigned*); // 0x0046D7DC | nintendogs:bytes [tier A]
    void RegisterImmediateTask(const unsigned char*, unsigned int, unsigned char, unsigned char); // 0x0046D850 | fefates:callgraph [tier A]
    void DeleteNsDataPrivileged(unsigned long long, unsigned int); // 0x0046D8A4 | fefates:bytes [tier A]
    void GetNsDataNewFlagPrivileged(unsigned long long, unsigned, bool*); // 0x0046D8DC | nintendogs:bytes [tier A]
    void SetNsDataNewFlagPrivileged(unsigned long long, unsigned, bool); // 0x0046D920 | nintendogs:bytes [tier A]
    void GetNsDataHeaderInfoPrivileged(unsigned long long, unsigned, nn::boss::HeaderInfoType, unsigned char*, unsigned); // 0x0046D960 | nintendogs:bytes [tier A]
    void GetNsDataLastUpdatedPrivileged(unsigned long long, unsigned, long long*); // 0x0046D9BC | nintendogs:bytes [tier A]
    void StartTask(const unsigned char*, unsigned); // 0x0046DA04 | nintendogs:callgraph [tier A]
};
} // namespace detail
} // namespace boss
} // namespace nn
