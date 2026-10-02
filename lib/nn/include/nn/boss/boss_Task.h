#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss4TaskE @ 0x008D038C
// vtable 0x009021BC (vptr 0x009021C4), offset_to_top 0, 2 entries
class Task
{
public:
    Task(); // ctor candidate(s) 0x0046C824 (unverified)
    virtual void vf_0x00(); // 0x0046C850 slot 0x00 | virtual slot, introduced by nn::boss::Task
    virtual void vf_0x04(); // 0x0046C848 slot 0x04 | virtual slot, introduced by nn::boss::Task
    void Initialize(const char*); // 0x0046C1AC | nintendogs:bytes [tier B]
    void WaitFinish(const nn::fnd::TimeSpan&); // 0x0046C20C | fefates:bytes-fuzzy [tier B]
    void UpdateCount(unsigned); // 0x0046C2FC | nintendogs:bytes [tier A]
    void GetStateDetail(nn::boss::TaskStatus*, bool, unsigned char*, unsigned char); // 0x0046C394 | fefates:bytes [tier B]
    void StartImmediate(); // 0x0046C484 | nintendogs:bytes [tier A]
    void GetServiceStatus(); // 0x0046C524 | fefates:bytes [tier B]
    void Start(); // 0x0046C5C0 | nintendogs:bytes [tier A]
    void GetState(bool, unsigned int*, unsigned char*); // 0x0046C64C | fefates:bytes-fuzzy [tier B]
    void GetResult(unsigned*, unsigned char*); // 0x0046C738 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn
