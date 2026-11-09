#pragma once

#include "decomp.h"
#include "script/dProject.h"

// a condition for the names of the project (RTTI only; slot names are ours)
// RTTI N6script7Project23ISearchContentConditionE @ 0x008D3AFC
class script::Project::ISearchContentCondition
{
public:
    // true: the name matches
    virtual bool Match(const char* name) = 0;
    virtual ~ISearchContentCondition() {}
};
