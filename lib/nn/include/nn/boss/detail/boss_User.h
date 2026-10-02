#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
namespace detail {
class User
{
public:
    void GetTaskStatus(const unsigned char*, unsigned int, bool, unsigned char*, unsigned char); // 0x0046D348 | fefates:bytes [tier A]
    void ReadNsData(unsigned, long long, unsigned char*, unsigned, unsigned*, unsigned*); // 0x0046E960 | nintendogs:callgraph [tier A]
    void GetErrorCode(unsigned*, nn::boss::TaskResultCode); // 0x0046E9F4 | nintendogs:callgraph [tier A]
    void RegisterTask(const unsigned char*, unsigned, unsigned char, unsigned char); // 0x0046EAA0 | nintendogs:callgraph [tier A]
    void SendProperty(nn::boss::PropertyType, const unsigned char*, unsigned int); // 0x0046EAF4 | fefates:callgraph [tier A]
    void GetTaskResult(const unsigned char*, unsigned, nn::boss::TaskResultCode*, unsigned*, unsigned char*); // 0x0046EB74 | nintendogs:callgraph [tier A]
    void SetOptoutFlag(bool); // 0x0046EC34 | nintendogs:callgraph [tier A]
    void GetStorageInfo(unsigned*); // 0x0046EC6C | nintendogs:callgraph [tier A]
    void UnregisterTask(const unsigned char*, unsigned, unsigned char); // 0x0046ECA0 | nintendogs:callgraph [tier A]
    void GetNsDataIdList(unsigned, unsigned*, unsigned, unsigned short*, unsigned short, unsigned, unsigned short*); // 0x0046ECEC | nintendogs:callgraph [tier A]
    void RegisterStorage(unsigned long long, unsigned, nn::fs::MediaType); // 0x0046EDB0 | nintendogs:callgraph [tier A]
    void UpdateTaskCount(const unsigned char*, unsigned, unsigned); // 0x0046EDF0 | nintendogs:callgraph [tier A]
    void GetNsDataNewFlag(unsigned, bool*); // 0x0046EE30 | nintendogs:callgraph [tier A]
    void SetNsDataNewFlag(unsigned, bool); // 0x0046EE6C | nintendogs:callgraph [tier A]
    void InitializeSession(unsigned long long); // 0x0046EEA8 | nintendogs:bytes [tier A]
    void SendPropertyHandle(nn::boss::PropertyType, nn::Handle); // 0x0046EF08 | fefates:callgraph [tier A]
    void StartTaskImmediate(const unsigned char*, unsigned); // 0x0046EF4C | nintendogs:callgraph [tier A]
    void GetNsDataHeaderInfo(unsigned, nn::boss::HeaderInfoType, unsigned char*, unsigned); // 0x0046EF8C | nintendogs:callgraph [tier A]
    void GetNsDataLastUpdated(unsigned, long long*); // 0x0046F014 | nintendogs:callgraph [tier A]
    void GetTaskServiceStatus(const unsigned char*, unsigned int, nn::boss::TaskServiceStatus*); // 0x0046F054 | fefates:callgraph [tier A]
    void RegisterImmediateTask(const unsigned char*, unsigned int, unsigned char, unsigned char); // 0x0046F0A0 | fefates:callgraph [tier A]
    void StartTask(const unsigned char*, unsigned); // 0x0046F0F4 | nintendogs:callgraph [tier A]
};
} // namespace detail
} // namespace boss
} // namespace nn
