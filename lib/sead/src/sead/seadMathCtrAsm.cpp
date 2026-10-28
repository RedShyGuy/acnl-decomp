// The hand-written VFP assembly of sead's math (the *CalcCtr functions on the nn::math types): the
// bodies are copied from ACNL USA 1.5 (orig/0004000000086300/code.elf) as naked functions, like
// nn::math's *Asm. Literal loads and branches use local labels; each function has its own literals
// (the original shares some pools between neighbouring functions).

#include "sead/seadMathCalcCommon.h"

namespace sead {
// the table of 256 steps per turn (and the first again): sin, its change to the next step, cos, its
// change (name is ours)
// 0x008C9AF4
extern const MathSinCosEntry s_MathSinCosTable[257] = {
    {0.0f, 0.024541229f, 1.0f, -0.00030118131f},
    {0.024541229f, 0.0245264452f, 0.999698818f, -0.000903362466f},
    {0.0490676761f, 0.0244968887f, 0.99879545f, -0.00150499947f},
    {0.0735645667f, 0.0244525764f, 0.997290432f, -0.00210573012f},
    {0.0980171412f, 0.0243935343f, 0.99518472f, -0.00270519196f},
    {0.122410677f, 0.0243197996f, 0.992479563f, -0.00330302469f},
    {0.146730468f, 0.0242314134f, 0.989176512f, -0.00389886764f},
    {0.170961887f, 0.0241284333f, 0.985277653f, -0.00449236203f},
    {0.195090324f, 0.024010919f, 0.980785251f, -0.00508315023f},
    {0.219101235f, 0.0238789394f, 0.975702107f, -0.00567087671f},
    {0.242980182f, 0.0237325784f, 0.970031261f, -0.00625518756f},
    {0.266712755f, 0.0235719196f, 0.963776052f, -0.00683573028f},
    {0.290284663f, 0.0233970638f, 0.956940353f, -0.00741215516f},
    {0.313681751f, 0.0232081134f, 0.949528158f, -0.00798411574f},
    {0.336889863f, 0.0230051838f, 0.941544056f, -0.00855126604f},
    {0.359895051f, 0.0227883961f, 0.932992816f, -0.00911326613f},
    {0.382683426f, 0.0225578807f, 0.923879504f, -0.00966977701f},
    {0.405241311f, 0.0223137792f, 0.914209783f, -0.0102204625f},
    {0.427555084f, 0.0220562369f, 0.903989315f, -0.0107649919f},
    {0.449611336f, 0.0217854064f, 0.893224299f, -0.0113030365f},
    {0.471396744f, 0.0215014555f, 0.881921291f, -0.0118342731f},
    {0.492898196f, 0.0212045517f, 0.870086968f, -0.0123583814f},
    {0.514102757f, 0.0208948757f, 0.857728601f, -0.0128750447f},
    {0.534997642f, 0.0205726139f, 0.84485358f, -0.0133839529f},
    {0.555570245f, 0.0202379581f, 0.831469595f, -0.0138847996f},
    {0.575808167f, 0.019891113f, 0.817584813f, -0.014377282f},
    {0.59569931f, 0.0195322856f, 0.803207517f, -0.0148611041f},
    {0.615231574f, 0.0191616938f, 0.78834641f, -0.0153359743f},
    {0.634393275f, 0.0187795591f, 0.773010433f, -0.0158016067f},
    {0.653172851f, 0.0183861125f, 0.757208824f, -0.0162577219f},
    {0.671558976f, 0.0179815907f, 0.740951121f, -0.0167040415f},
    {0.689540565f, 0.0175662357f, 0.724247098f, -0.0171403009f},
    {0.707106769f, 0.0171403009f, 0.707106769f, -0.0175662357f},
    {0.724247098f, 0.0167040415f, 0.689540565f, -0.0179815907f},
    {0.740951121f, 0.0162577219f, 0.671558976f, -0.0183861125f},
    {0.757208824f, 0.0158016067f, 0.653172851f, -0.0187795591f},
    {0.773010433f, 0.0153359743f, 0.634393275f, -0.0191616938f},
    {0.78834641f, 0.0148611041f, 0.615231574f, -0.0195322856f},
    {0.803207517f, 0.014377282f, 0.59569931f, -0.019891113f},
    {0.817584813f, 0.0138847996f, 0.575808167f, -0.0202379581f},
    {0.831469595f, 0.0133839529f, 0.555570245f, -0.0205726139f},
    {0.84485358f, 0.0128750447f, 0.534997642f, -0.0208948757f},
    {0.857728601f, 0.0123583814f, 0.514102757f, -0.0212045517f},
    {0.870086968f, 0.0118342731f, 0.492898196f, -0.0215014555f},
    {0.881921291f, 0.0113030365f, 0.471396744f, -0.0217854064f},
    {0.893224299f, 0.0107649919f, 0.449611336f, -0.0220562369f},
    {0.903989315f, 0.0102204625f, 0.427555084f, -0.0223137792f},
    {0.914209783f, 0.00966977701f, 0.405241311f, -0.0225578807f},
    {0.923879504f, 0.00911326613f, 0.382683426f, -0.0227883961f},
    {0.932992816f, 0.00855126604f, 0.359895051f, -0.0230051838f},
    {0.941544056f, 0.00798411574f, 0.336889863f, -0.0232081134f},
    {0.949528158f, 0.00741215516f, 0.313681751f, -0.0233970638f},
    {0.956940353f, 0.00683573028f, 0.290284663f, -0.0235719196f},
    {0.963776052f, 0.00625518756f, 0.266712755f, -0.0237325784f},
    {0.970031261f, 0.00567087671f, 0.242980182f, -0.0238789394f},
    {0.975702107f, 0.00508315023f, 0.219101235f, -0.024010919f},
    {0.980785251f, 0.00449236203f, 0.195090324f, -0.0241284333f},
    {0.985277653f, 0.00389886764f, 0.170961887f, -0.0242314134f},
    {0.989176512f, 0.00330302469f, 0.146730468f, -0.0243197996f},
    {0.992479563f, 0.00270519196f, 0.122410677f, -0.0243935343f},
    {0.99518472f, 0.00210573012f, 0.0980171412f, -0.0244525764f},
    {0.997290432f, 0.00150499947f, 0.0735645667f, -0.0244968887f},
    {0.99879545f, 0.000903362466f, 0.0490676761f, -0.0245264452f},
    {0.999698818f, 0.00030118131f, 0.024541229f, -0.024541229f},
    {1.0f, -0.00030118131f, 0.0f, -0.024541229f},
    {0.999698818f, -0.000903362466f, -0.024541229f, -0.0245264452f},
    {0.99879545f, -0.00150499947f, -0.0490676761f, -0.0244968887f},
    {0.997290432f, -0.00210573012f, -0.0735645667f, -0.0244525764f},
    {0.99518472f, -0.00270519196f, -0.0980171412f, -0.0243935343f},
    {0.992479563f, -0.00330302469f, -0.122410677f, -0.0243197996f},
    {0.989176512f, -0.00389886764f, -0.146730468f, -0.0242314134f},
    {0.985277653f, -0.00449236203f, -0.170961887f, -0.0241284333f},
    {0.980785251f, -0.00508315023f, -0.195090324f, -0.024010919f},
    {0.975702107f, -0.00567087671f, -0.219101235f, -0.0238789394f},
    {0.970031261f, -0.00625518756f, -0.242980182f, -0.0237325784f},
    {0.963776052f, -0.00683573028f, -0.266712755f, -0.0235719196f},
    {0.956940353f, -0.00741215516f, -0.290284663f, -0.0233970638f},
    {0.949528158f, -0.00798411574f, -0.313681751f, -0.0232081134f},
    {0.941544056f, -0.00855126604f, -0.336889863f, -0.0230051838f},
    {0.932992816f, -0.00911326613f, -0.359895051f, -0.0227883961f},
    {0.923879504f, -0.00966977701f, -0.382683426f, -0.0225578807f},
    {0.914209783f, -0.0102204625f, -0.405241311f, -0.0223137792f},
    {0.903989315f, -0.0107649919f, -0.427555084f, -0.0220562369f},
    {0.893224299f, -0.0113030365f, -0.449611336f, -0.0217854064f},
    {0.881921291f, -0.0118342731f, -0.471396744f, -0.0215014555f},
    {0.870086968f, -0.0123583814f, -0.492898196f, -0.0212045517f},
    {0.857728601f, -0.0128750447f, -0.514102757f, -0.0208948757f},
    {0.84485358f, -0.0133839529f, -0.534997642f, -0.0205726139f},
    {0.831469595f, -0.0138847996f, -0.555570245f, -0.0202379581f},
    {0.817584813f, -0.014377282f, -0.575808167f, -0.019891113f},
    {0.803207517f, -0.0148611041f, -0.59569931f, -0.0195322856f},
    {0.78834641f, -0.0153359743f, -0.615231574f, -0.0191616938f},
    {0.773010433f, -0.0158016067f, -0.634393275f, -0.0187795591f},
    {0.757208824f, -0.0162577219f, -0.653172851f, -0.0183861125f},
    {0.740951121f, -0.0167040415f, -0.671558976f, -0.0179815907f},
    {0.724247098f, -0.0171403009f, -0.689540565f, -0.0175662357f},
    {0.707106769f, -0.0175662357f, -0.707106769f, -0.0171403009f},
    {0.689540565f, -0.0179815907f, -0.724247098f, -0.0167040415f},
    {0.671558976f, -0.0183861125f, -0.740951121f, -0.0162577219f},
    {0.653172851f, -0.0187795591f, -0.757208824f, -0.0158016067f},
    {0.634393275f, -0.0191616938f, -0.773010433f, -0.0153359743f},
    {0.615231574f, -0.0195322856f, -0.78834641f, -0.0148611041f},
    {0.59569931f, -0.019891113f, -0.803207517f, -0.014377282f},
    {0.575808167f, -0.0202379581f, -0.817584813f, -0.0138847996f},
    {0.555570245f, -0.0205726139f, -0.831469595f, -0.0133839529f},
    {0.534997642f, -0.0208948757f, -0.84485358f, -0.0128750447f},
    {0.514102757f, -0.0212045517f, -0.857728601f, -0.0123583814f},
    {0.492898196f, -0.0215014555f, -0.870086968f, -0.0118342731f},
    {0.471396744f, -0.0217854064f, -0.881921291f, -0.0113030365f},
    {0.449611336f, -0.0220562369f, -0.893224299f, -0.0107649919f},
    {0.427555084f, -0.0223137792f, -0.903989315f, -0.0102204625f},
    {0.405241311f, -0.0225578807f, -0.914209783f, -0.00966977701f},
    {0.382683426f, -0.0227883961f, -0.923879504f, -0.00911326613f},
    {0.359895051f, -0.0230051838f, -0.932992816f, -0.00855126604f},
    {0.336889863f, -0.0232081134f, -0.941544056f, -0.00798411574f},
    {0.313681751f, -0.0233970638f, -0.949528158f, -0.00741215516f},
    {0.290284663f, -0.0235719196f, -0.956940353f, -0.00683573028f},
    {0.266712755f, -0.0237325784f, -0.963776052f, -0.00625518756f},
    {0.242980182f, -0.0238789394f, -0.970031261f, -0.00567087671f},
    {0.219101235f, -0.024010919f, -0.975702107f, -0.00508315023f},
    {0.195090324f, -0.0241284333f, -0.980785251f, -0.00449236203f},
    {0.170961887f, -0.0242314134f, -0.985277653f, -0.00389886764f},
    {0.146730468f, -0.0243197996f, -0.989176512f, -0.00330302469f},
    {0.122410677f, -0.0243935343f, -0.992479563f, -0.00270519196f},
    {0.0980171412f, -0.0244525764f, -0.99518472f, -0.00210573012f},
    {0.0735645667f, -0.0244968887f, -0.997290432f, -0.00150499947f},
    {0.0490676761f, -0.0245264452f, -0.99879545f, -0.000903362466f},
    {0.024541229f, -0.024541229f, -0.999698818f, -0.00030118131f},
    {0.0f, -0.024541229f, -1.0f, 0.00030118131f},
    {-0.024541229f, -0.0245264452f, -0.999698818f, 0.000903362466f},
    {-0.0490676761f, -0.0244968887f, -0.99879545f, 0.00150499947f},
    {-0.0735645667f, -0.0244525764f, -0.997290432f, 0.00210573012f},
    {-0.0980171412f, -0.0243935343f, -0.99518472f, 0.00270519196f},
    {-0.122410677f, -0.0243197996f, -0.992479563f, 0.00330302469f},
    {-0.146730468f, -0.0242314134f, -0.989176512f, 0.00389886764f},
    {-0.170961887f, -0.0241284333f, -0.985277653f, 0.00449236203f},
    {-0.195090324f, -0.024010919f, -0.980785251f, 0.00508315023f},
    {-0.219101235f, -0.0238789394f, -0.975702107f, 0.00567087671f},
    {-0.242980182f, -0.0237325784f, -0.970031261f, 0.00625518756f},
    {-0.266712755f, -0.0235719196f, -0.963776052f, 0.00683573028f},
    {-0.290284663f, -0.0233970638f, -0.956940353f, 0.00741215516f},
    {-0.313681751f, -0.0232081134f, -0.949528158f, 0.00798411574f},
    {-0.336889863f, -0.0230051838f, -0.941544056f, 0.00855126604f},
    {-0.359895051f, -0.0227883961f, -0.932992816f, 0.00911326613f},
    {-0.382683426f, -0.0225578807f, -0.923879504f, 0.00966977701f},
    {-0.405241311f, -0.0223137792f, -0.914209783f, 0.0102204625f},
    {-0.427555084f, -0.0220562369f, -0.903989315f, 0.0107649919f},
    {-0.449611336f, -0.0217854064f, -0.893224299f, 0.0113030365f},
    {-0.471396744f, -0.0215014555f, -0.881921291f, 0.0118342731f},
    {-0.492898196f, -0.0212045517f, -0.870086968f, 0.0123583814f},
    {-0.514102757f, -0.0208948757f, -0.857728601f, 0.0128750447f},
    {-0.534997642f, -0.0205726139f, -0.84485358f, 0.0133839529f},
    {-0.555570245f, -0.0202379581f, -0.831469595f, 0.0138847996f},
    {-0.575808167f, -0.019891113f, -0.817584813f, 0.014377282f},
    {-0.59569931f, -0.0195322856f, -0.803207517f, 0.0148611041f},
    {-0.615231574f, -0.0191616938f, -0.78834641f, 0.0153359743f},
    {-0.634393275f, -0.0187795591f, -0.773010433f, 0.0158016067f},
    {-0.653172851f, -0.0183861125f, -0.757208824f, 0.0162577219f},
    {-0.671558976f, -0.0179815907f, -0.740951121f, 0.0167040415f},
    {-0.689540565f, -0.0175662357f, -0.724247098f, 0.0171403009f},
    {-0.707106769f, -0.0171403009f, -0.707106769f, 0.0175662357f},
    {-0.724247098f, -0.0167040415f, -0.689540565f, 0.0179815907f},
    {-0.740951121f, -0.0162577219f, -0.671558976f, 0.0183861125f},
    {-0.757208824f, -0.0158016067f, -0.653172851f, 0.0187795591f},
    {-0.773010433f, -0.0153359743f, -0.634393275f, 0.0191616938f},
    {-0.78834641f, -0.0148611041f, -0.615231574f, 0.0195322856f},
    {-0.803207517f, -0.014377282f, -0.59569931f, 0.019891113f},
    {-0.817584813f, -0.0138847996f, -0.575808167f, 0.0202379581f},
    {-0.831469595f, -0.0133839529f, -0.555570245f, 0.0205726139f},
    {-0.84485358f, -0.0128750447f, -0.534997642f, 0.0208948757f},
    {-0.857728601f, -0.0123583814f, -0.514102757f, 0.0212045517f},
    {-0.870086968f, -0.0118342731f, -0.492898196f, 0.0215014555f},
    {-0.881921291f, -0.0113030365f, -0.471396744f, 0.0217854064f},
    {-0.893224299f, -0.0107649919f, -0.449611336f, 0.0220562369f},
    {-0.903989315f, -0.0102204625f, -0.427555084f, 0.0223137792f},
    {-0.914209783f, -0.00966977701f, -0.405241311f, 0.0225578807f},
    {-0.923879504f, -0.00911326613f, -0.382683426f, 0.0227883961f},
    {-0.932992816f, -0.00855126604f, -0.359895051f, 0.0230051838f},
    {-0.941544056f, -0.00798411574f, -0.336889863f, 0.0232081134f},
    {-0.949528158f, -0.00741215516f, -0.313681751f, 0.0233970638f},
    {-0.956940353f, -0.00683573028f, -0.290284663f, 0.0235719196f},
    {-0.963776052f, -0.00625518756f, -0.266712755f, 0.0237325784f},
    {-0.970031261f, -0.00567087671f, -0.242980182f, 0.0238789394f},
    {-0.975702107f, -0.00508315023f, -0.219101235f, 0.024010919f},
    {-0.980785251f, -0.00449236203f, -0.195090324f, 0.0241284333f},
    {-0.985277653f, -0.00389886764f, -0.170961887f, 0.0242314134f},
    {-0.989176512f, -0.00330302469f, -0.146730468f, 0.0243197996f},
    {-0.992479563f, -0.00270519196f, -0.122410677f, 0.0243935343f},
    {-0.99518472f, -0.00210573012f, -0.0980171412f, 0.0244525764f},
    {-0.997290432f, -0.00150499947f, -0.0735645667f, 0.0244968887f},
    {-0.99879545f, -0.000903362466f, -0.0490676761f, 0.0245264452f},
    {-0.999698818f, -0.00030118131f, -0.024541229f, 0.024541229f},
    {-1.0f, 0.00030118131f, 0.0f, 0.024541229f},
    {-0.999698818f, 0.000903362466f, 0.024541229f, 0.0245264452f},
    {-0.99879545f, 0.00150499947f, 0.0490676761f, 0.0244968887f},
    {-0.997290432f, 0.00210573012f, 0.0735645667f, 0.0244525764f},
    {-0.99518472f, 0.00270519196f, 0.0980171412f, 0.0243935343f},
    {-0.992479563f, 0.00330302469f, 0.122410677f, 0.0243197996f},
    {-0.989176512f, 0.00389886764f, 0.146730468f, 0.0242314134f},
    {-0.985277653f, 0.00449236203f, 0.170961887f, 0.0241284333f},
    {-0.980785251f, 0.00508315023f, 0.195090324f, 0.024010919f},
    {-0.975702107f, 0.00567087671f, 0.219101235f, 0.0238789394f},
    {-0.970031261f, 0.00625518756f, 0.242980182f, 0.0237325784f},
    {-0.963776052f, 0.00683573028f, 0.266712755f, 0.0235719196f},
    {-0.956940353f, 0.00741215516f, 0.290284663f, 0.0233970638f},
    {-0.949528158f, 0.00798411574f, 0.313681751f, 0.0232081134f},
    {-0.941544056f, 0.00855126604f, 0.336889863f, 0.0230051838f},
    {-0.932992816f, 0.00911326613f, 0.359895051f, 0.0227883961f},
    {-0.923879504f, 0.00966977701f, 0.382683426f, 0.0225578807f},
    {-0.914209783f, 0.0102204625f, 0.405241311f, 0.0223137792f},
    {-0.903989315f, 0.0107649919f, 0.427555084f, 0.0220562369f},
    {-0.893224299f, 0.0113030365f, 0.449611336f, 0.0217854064f},
    {-0.881921291f, 0.0118342731f, 0.471396744f, 0.0215014555f},
    {-0.870086968f, 0.0123583814f, 0.492898196f, 0.0212045517f},
    {-0.857728601f, 0.0128750447f, 0.514102757f, 0.0208948757f},
    {-0.84485358f, 0.0133839529f, 0.534997642f, 0.0205726139f},
    {-0.831469595f, 0.0138847996f, 0.555570245f, 0.0202379581f},
    {-0.817584813f, 0.014377282f, 0.575808167f, 0.019891113f},
    {-0.803207517f, 0.0148611041f, 0.59569931f, 0.0195322856f},
    {-0.78834641f, 0.0153359743f, 0.615231574f, 0.0191616938f},
    {-0.773010433f, 0.0158016067f, 0.634393275f, 0.0187795591f},
    {-0.757208824f, 0.0162577219f, 0.653172851f, 0.0183861125f},
    {-0.740951121f, 0.0167040415f, 0.671558976f, 0.0179815907f},
    {-0.724247098f, 0.0171403009f, 0.689540565f, 0.0175662357f},
    {-0.707106769f, 0.0175662357f, 0.707106769f, 0.0171403009f},
    {-0.689540565f, 0.0179815907f, 0.724247098f, 0.0167040415f},
    {-0.671558976f, 0.0183861125f, 0.740951121f, 0.0162577219f},
    {-0.653172851f, 0.0187795591f, 0.757208824f, 0.0158016067f},
    {-0.634393275f, 0.0191616938f, 0.773010433f, 0.0153359743f},
    {-0.615231574f, 0.0195322856f, 0.78834641f, 0.0148611041f},
    {-0.59569931f, 0.019891113f, 0.803207517f, 0.014377282f},
    {-0.575808167f, 0.0202379581f, 0.817584813f, 0.0138847996f},
    {-0.555570245f, 0.0205726139f, 0.831469595f, 0.0133839529f},
    {-0.534997642f, 0.0208948757f, 0.84485358f, 0.0128750447f},
    {-0.514102757f, 0.0212045517f, 0.857728601f, 0.0123583814f},
    {-0.492898196f, 0.0215014555f, 0.870086968f, 0.0118342731f},
    {-0.471396744f, 0.0217854064f, 0.881921291f, 0.0113030365f},
    {-0.449611336f, 0.0220562369f, 0.893224299f, 0.0107649919f},
    {-0.427555084f, 0.0223137792f, 0.903989315f, 0.0102204625f},
    {-0.405241311f, 0.0225578807f, 0.914209783f, 0.00966977701f},
    {-0.382683426f, 0.0227883961f, 0.923879504f, 0.00911326613f},
    {-0.359895051f, 0.0230051838f, 0.932992816f, 0.00855126604f},
    {-0.336889863f, 0.0232081134f, 0.941544056f, 0.00798411574f},
    {-0.313681751f, 0.0233970638f, 0.949528158f, 0.00741215516f},
    {-0.290284663f, 0.0235719196f, 0.956940353f, 0.00683573028f},
    {-0.266712755f, 0.0237325784f, 0.963776052f, 0.00625518756f},
    {-0.242980182f, 0.0238789394f, 0.970031261f, 0.00567087671f},
    {-0.219101235f, 0.024010919f, 0.975702107f, 0.00508315023f},
    {-0.195090324f, 0.0241284333f, 0.980785251f, 0.00449236203f},
    {-0.170961887f, 0.0242314134f, 0.985277653f, 0.00389886764f},
    {-0.146730468f, 0.0243197996f, 0.989176512f, 0.00330302469f},
    {-0.122410677f, 0.0243935343f, 0.992479563f, 0.00270519196f},
    {-0.0980171412f, 0.0244525764f, 0.99518472f, 0.00210573012f},
    {-0.0735645667f, 0.0244968887f, 0.997290432f, 0.00150499947f},
    {-0.0490676761f, 0.0245264452f, 0.99879545f, 0.000903362466f},
    {-0.024541229f, 0.024541229f, 0.999698818f, 0.00030118131f},
    {0.0f, 0.024541229f, 1.0f, -0.00030118131f},
};

// the angle index of atan(i / 128) for i = 0..128 (one turn = 2^32) and the change to the next entry
// (name is ours)
// 0x008CAB04
extern const MathAtanEntry s_MathAtanTable[129] = {
    {0x00000000, 5340245.0f},
    {0x00517C55, 5339593.0f},
    {0x00A2F61E, 5338290.0f},
    {0x00F46AD0, 5336337.0f},
    {0x0145D7E1, 5333734.0f},
    {0x01973AC7, 5330485.0f},
    {0x01E890FC, 5326591.0f},
    {0x0239D7FB, 5322056.0f},
    {0x028B0D43, 5316880.0f},
    {0x02DC2E53, 5311072.0f},
    {0x032D38B3, 5304632.0f},
    {0x037E29EB, 5297566.0f},
    {0x03CEFF89, 5289880.0f},
    {0x041FB721, 5281577.0f},
    {0x04704E4A, 5272666.0f},
    {0x04C0C2A4, 5263152.0f},
    {0x051111D4, 5253040.0f},
    {0x05613984, 5242339.0f},
    {0x05B13767, 5231056.0f},
    {0x06010937, 5219199.0f},
    {0x0650ACB6, 5206776.0f},
    {0x06A01FAE, 5193795.0f},
    {0x06EF5FF1, 5180265.0f},
    {0x073E6B5A, 5166196.0f},
    {0x078D3FCE, 5151596.0f},
    {0x07DBDB3A, 5136474.0f},
    {0x082A3B94, 5120843.0f},
    {0x08785EDF, 5104710.0f},
    {0x08C64325, 5088087.0f},
    {0x0913E67C, 5070983.0f},
    {0x09614703, 5053411.0f},
    {0x09AE62E6, 5035381.0f},
    {0x09FB385B, 5016903.0f},
    {0x0A47C5A2, 4997989.0f},
    {0x0A940907, 4978650.0f},
    {0x0AE000E1, 4958900.0f},
    {0x0B2BAB95, 4938746.0f},
    {0x0B77078F, 4918204.0f},
    {0x0BC2134B, 4897283.0f},
    {0x0C0CCD4E, 4875996.0f},
    {0x0C57342A, 4854354.0f},
    {0x0CA1467C, 4832371.0f},
    {0x0CEB02EF, 4810055.0f},
    {0x0D346836, 4787422.0f},
    {0x0D7D7514, 4764482.0f},
    {0x0DC62856, 4741246.0f},
    {0x0E0E80D4, 4717727.0f},
    {0x0E567D73, 4693937.0f},
    {0x0E9E1D24, 4669886.0f},
    {0x0EE55EE2, 4645588.0f},
    {0x0F2C41B6, 4621054.0f},
    {0x0F72C4B4, 4596293.0f},
    {0x0FB8E6F9, 4571319.0f},
    {0x0FFEA7B0, 4546143.0f},
    {0x1044060F, 4520775.0f},
    {0x10890156, 4495227.0f},
    {0x10CD98D1, 4469509.0f},
    {0x1111CBD6, 4443632.0f},
    {0x115599C6, 4417608.0f},
    {0x1199020E, 4391445.0f},
    {0x11DC0423, 4365155.0f},
    {0x121E9F86, 4338747.0f},
    {0x1260D3C1, 4312233.0f},
    {0x12A2A06A, 4285619.0f},
    {0x12E4051D, 4258919.0f},
    {0x13250184, 4232138.0f},
    {0x1365954E, 4205290.0f},
    {0x13A5C038, 4178379.0f},
    {0x13E58203, 4151418.0f},
    {0x1424DA7D, 4124413.0f},
    {0x1463C97A, 4097373.0f},
    {0x14A24ED7, 4070307.0f},
    {0x14E06A7A, 4043223.0f},
    {0x151E1C51, 4016127.0f},
    {0x155B6450, 3989029.0f},
    {0x15984275, 3961935.0f},
    {0x15D4B6C4, 3934853.0f},
    {0x1610C149, 3907789.0f},
    {0x164C6216, 3880751.0f},
    {0x16879945, 3853745.0f},
    {0x16C266F6, 3826778.0f},
    {0x16FCCB50, 3799855.0f},
    {0x1736C67F, 3772982.0f},
    {0x177058B5, 3746167.0f},
    {0x17A9822C, 3719414.0f},
    {0x17E24322, 3692729.0f},
    {0x181A9BDB, 3666115.0f},
    {0x18528C9E, 3639581.0f},
    {0x188A15BB, 3613130.0f},
    {0x18C13785, 3586765.0f},
    {0x18F7F252, 3560493.0f},
    {0x192E467F, 3534318.0f},
    {0x1964346D, 3508243.0f},
    {0x1999BC80, 3482274.0f},
    {0x19CEDF22, 3456412.0f},
    {0x1A039CBE, 3430662.0f},
    {0x1A37F5C4, 3405030.0f},
    {0x1A6BEAAA, 3379514.0f},
    {0x1A9F7BE4, 3354123.0f},
    {0x1AD2A9EF, 3328857.0f},
    {0x1B057548, 3303718.0f},
    {0x1B37DE6E, 3278711.0f},
    {0x1B69E5E5, 3253838.0f},
    {0x1B9B8C33, 3229100.0f},
    {0x1BCCD1DF, 3204502.0f},
    {0x1BFDB775, 3180044.0f},
    {0x1C2E3D81, 3155729.0f},
    {0x1C5E6492, 3131558.0f},
    {0x1C8E2D38, 3107535.0f},
    {0x1CBD9807, 3083660.0f},
    {0x1CECA593, 3059934.0f},
    {0x1D1B5671, 3036361.0f},
    {0x1D49AB3A, 3012941.0f},
    {0x1D77A487, 2989674.0f},
    {0x1DA542F1, 2966562.0f},
    {0x1DD28713, 2943609.0f},
    {0x1DFF718C, 2920811.0f},
    {0x1E2C02F7, 2898173.0f},
    {0x1E583BF4, 2875693.0f},
    {0x1E841D21, 2853373.0f},
    {0x1EAFA71E, 2831215.0f},
    {0x1EDADA8D, 2809217.0f},
    {0x1F05B80E, 2787380.0f},
    {0x1F304042, 2765706.0f},
    {0x1F5A73CC, 2744194.0f},
    {0x1F84534E, 2722845.0f},
    {0x1FADDF6B, 2701658.0f},
    {0x1FD718C5, 2680635.0f},
    {0x20000000, 2659773.0f},
};

template <>
void Matrix33CalcCtr<float>::copy(nn::math::MTX33&, const nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::copy(nn::math::MTX33&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX33&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX34&, const nn::math::VEC4&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX33&, const nn::math::VEC3&, const nn::math::VEC4&) SEAD_MATH_ASM;
template <>
void Matrix22CalcCtr<float>::multiply(nn::math::MTX22&, const nn::math::MTX22&, const nn::math::MTX22&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::inverse(nn::math::MTX33&, const nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::inverseTranspose(nn::math::MTX33&, const nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::multiply(nn::math::MTX33&, const nn::math::MTX33&, const nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::transposeTo(nn::math::MTX33&, const nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix33CalcCtr<float>::makeZero(nn::math::MTX33&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::inverse(nn::math::MTX34&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::inverseTranspose(nn::math::MTX34&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::multiply(nn::math::MTX34&, const nn::math::MTX34&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::transposeTo(nn::math::MTX34&, const nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::makeZero(nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix34CalcCtr<float>::makeIdentity(nn::math::MTX34&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::multiply(nn::math::MTX44&, const nn::math::MTX44&, const nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::transpose(nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::transposeTo(nn::math::MTX44&, const nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::makeZero(nn::math::MTX44&) SEAD_MATH_ASM;
template <>
void Matrix44CalcCtr<float>::makeIdentity(nn::math::MTX44&) SEAD_MATH_ASM;
template <>
bool QuatCalcCtr<float>::makeVectorRotation(nn::math::QUAT&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector2CalcCtr<float>::normalize(nn::math::VEC2&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::add(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::sub(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::MTX34&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::MTX33&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::rotate(nn::math::VEC3&, const nn::math::MTX34&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::div(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::multScalar(nn::math::VEC3&, const nn::math::VEC3&, float) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::divScalar(nn::math::VEC3&, const nn::math::VEC3&, float) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::multScalarAdd(nn::math::VEC3&, float, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::normalize(nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void Vector3CalcCtr<float>::cross(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&) SEAD_MATH_ASM;
template <>
void MathCtr<float>::sinCosIdx(float*, float*, u32) SEAD_MATH_ASM;
template <>
float MathCtr<float>::tanIdx(u32) SEAD_MATH_ASM;

// 0x0014EEA4 (name is ours)
template <>
void sead::Matrix33CalcCtr<float>::copy(nn::math::MTX33&, const nn::math::MTX33&)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
    );
}

// 0x0014EEB0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix33CalcCtr<float>::copy(nn::math::MTX33&, const nn::math::MTX34&)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vstmia r0!, {s0-s2}\n"
        "vstmia r0!, {s4-s6}\n"
        "vstmia r0!, {s8-s10}\n"
        "bx lr\n"
    );
}

// 0x0014EEC4 | mk7dlp:callgraph [tier C]
template <>
void sead::Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX34&)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
    );
}

// 0x0014EED0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX33&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "vldmia r1!, {s4-s6}\n"
        "vldmia r1!, {s8-s10}\n"
        "vldr s3, [r2]\n"
        "vldr s7, [r2, #4]\n"
        "vldr s11, [r2, #8]\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
    );
}

// 0x0014EEF0 (name is ours)
template <>
void sead::Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX44&)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
    );
}

// 0x0014EEFC (name is ours)
template <>
void sead::Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX44&)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "vldmia r1, {s12-s15}\n"
        "vstmia r0!, {s0-s15}\n"
        "bx lr\n"
    );
}

// 0x0014EF0C (name is ours)
template <>
void sead::Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX34&, const nn::math::VEC4&)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "vldmia r2, {s12-s15}\n"
        "vstmia r0, {s0-s15}\n"
        "bx lr\n"
    );
}

// 0x0014EF1C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::copy(nn::math::MTX44&, const nn::math::MTX33&, const nn::math::VEC3&, const nn::math::VEC4&)
{
    asm volatile(
        "vldmia r1!, {s0-s2}\n"
        "vldmia r1!, {s4-s6}\n"
        "vldmia r1!, {s8-s10}\n"
        "vldr s3, [r2]\n"
        "vldr s7, [r2, #4]\n"
        "vldr s11, [r2, #8]\n"
        "vldmia r3, {s12-s15}\n"
        "vstmia r0, {s0-s15}\n"
        "bx lr\n"
    );
}

// 0x0014EF40 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix22CalcCtr<float>::multiply(nn::math::MTX22&, const nn::math::MTX22&, const nn::math::MTX22&)
{
    asm volatile(
        "vldmia r1, {s0-s3}\n"
        "vldmia r2!, {s4-s5}\n"
        "vmul.f32 s8, s0, s4\n"
        "vmul.f32 s9, s0, s5\n"
        "vmul.f32 s10, s2, s4\n"
        "vmul.f32 s11, s2, s5\n"
        "vldmia r2, {s6-s7}\n"
        "vmla.f32 s8, s1, s6\n"
        "vmla.f32 s9, s1, s7\n"
        "vmla.f32 s10, s3, s6\n"
        "vmla.f32 s11, s3, s7\n"
        "vstmia r0, {s8-s11}\n"
        "bx lr\n"
    );
}

// 0x0014EF74 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix33CalcCtr<float>::inverse(nn::math::MTX33&, const nn::math::MTX33&)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L14ef74_3f800000\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648\n"
        "cmpne r2, #0\n"
        "bne .L14eff0\n"
        "vmov.f32 s0, s15\n"
        "vldr s1, .L14ef74_0\n"
        "vldr s2, .L14ef74_0\n"
        "vldr s3, .L14ef74_0\n"
        "vmov.f32 s4, s15\n"
        "vldr s5, .L14ef74_0\n"
        "vldr s6, .L14ef74_0\n"
        "vldr s7, .L14ef74_0\n"
        "vmov.f32 s8, s15\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
        ".L14eff0:\n"
        "vpush {d8-d11}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s1, s8\n"
        "vmul.f32 s18, s1, s5\n"
        "vmul.f32 s19, s3, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s5\n"
        "vmul.f32 s9, s3, s7\n"
        "vmul.f32 s10, s0, s7\n"
        "vmul.f32 s11, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s7, s2\n"
        "vmls.f32 s18, s4, s2\n"
        "vmls.f32 s19, s6, s5\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s3, s2\n"
        "vmls.f32 s9, s6, s4\n"
        "vmls.f32 s10, s6, s1\n"
        "vmls.f32 s11, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s3, s19, s15\n"
        "vmul.f32 s4, s20, s15\n"
        "vnmul.f32 s5, s21, s15\n"
        "vmul.f32 s6, s9, s15\n"
        "vnmul.f32 s7, s10, s15\n"
        "vmul.f32 s8, s11, s15\n"
        "vpop {d8-d11}\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
        ".L14ef74_3f800000:\n"
        ".word 0x3f800000\n"
        ".L14ef74_0:\n"
        ".word 0x00000000\n"
    );
}

// 0x0014F078 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix33CalcCtr<float>::inverseTranspose(nn::math::MTX33&, const nn::math::MTX33&)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L14f078_3f800000\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648\n"
        "cmpne r2, #0\n"
        "bne .L14f0f4\n"
        "vmov.f32 s0, s15\n"
        "vldr s1, .L14f078_0\n"
        "vldr s2, .L14f078_0\n"
        "vldr s3, .L14f078_0\n"
        "vmov.f32 s4, s15\n"
        "vldr s5, .L14f078_0\n"
        "vldr s6, .L14f078_0\n"
        "vldr s7, .L14f078_0\n"
        "vmov.f32 s8, s15\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
        ".L14f0f4:\n"
        "vpush {d8-d11}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s3, s8\n"
        "vmul.f32 s18, s3, s7\n"
        "vmul.f32 s19, s1, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s7\n"
        "vmul.f32 s9, s1, s5\n"
        "vmul.f32 s10, s0, s5\n"
        "vmul.f32 s11, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s6, s5\n"
        "vmls.f32 s18, s6, s4\n"
        "vmls.f32 s19, s7, s2\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s6, s1\n"
        "vmls.f32 s9, s4, s2\n"
        "vmls.f32 s10, s3, s2\n"
        "vmls.f32 s11, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s3, s19, s15\n"
        "vmul.f32 s4, s20, s15\n"
        "vnmul.f32 s5, s21, s15\n"
        "vmul.f32 s6, s9, s15\n"
        "vnmul.f32 s7, s10, s15\n"
        "vmul.f32 s8, s11, s15\n"
        "vpop {d8-d11}\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
        ".L14f078_3f800000:\n"
        ".word 0x3f800000\n"
        ".L14f078_0:\n"
        ".word 0x00000000\n"
    );
}

// 0x0014F174 (name is ours)
template <>
void sead::Matrix33CalcCtr<float>::multiply(nn::math::MTX33&, const nn::math::MTX33&, const nn::math::MTX33&)
{
    asm volatile(
        "vpush {d8}\n"
        "vldmia r2!, {s10-s15}\n"
        "vldr s16, [r1]\n"
        "vldr s17, [r1, #12]\n"
        "vmul.f32 s0, s10, s16\n"
        "vmul.f32 s1, s11, s16\n"
        "vmul.f32 s2, s12, s16\n"
        "vldr s16, [r1, #24]\n"
        "vmul.f32 s3, s10, s17\n"
        "vmul.f32 s4, s11, s17\n"
        "vmul.f32 s5, s12, s17\n"
        "vldr s17, [r1, #4]\n"
        "vmul.f32 s6, s10, s16\n"
        "vmul.f32 s7, s11, s16\n"
        "vmul.f32 s8, s12, s16\n"
        "vldr s16, [r1, #16]\n"
        "vldmia r2, {s10-s12}\n"
        "vmla.f32 s0, s13, s17\n"
        "vmla.f32 s1, s14, s17\n"
        "vmla.f32 s2, s15, s17\n"
        "vldr s17, [r1, #28]\n"
        "vmla.f32 s3, s13, s16\n"
        "vmla.f32 s4, s14, s16\n"
        "vmla.f32 s5, s15, s16\n"
        "vldr s16, [r1, #8]\n"
        "vmla.f32 s6, s13, s17\n"
        "vmla.f32 s7, s14, s17\n"
        "vmla.f32 s8, s15, s17\n"
        "vldr s17, [r1, #20]\n"
        "vmla.f32 s0, s10, s16\n"
        "vmla.f32 s1, s11, s16\n"
        "vmla.f32 s2, s12, s16\n"
        "vldr s16, [r1, #32]\n"
        "vmla.f32 s3, s10, s17\n"
        "vmla.f32 s4, s11, s17\n"
        "vmla.f32 s5, s12, s17\n"
        "vmla.f32 s6, s10, s16\n"
        "vmla.f32 s7, s11, s16\n"
        "vmla.f32 s8, s12, s16\n"
        "vpop {d8}\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
    );
}

// 0x0014F21C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix33CalcCtr<float>::transposeTo(nn::math::MTX33&, const nn::math::MTX33&)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vstr s0, [r0]\n"
        "vstr s1, [r0, #12]\n"
        "vstr s2, [r0, #24]\n"
        "vstr s3, [r0, #4]\n"
        "vstr s4, [r0, #16]\n"
        "vstr s5, [r0, #28]\n"
        "vstr s6, [r0, #8]\n"
        "vstr s7, [r0, #20]\n"
        "vstr s8, [r0, #32]\n"
        "bx lr\n"
    );
}

// 0x0014F248 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix33CalcCtr<float>::makeZero(nn::math::MTX33&)
{
    asm volatile(
        "mov r1, #0\n"
        "mov r2, #0\n"
        "mov r3, #0\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stm r0, {r1, r2, r3}\n"
        "bx lr\n"
    );
}

// 0x0014F264 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::inverse(nn::math::MTX34&, const nn::math::MTX34&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s3-s5}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s6-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L14f264_3f800000\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648\n"
        "cmpne r2, #0\n"
        "bne .L14f2fc\n"
        "vmov.f32 s0, s15\n"
        "vldr s1, .L14f264_0\n"
        "vldr s2, .L14f264_0\n"
        "vldr s3, .L14f264_0\n"
        "vldr s4, .L14f264_0\n"
        "vmov.f32 s5, s15\n"
        "vldr s6, .L14f264_0\n"
        "vldr s7, .L14f264_0\n"
        "vldr s8, .L14f264_0\n"
        "vldr s9, .L14f264_0\n"
        "vmov.f32 s10, s15\n"
        "vldr s11, .L14f264_0\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
        ".L14f2fc:\n"
        "vpush {d8-d12}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s1, s8\n"
        "vmul.f32 s18, s1, s5\n"
        "vmul.f32 s19, s3, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s5\n"
        "vmul.f32 s22, s3, s7\n"
        "vmul.f32 s23, s0, s7\n"
        "vmul.f32 s24, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s7, s2\n"
        "vmls.f32 s18, s4, s2\n"
        "vmls.f32 s19, s6, s5\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s3, s2\n"
        "vmls.f32 s22, s6, s4\n"
        "vmls.f32 s23, s6, s1\n"
        "vmls.f32 s24, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s4, s19, s15\n"
        "vmul.f32 s5, s20, s15\n"
        "vnmul.f32 s6, s21, s15\n"
        "vmul.f32 s8, s22, s15\n"
        "vldr s12, [r1, #-20]\n"
        "vnmul.f32 s9, s23, s15\n"
        "vmul.f32 s10, s24, s15\n"
        "vnmul.f32 s3, s0, s12\n"
        "vldr s13, [r1, #-4]\n"
        "vnmul.f32 s7, s4, s12\n"
        "vnmul.f32 s11, s8, s12\n"
        "vmls.f32 s3, s1, s13\n"
        "vldr s14, [r1, #12]\n"
        "vmls.f32 s7, s5, s13\n"
        "vmls.f32 s11, s9, s13\n"
        "vmls.f32 s3, s2, s14\n"
        "vmls.f32 s7, s6, s14\n"
        "vmls.f32 s11, s10, s14\n"
        "vpop {d8-d12}\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
        ".L14f264_3f800000:\n"
        ".word 0x3f800000\n"
        ".L14f264_0:\n"
        ".word 0x00000000\n"
    );
}

