#pragma once

// The C++ side of LibMessageStudio: the memory functions of the C library and handles of its three binaries.
// All names except the namespace are ours. The one-line methods are inline; the original has out-of-line copies
// of them (they fall into the C function, armlink replaced the tail call by a nop).

#include "decomp.h"
#include "libms/lms_Api.h"

namespace libms {

void Initialize(LMS_AllocFunc alloc, LMS_FreeFunc free);
void Finalize();

// a text binary ("MsgStdBn")
class Message
{
public:
    Message();

    void Initialize(const void* data);
    void Finalize();

    DECOMP_EMIT_COPY const void* GetAttribute(s32 index) { return LMS_GetAttribute(m_pBinary, index); } // 0x00159A7C (name is ours)
    DECOMP_EMIT_COPY bool GetLabel(s32 index, char* label) { return LMS_GetLabelByIndex(m_pBinary, index, label); } // 0x00159CE0 (name is ours)
    DECOMP_EMIT_COPY const void* GetText(s32 index) { return LMS_GetText(m_pBinary, index); } // 0x00159D94 (name is ours)
    DECOMP_EMIT_COPY const void* GetTextByLabel(const char* label) { return LMS_GetTextByLabel(m_pBinary, label); } // 0x00159DD8 (name is ours)
    DECOMP_EMIT_COPY s32 GetTextIndex(const char* label) { return LMS_GetTextIndexByLabel(m_pBinary, label); } // 0x00159E34 (name is ours)
    DECOMP_EMIT_COPY s32 GetTextNum() { return LMS_GetTextNum(m_pBinary); } // 0x00159F14 (name is ours)

private:
    LMS_MessageBinary* m_pBinary;  // +0x00
    const void* m_pData;           // +0x04
};
ASSERT_SIZE(Message, 0x8);

// a flow chart binary ("MsgFlwBn")
class Flow
{
public:
    Flow();

    void Initialize(const void* data);
    void Finalize();

    DECOMP_EMIT_COPY const u16* GetBranchCases(s32 index) { return LMS_GetBranchCases(m_pBinary, index); } // 0x00159AA0 (name is ours)
    DECOMP_EMIT_COPY s32 GetEntryIndex(const char* label) { return LMS_GetFlowEntryIndex(m_pBinary, label); } // 0x00159BC4 (name is ours)
    DECOMP_EMIT_COPY const void* GetData(s32 offset) { return LMS_GetFlowData(m_pBinary, offset); } // 0x00159CA8 (name is ours)
    DECOMP_EMIT_COPY const LMS_FlowNode* GetNode(s32 index) { return LMS_GetFlowNode(m_pBinary, index); } // 0x00159D64 (name is ours)

private:
    LMS_FlowBinary* m_pBinary;  // +0x00
    const void* m_pData;        // +0x04
};
ASSERT_SIZE(Flow, 0x8);

// a project binary ("MsgPrjBn")
class Project
{
public:
    Project();

    void Initialize(const void* data);
    void Finalize();

    DECOMP_EMIT_COPY const char* GetName(s32 index) { return LMS_GetName(m_pBinary, index); } // 0x00159B5C (name is ours)
    DECOMP_EMIT_COPY s32 GetNameNum() { return LMS_GetNameNum(m_pBinary); } // 0x00159BA0 (name is ours)

private:
    LMS_ProjectBinary* m_pBinary;  // +0x00
    const void* m_pData;           // +0x04
};
ASSERT_SIZE(Project, 0x8);

} // namespace libms
