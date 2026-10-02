#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class JobDataStoreHpp
{
public:
    void StepWaitingForHttp(); // 0x00378700 | fefates:bytes [tier B]
    void ProcessFinalResponse(const nn::nex::qVector<unsigned char>&); // 0x00378918 | fefates:bytes [tier B]
    void PrepareRequestHeaders(); // 0x00378A88 | fefates:bytes-fuzzy [tier B]
    void StepRetrieveGameAuthToken(); // 0x003792DC | fefates:bytes [tier B]
    void StepWaitingRetrieveGameAuthToken(); // 0x00379400 | fefates:bytes [tier B]
    void StepHttp(); // 0x003794BC | fefates:bytes [tier B]
    void CompleteJob(const nn::nex::qResult&); // 0x003CC5B0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