// 0x0014F3AC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::inverseTranspose(nn::math::MTX34&, const nn::math::MTX34&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s3-s5}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s6-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L14f3ac_3f800000\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648\n"
        "cmpne r2, #0\n"
        "bne .L14f444\n"
        "vmov.f32 s0, s15\n"
        "vldr s1, .L14f3ac_0\n"
        "vldr s2, .L14f3ac_0\n"
        "vldr s3, .L14f3ac_0\n"
        "vldr s4, .L14f3ac_0\n"
        "vmov.f32 s5, s15\n"
        "vldr s6, .L14f3ac_0\n"
        "vldr s7, .L14f3ac_0\n"
        "vldr s8, .L14f3ac_0\n"
        "vldr s9, .L14f3ac_0\n"
        "vmov.f32 s10, s15\n"
        "vldr s11, .L14f3ac_0\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
        ".L14f444:\n"
        "vpush {d8-d12}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s3, s8\n"
        "vmul.f32 s18, s3, s7\n"
        "vmul.f32 s19, s1, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s7\n"
        "vmul.f32 s22, s1, s5\n"
        "vmul.f32 s23, s0, s5\n"
        "vmul.f32 s24, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s6, s5\n"
        "vmls.f32 s18, s6, s4\n"
        "vmls.f32 s19, s7, s2\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s6, s1\n"
        "vmls.f32 s22, s4, s2\n"
        "vmls.f32 s23, s3, s2\n"
        "vmls.f32 s24, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s4, s19, s15\n"
        "vmul.f32 s5, s20, s15\n"
        "vnmul.f32 s6, s21, s15\n"
        "vmul.f32 s8, s22, s15\n"
        "vnmul.f32 s9, s23, s15\n"
        "vmul.f32 s10, s24, s15\n"
        "vldr s3, .L14f3ac_0\n"
        "vldr s7, .L14f3ac_0\n"
        "vldr s11, .L14f3ac_0\n"
        "vpop {d8-d12}\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
        ".L14f3ac_3f800000:\n"
        ".word 0x3f800000\n"
        ".L14f3ac_0:\n"
        ".word 0x00000000\n"
    );
}

