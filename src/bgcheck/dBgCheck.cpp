#include "bgcheck/dDynamicCylinder.h"
#include "bgcheck/dMoveBg.h"

// the collision worlds of bgcheck that are written so far (the static initializer that builds them also builds globals
// of other files; 0x00987BE0 and 0x00987D78 belong to the movement test 0x0061553C)
// 0x00987F10
collision::World bgcheck::g_MoveBgWorld;
// 0x009880A8
collision::World bgcheck::g_CylinderWorld;

namespace bgcheck {
// 0x00615208
void AddBody(collision::World::Body* body)
{
    if (body != NULL) {
        g_CylinderWorld.Remove(body);
        g_CylinderWorld.Add(body);
    }
}

// 0x00615244
void RemoveBody(collision::World::Body* body)
{
    if (body != NULL) {
        g_CylinderWorld.Remove(body);
    }
}

// 0x0061550C
void AddMoveBg(collision::World::Body* body)
{
    if (body != NULL) {
        g_MoveBgWorld.Remove(body);
        g_MoveBgWorld.Add(body);
    }
}

} // namespace bgcheck
