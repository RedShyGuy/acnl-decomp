#include "libms/lms_Api.h"

extern "C" {

// 0x0015A080 (name is ours)
LMS_ProjectBinary* LMS_InitProject(const void* data)
{
    LMS_ProjectBinary* project = static_cast<LMS_ProjectBinary*>(LMSi_Allocate(sizeof(LMS_ProjectBinary)));
    project->common.data = static_cast<const u8*>(data);
    LMSi_AnalyzeMessageHeader(&project->common, "MsgPrjBn");
    project->clr1 = LMSi_SearchBlockByName(&project->common, "CLR1");
    project->clb1 = LMSi_SearchBlockByName(&project->common, "CLB1");
    project->ati2 = LMSi_SearchBlockByName(&project->common, "ATI2");
    project->alb1 = LMSi_SearchBlockByName(&project->common, "ALB1");
    project->ali2 = LMSi_SearchBlockByName(&project->common, "ALI2");
    project->tgg2 = LMSi_SearchBlockByName(&project->common, "TGG2");
    project->tag2 = LMSi_SearchBlockByName(&project->common, "TAG2");
    project->tgp2 = LMSi_SearchBlockByName(&project->common, "TGP2");
    project->tgl2 = LMSi_SearchBlockByName(&project->common, "TGL2");
    project->syl3 = LMSi_SearchBlockByName(&project->common, "SYL3");
    project->slb1 = LMSi_SearchBlockByName(&project->common, "SLB1");
    project->cti1 = LMSi_SearchBlockByName(&project->common, "CTI1");
    return project;
}

// (armlink replaced the tail call to LMSi_Free by a nop: the function falls into it)
// 0x0013DDE4 (name is ours)
void LMS_CloseProject(LMS_ProjectBinary* project)
{
    if (project->common.blocks != NULL)
    {
        LMSi_Free(project->common.blocks);
    }
    LMSi_Free(project);
}

// CLR1: u32 colorNum, RGBA colors
// 0x00159B04 (name is ours)
s32 LMS_GetColor(LMS_ProjectBinary* project, s32 index, u8* rgba)
{
    if (project->clr1 == -1)
    {
        return LMS_ERROR_NO_COLORS;
    }
    const u8* data = project->common.blocks[project->clr1].data;
    if (*reinterpret_cast<const s32*>(data) <= index)
    {
        return LMS_ERROR_NOT_FOUND;
    }
    const u8* color = data + 4 + index * 4;
    rgba[0] = color[0];
    rgba[1] = color[1];
    rgba[2] = color[2];
    rgba[3] = color[3];
    return 0;
}

// CTI1: u32 nameNum, u32 offset[nameNum] of the strings (from the start of the block)
// 0x00159BA8 (name is ours)
s32 LMS_GetNameNum(LMS_ProjectBinary* project)
{
    if (project->cti1 == -1)
    {
        return -1;
    }
    return *reinterpret_cast<const s32*>(project->common.blocks[project->cti1].data);
}

// 0x00159B64 (name is ours)
const char* LMS_GetName(LMS_ProjectBinary* project, s32 index)
{
    if (project->cti1 == -1)
    {
        return NULL;
    }
    const u8* data = project->common.blocks[project->cti1].data;
    if (*reinterpret_cast<const s32*>(data) > index)
    {
        return reinterpret_cast<const char*>(data + reinterpret_cast<const u32*>(data + 4)[index]);
    }
    return NULL;
}

} // extern "C"