// 0x0014F4D4 | mk7dlp:callseq-callee [tier C]
template <>
void sead::Matrix34CalcCtr<float>::multiply(nn::math::MTX34&, const nn::math::MTX34&, const nn::math::MTX34&)
{
    asm volatile(
        "vpush {d8-d10}\n"
        "vldr s3, [r1, #12]\n"
        "vldr s7, [r1, #28]\n"
        "vldr s11, [r1, #44]\n"
        "vldmia r2!, {s12-s15}\n"
        "vldr s20, [r1]\n"
        "vldr s21, [r1, #16]\n"
        "vmul.f32 s0, s12, s20\n"
        "vmul.f32 s1, s13, s20\n"
        "vmul.f32 s2, s14, s20\n"
        "vmla.f32 s3, s15, s20\n"
        "vldr s20, [r1, #32]\n"
        "vmul.f32 s4, s12, s21\n"
        "vmul.f32 s5, s13, s21\n"
        "vmul.f32 s6, s14, s21\n"
        "vmla.f32 s7, s15, s21\n"
        "vldmia r2!, {s16-s19}\n"
        "vldr s21, [r1, #4]\n"
        "vmul.f32 s8, s12, s20\n"
        "vmul.f32 s9, s13, s20\n"
        "vmul.f32 s10, s14, s20\n"
        "vmla.f32 s11, s15, s20\n"
        "vldmia r2, {s12-s15}\n"
        "vldr s20, [r1, #20]\n"
        "vmla.f32 s0, s16, s21\n"
        "vmla.f32 s1, s17, s21\n"
        "vmla.f32 s2, s18, s21\n"
        "vmla.f32 s3, s19, s21\n"
        "vldr s21, [r1, #36]\n"
        "vmla.f32 s4, s16, s20\n"
        "vmla.f32 s5, s17, s20\n"
        "vmla.f32 s6, s18, s20\n"
        "vmla.f32 s7, s19, s20\n"
        "vldr s20, [r1, #8]\n"
        "vmla.f32 s8, s16, s21\n"
        "vmla.f32 s9, s17, s21\n"
        "vmla.f32 s10, s18, s21\n"
        "vmla.f32 s11, s19, s21\n"
        "vldr s21, [r1, #24]\n"
        "vmla.f32 s0, s12, s20\n"
        "vmla.f32 s1, s13, s20\n"
        "vmla.f32 s2, s14, s20\n"
        "vmla.f32 s3, s15, s20\n"
        "vldr s20, [r1, #40]\n"
        "vmla.f32 s4, s12, s21\n"
        "vmla.f32 s5, s13, s21\n"
        "vmla.f32 s6, s14, s21\n"
        "vmla.f32 s7, s15, s21\n"
        "vmla.f32 s8, s12, s20\n"
        "vmla.f32 s9, s13, s20\n"
        "vmla.f32 s10, s14, s20\n"
        "vmla.f32 s11, s15, s20\n"
        "vpop {d8-d10}\n"
        "mov r1, r0\n"
        "vstmia r1!, {s0-s3}\n"
        "vstmia r1, {s4-s11}\n"
        "bx lr\n"
    );
}

