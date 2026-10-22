#include "nw/gfx/res/res_Api.h"

namespace nw {
namespace gfx {
namespace res {
// 0x004A874C | nintendogs:bytes [tier A]
ReferenceTarget<ResShader> GetReferenceShaderTarget(ResReferenceShader shader, ResGraphicsFile graphicsFile)
{
    bool isFound = false;
    const void* pFound = graphicsFile.ptr()->shaders.GetData(shader.ptr()->path.To<char>());
    if (pFound != nullptr) {
        const void* pReference = DynamicCastData<ResShaderData>(pFound, ResShader::TYPE_REFERENCE);
        if (pReference != nullptr) {
            ReferenceTarget<ResShader> target = GetReferenceShaderTarget(ResReferenceShader(pReference), graphicsFile);
            if (target.isFound) {
                shader.ptr()->target.Set(target.target.ptr());
                isFound = true;
            }
        } else {
            shader.ptr()->target.Set(pFound);
            isFound = true;
        }
    }
    ReferenceTarget<ResShader> target;
    target.target = ResShader(shader.ptr()->target.ToPtr());
    target.isFound = isFound;
    return target;
}

} // namespace res
} // namespace gfx
} // namespace nw
