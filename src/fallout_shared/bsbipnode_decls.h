// fallout_shared/bsbipnode_decls.h: AnimationInterpolatorNode declarations
#ifndef FNV_BSBIPNODE_DECLS_H
#define FNV_BSBIPNODE_DECLS_H

#include "../../include/NiBlendInterpolator.h"
#include "../../include/NiBlendTransformInterpolator.h"
#include "../../include/NiInterpController.h"
#include "../../include/NiInterpolator.h"
#include "../../include/NiMultiTargetTransformController.h"
#include "../../include/NiQuatTransform.h"
#include "../../include/NiQuaternion.h"
#include "../../include/NiTimeController.h"

class AnimationInterpolatorNode : public NiMultiTargetTransformController
{
  public:
    bool NiTimeController_GetActive();
    void BSTransCon_UpdateAllButTarget(float afTime, bool abSkipSelectiveUpdateTest);
};

typedef char AnimationInterpolatorNode_size_check[1];
typedef char AnimationInterpolatorNode_blend_offset_check[1];
typedef char AnimationInterpolatorNode_targets_offset_check[1];
typedef char AnimationInterpolatorNode_count_offset_check[1];

#endif