// 0x0014F5B8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::transposeTo(nn::math::MTX34&, const nn::math::MTX34&)
{
    asm volatile(
        "vldmia r1, {s0-s10}\n"
        "vldr s11, .L14f5b8_0\n"
        "vstr s0, [r0]\n"
        "vstr s1, [r0, #16]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s11, [r0, #12]\n"
        "vstr s4, [r0, #4]\n"
        "vstr s5, [r0, #20]\n"
        "vstr s6, [r0, #36]\n"
        "vstr s11, [r0, #28]\n"
        "vstr s8, [r0, #8]\n"
        "vstr s9, [r0, #24]\n"
        "vstr s10, [r0, #40]\n"
        "vstr s11, [r0, #44]\n"
        "bx lr\n"
        ".L14f5b8_0:\n"
        ".word 0x00000000\n"
    );
}

// 0x0014F5F4 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::makeZero(nn::math::MTX34&)
{
    asm volatile(
        "mov r1, #0\n"
        "mov r2, #0\n"
        "mov r3, #0\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stm r0, {r1, r2, r3}\n"
        "bx lr\n"
    );
}

// 0x0014F614 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix34CalcCtr<float>::makeIdentity(nn::math::MTX34&)
{
    asm volatile(
        "mov r1, #1065353216\n"
        "mov r2, #0\n"
        "mov r3, #0\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {r1, r2}\n"
        "bx lr\n"
    );
}

