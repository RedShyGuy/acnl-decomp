#include "script/dFlowSeq.h"
#include "script/dFlowSystem.h"

namespace script {
// (in its own file: GCC would fold the tables into the calls otherwise)

// 0x008920A8
const FlowMessage::StateFunc FlowMessage::s_StateFuncs[STATE_NUM] = {
    NULL, &FlowMessage::UpdateStart, &FlowMessage::UpdateResume, &FlowMessage::UpdateWait, NULL, NULL,
};

// 0x008920D8
const FlowSeq::StateFunc FlowSeq::s_EnterFuncs[STATE_NUM] = {
    &FlowSeq::EnterIdle,
    &FlowSeq::EnterLoad,
    &FlowSeq::EnterRun,
    &FlowSeq::EnterEnd,
};

// 0x008920F8
const FlowSeq::StateFunc FlowSeq::s_UpdateFuncs[STATE_NUM] = {
    &FlowSeq::UpdateIdle,
    &FlowSeq::UpdateLoad,
    &FlowSeq::UpdateRun,
    &FlowSeq::UpdateEnd,
};

// 0x00892118
const FlowSystem::StateFunc FlowSystem::s_StateFuncs[STATE_NUM] = {
    &FlowSystem::UpdateRun,
    &FlowSystem::UpdateEndTalk,
};
} // namespace script
