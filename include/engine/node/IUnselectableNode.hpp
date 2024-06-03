#pragma once

#include <rttr/registration>
#include "engine/util/ReflectUtil.hpp"

class IUnselectableNode {
    RTTR_ENABLE()
};

BEGIN_RTTR_REG(IUnselectableNode)
RTTR_CLASS(IUnselectableNode);
END_RTTR_REG(IUnselectableNode)