// 0x0014F638 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::multiply(nn::math::MTX44&, const nn::math::MTX44&, const nn::math::MTX44&)
{
    asm volatile(
        "vpush {d8-d15}\n"
        "vldmia r2, {s8-s23}\n"
        "vldmia r1!, {s0-s7}\n"
        "vmul.f32 s24, s8, s0\n"
        "vmul.f32 s25, s9, s0\n"
        "vmul.f32 s26, s10, s0\n"
        "vmul.f32 s27, s11, s0\n"
        "vmul.f32 s28, s8, s4\n"
        "vmul.f32 s29, s9, s4\n"
        "vmul.f32 s30, s10, s4\n"
        "vmul.f32 s31, s11, s4\n"
        "vmla.f32 s24, s12, s1\n"
        "vmla.f32 s25, s13, s1\n"
        "vmla.f32 s26, s14, s1\n"
        "vmla.f32 s27, s15, s1\n"
        "vmla.f32 s28, s12, s5\n"
        "vmla.f32 s29, s13, s5\n"
        "vmla.f32 s30, s14, s5\n"
        "vmla.f32 s31, s15, s5\n"
        "vmla.f32 s24, s16, s2\n"
        "vmla.f32 s25, s17, s2\n"
        "vmla.f32 s26, s18, s2\n"
        "vmla.f32 s27, s19, s2\n"
        "vmla.f32 s28, s16, s6\n"
        "vmla.f32 s29, s17, s6\n"
        "vmla.f32 s30, s18, s6\n"
        "vmla.f32 s31, s19, s6\n"
        "vmla.f32 s24, s20, s3\n"
        "vmla.f32 s25, s21, s3\n"
        "vmla.f32 s26, s22, s3\n"
        "vmla.f32 s27, s23, s3\n"
        "vmla.f32 s28, s20, s7\n"
        "vmla.f32 s29, s21, s7\n"
        "vmla.f32 s30, s22, s7\n"
        "vmla.f32 s31, s23, s7\n"
        "vldmia r1!, {s0-s7}\n"
        "vstmia r0!, {s24-s27}\n"
        "vstmia r0!, {s28-s31}\n"
        "vmul.f32 s24, s8, s0\n"
        "vmul.f32 s25, s9, s0\n"
        "vmul.f32 s26, s10, s0\n"
        "vmul.f32 s27, s11, s0\n"
        "vmul.f32 s28, s8, s4\n"
        "vmul.f32 s29, s9, s4\n"
        "vmul.f32 s30, s10, s4\n"
        "vmul.f32 s31, s11, s4\n"
        "vmla.f32 s24, s12, s1\n"
        "vmla.f32 s25, s13, s1\n"
        "vmla.f32 s26, s14, s1\n"
        "vmla.f32 s27, s15, s1\n"
        "vmla.f32 s28, s12, s5\n"
        "vmla.f32 s29, s13, s5\n"
        "vmla.f32 s30, s14, s5\n"
        "vmla.f32 s31, s15, s5\n"
        "vmla.f32 s24, s16, s2\n"
        "vmla.f32 s25, s17, s2\n"
        "vmla.f32 s26, s18, s2\n"
        "vmla.f32 s27, s19, s2\n"
        "vmla.f32 s28, s16, s6\n"
        "vmla.f32 s29, s17, s6\n"
        "vmla.f32 s30, s18, s6\n"
        "vmla.f32 s31, s19, s6\n"
        "vmla.f32 s24, s20, s3\n"
        "vmla.f32 s25, s21, s3\n"
        "vmla.f32 s26, s22, s3\n"
        "vmla.f32 s27, s23, s3\n"
        "vmla.f32 s28, s20, s7\n"
        "vmla.f32 s29, s21, s7\n"
        "vmla.f32 s30, s22, s7\n"
        "vmla.f32 s31, s23, s7\n"
        "vpop {d8-d11}\n"
        "vstmia r0!, {s24-s27}\n"
        "vpop {d12-d13}\n"
        "vstmia r0, {s28-s31}\n"
        "vpop {d14-d15}\n"
        "bx lr\n"
    );
}

// 0x0014F768 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::transpose(nn::math::MTX44&)
{
    asm volatile(
        "vldmia r0, {s0-s14}\n"
        "vstr s1, [r0, #16]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s3, [r0, #48]\n"
        "vstr s4, [r0, #4]\n"
        "vstr s6, [r0, #36]\n"
        "vstr s7, [r0, #52]\n"
        "vstr s8, [r0, #8]\n"
        "vstr s9, [r0, #24]\n"
        "vstr s11, [r0, #56]\n"
        "vstr s12, [r0, #12]\n"
        "vstr s13, [r0, #28]\n"
        "vstr s14, [r0, #44]\n"
        "bx lr\n"
    );
}

// 0x0014F7A0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::transposeTo(nn::math::MTX44&, const nn::math::MTX44&)
{
    asm volatile(
        "vldmia r1!, {s0-s11}\n"
        "vldmia r1, {s12-s15}\n"
        "vstr s0, [r0]\n"
        "vstr s1, [r0, #16]\n"
        "vstr s2, [r0, #32]\n"
        "vstr s3, [r0, #48]\n"
        "vstr s4, [r0, #4]\n"
        "vstr s5, [r0, #20]\n"
        "vstr s6, [r0, #36]\n"
        "vstr s7, [r0, #52]\n"
        "vstr s8, [r0, #8]\n"
        "vstr s9, [r0, #24]\n"
        "vstr s10, [r0, #40]\n"
        "vstr s11, [r0, #56]\n"
        "vstr s12, [r0, #12]\n"
        "vstr s13, [r0, #28]\n"
        "vstr s14, [r0, #44]\n"
        "vstr s15, [r0, #60]\n"
        "bx lr\n"
    );
}

// 0x0014F7EC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::makeZero(nn::math::MTX44&)
{
    asm volatile(
        "mov r1, #0\n"
        "mov r2, #0\n"
        "mov r3, #0\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "str r1, [r0]\n"
        "bx lr\n"
    );
}

// 0x0014F814 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Matrix44CalcCtr<float>::makeIdentity(nn::math::MTX44&)
{
    asm volatile(
        "mov r1, #1065353216\n"
        "mov r2, #0\n"
        "mov r3, #0\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r2, r3}\n"
        "stmia r0!, {r1, r2, r3}\n"
        "stmia r0!, {r2, r3}\n"
        "str r1, [r0]\n"
        "bx lr\n"
    );
}

// 0x0014F840 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
bool sead::QuatCalcCtr<float>::makeVectorRotation(nn::math::QUAT&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldr s9, .L14f840_3f800000\n"
        "vldmia r1!, {s0}\n"
        "vldmia r2!, {s3}\n"
        "vmla.f32 s9, s0, s3\n"
        "vldmia r1, {s1-s2}\n"
        "vldmia r2, {s4-s5}\n"
        "vmla.f32 s9, s1, s4\n"
        "vmul.f32 s6, s1, s5\n"
        "vmul.f32 s7, s2, s3\n"
        "vmul.f32 s8, s0, s4\n"
        "vmla.f32 s9, s2, s5\n"
        "vldr s10, .L14f840_34000000\n"
        "vmls.f32 s6, s2, s4\n"
        "vmls.f32 s7, s0, s5\n"
        "vmls.f32 s8, s1, s3\n"
        "vcmpe.f32 s10, s9\n"
        "vmrs APSR_nzcv, fpscr\n"
        "ldrge r1, .L14f840_14f8d0\n"
        "vldmiage r1, {s0-s3}\n"
        "vstmiage r0, {s0-s3}\n"
        "movge r0, #0\n"
        "bxge lr\n"
        "vldr s10, .L14f840_40000000\n"
        "vmul.f32 s9, s9, s10\n"
        "vsqrt.f32 s9, s9\n"
        "vldr s10, .L14f840_3f800000\n"
        "vldr s15, .L14f840_3f000000\n"
        "vdiv.f32 s10, s10, s9\n"
        "vmul.f32 s14, s9, s15\n"
        "vmul.f32 s11, s6, s10\n"
        "vmul.f32 s12, s7, s10\n"
        "vmul.f32 s13, s8, s10\n"
        "vstmia r0, {s11-s14}\n"
        "mov r0, #1\n"
        "bx lr\n"
        ".L14f840_identity:\n"
        ".word 0x00000000, 0x00000000, 0x00000000\n"
        ".L14f840_3f800000:\n"
        ".word 0x3f800000\n"
        ".L14f840_34000000:\n"
        ".word 0x34000000\n"
        ".L14f840_14f8d0:\n"
        ".word .L14f840_identity\n"
        ".L14f840_40000000:\n"
        ".word 0x40000000\n"
        ".L14f840_3f000000:\n"
        ".word 0x3f000000\n"
    );
}

// 0x0014F8EC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector2CalcCtr<float>::normalize(nn::math::VEC2&)
{
    asm volatile(
        "vldmia r0, {s2-s3}\n"
        "vmul.f32 s6, s2, s2\n"
        "vmla.f32 s6, s3, s3\n"
        "vsqrt.f32 s0, s6\n"
        "vldr s1, .L14f8ec_0\n"
        "vcmpe.f32 s0, s1\n"
        "vmrs APSR_nzcv, fpscr\n"
        "bxle lr\n"
        "vldr s6, .L14f8ec_3f800000\n"
        "vdiv.f32 s6, s6, s0\n"
        "vmul.f32 s4, s2, s6\n"
        "vmul.f32 s5, s3, s6\n"
        "vstmia r0, {s4-s5}\n"
        "bx lr\n"
        ".L14f8ec_0:\n"
        ".word 0x00000000\n"
        ".L14f8ec_3f800000:\n"
        ".word 0x3f800000\n"
    );
}

// 0x0014F92C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::add(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "vldmia r2, {s3-s5}\n"
        "vadd.f32 s6, s0, s3\n"
        "vadd.f32 s7, s1, s4\n"
        "vadd.f32 s8, s2, s5\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
    );
}

// 0x0014F948 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::sub(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "vldmia r2, {s3-s5}\n"
        "vsub.f32 s6, s0, s3\n"
        "vsub.f32 s7, s1, s4\n"
        "vsub.f32 s8, s2, s5\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
    );
}

// 0x0014F964 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "vldmia r2, {s3-s5}\n"
        "vmul.f32 s6, s0, s3\n"
        "vmul.f32 s7, s1, s4\n"
        "vmul.f32 s8, s2, s5\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
    );
}

// 0x0014F980 | mk7dlp:callgraph [tier C]
template <>
void sead::Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::MTX34&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vldmia r2, {s12-s14}\n"
        "vmla.f32 s3, s0, s12\n"
        "vmla.f32 s7, s4, s12\n"
        "vmla.f32 s11, s8, s12\n"
        "vmla.f32 s3, s1, s13\n"
        "vmla.f32 s7, s5, s13\n"
        "vmla.f32 s11, s9, s13\n"
        "vmla.f32 s3, s2, s14\n"
        "vmla.f32 s7, s6, s14\n"
        "vmla.f32 s11, s10, s14\n"
        "vstr s3, [r0]\n"
        "vstr s7, [r0, #4]\n"
        "vstr s11, [r0, #8]\n"
        "bx lr\n"
    );
}

// 0x0014F9BC (name is ours)
template <>
void sead::Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::MTX33&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vldmia r2, {s9-s11}\n"
        "vmul.f32 s12, s0, s9\n"
        "vmul.f32 s13, s3, s9\n"
        "vmul.f32 s14, s6, s9\n"
        "vmla.f32 s12, s1, s10\n"
        "vmla.f32 s13, s4, s10\n"
        "vmla.f32 s14, s7, s10\n"
        "vmla.f32 s12, s2, s11\n"
        "vmla.f32 s13, s5, s11\n"
        "vmla.f32 s14, s8, s11\n"
        "vstmia r0, {s12-s14}\n"
        "bx lr\n"
    );
}

// 0x0014F9F0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::rotate(nn::math::VEC3&, const nn::math::MTX34&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vldr s3, [r2]\n"
        "vldr s7, [r2, #4]\n"
        "vldr s11, [r2, #8]\n"
        "vmul.f32 s12, s0, s3\n"
        "vmul.f32 s13, s4, s3\n"
        "vmul.f32 s14, s8, s3\n"
        "vmla.f32 s12, s1, s7\n"
        "vmla.f32 s13, s5, s7\n"
        "vmla.f32 s14, s9, s7\n"
        "vmla.f32 s12, s2, s11\n"
        "vmla.f32 s13, s6, s11\n"
        "vmla.f32 s14, s10, s11\n"
        "vstmia r0, {s12-s14}\n"
        "bx lr\n"
    );
}

// 0x0014FA2C | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::div(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "vldmia r2, {s3-s5}\n"
        "vdiv.f32 s6, s0, s3\n"
        "vdiv.f32 s7, s1, s4\n"
        "vdiv.f32 s8, s2, s5\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
    );
}

// 0x0014FA48 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::multScalar(nn::math::VEC3&, const nn::math::VEC3&, float)
{
    asm volatile(
        "vldmia r1, {s1-s3}\n"
        "vmul.f32 s4, s1, s0\n"
        "vmul.f32 s5, s2, s0\n"
        "vmul.f32 s6, s3, s0\n"
        "vstmia r0, {s4-s6}\n"
        "bx lr\n"
    );
}

// 0x0014FA60 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::divScalar(nn::math::VEC3&, const nn::math::VEC3&, float)
{
    asm volatile(
        "vldr s1, .L14fa60_3f800000\n"
        "vdiv.f32 s2, s1, s0\n"
        "vldmia r1, {s3-s5}\n"
        "vmul.f32 s6, s3, s2\n"
        "vmul.f32 s7, s4, s2\n"
        "vmul.f32 s8, s5, s2\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
        ".L14fa60_3f800000:\n"
        ".word 0x3f800000\n"
    );
}

// 0x0014FA84 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::multScalarAdd(nn::math::VEC3&, float, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s1-s3}\n"
        "vldmia r2, {s4-s6}\n"
        "vmla.f32 s4, s1, s0\n"
        "vmla.f32 s5, s2, s0\n"
        "vmla.f32 s6, s3, s0\n"
        "vstmia r0, {s4-s6}\n"
        "bx lr\n"
    );
}

// 0x0014FAA0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::normalize(nn::math::VEC3&)
{
    asm volatile(
        "vldmia r0, {s2-s4}\n"
        "vmul.f32 s7, s2, s2\n"
        "vmla.f32 s7, s3, s3\n"
        "vmla.f32 s7, s4, s4\n"
        "vsqrt.f32 s0, s7\n"
        "vldr s1, .L14faa0_0\n"
        "vcmpe.f32 s0, s1\n"
        "vmrs APSR_nzcv, fpscr\n"
        "bxle lr\n"
        "vldr s7, .L14faa0_3f800000\n"
        "vdiv.f32 s6, s7, s0\n"
        "vmul.f32 s8, s2, s6\n"
        "vmul.f32 s9, s3, s6\n"
        "vmul.f32 s10, s4, s6\n"
        "vstmia r0, {s8-s10}\n"
        "bx lr\n"
        ".L14faa0_0:\n"
        ".word 0x00000000\n"
        ".L14faa0_3f800000:\n"
        ".word 0x3f800000\n"
    );
}

// 0x0014FAE8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::Vector3CalcCtr<float>::cross(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "vldmia r2, {s3-s5}\n"
        "vmul.f32 s6, s1, s5\n"
        "vmul.f32 s7, s2, s3\n"
        "vmul.f32 s8, s0, s4\n"
        "vmls.f32 s6, s2, s4\n"
        "vmls.f32 s7, s0, s5\n"
        "vmls.f32 s8, s1, s3\n"
        "vstmia r0, {s6-s8}\n"
        "bx lr\n"
    );
}

// 0x0014FB10 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
void sead::MathCtr<float>::sinCosIdx(float*, float*, u32)
{
    asm volatile(
        "bic r3, r2, #-16777216\n"
        "vmov s4, r3\n"
        "lsr r3, r2, #24\n"
        "ldr r2, .L14fb10_8c9af4\n"
        "vcvt.f32.u32 s4, s4\n"
        "add r2, r2, r3, lsl #4\n"
        "vldr s5, .L14fb10_33800000\n"
        "vmul.f32 s6, s4, s5\n"
        "vldmia r2, {s0-s3}\n"
        "vmla.f32 s0, s1, s6\n"
        "vmla.f32 s2, s3, s6\n"
        "vstr s0, [r0]\n"
        "vstr s2, [r1]\n"
        "bx lr\n"
        ".L14fb10_8c9af4:\n"
        ".word _ZN4sead17s_MathSinCosTableE\n"
        ".L14fb10_33800000:\n"
        ".word 0x33800000\n"
    );
}

// 0x0014FB50 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template <>
float sead::MathCtr<float>::tanIdx(u32)
{
    asm volatile(
        "bic r1, r0, #-16777216\n"
        "vmov s4, r1\n"
        "lsr r1, r0, #24\n"
        "ldr r0, .L14fb50_8c9af4\n"
        "vcvt.f32.u32 s4, s4\n"
        "add r0, r0, r1, lsl #4\n"
        "vldr s5, .L14fb50_33800000\n"
        "vmul.f32 s6, s4, s5\n"
        "vldmia r0, {s0-s3}\n"
        "vmla.f32 s0, s1, s6\n"
        "vmla.f32 s2, s3, s6\n"
        "vdiv.f32 s0, s0, s2\n"
        "bx lr\n"
        ".L14fb50_8c9af4:\n"
        ".word _ZN4sead17s_MathSinCosTableE\n"
        ".L14fb50_33800000:\n"
        ".word 0x33800000\n"
    );
}

} // namespace sead
