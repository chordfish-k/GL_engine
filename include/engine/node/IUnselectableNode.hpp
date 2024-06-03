#pragma once

#include <rttr/registration>
#include "engine/util/ReflectUtil.hpp"

class IUnselectableNode {
    RTTR_ENABLE()
};

BEGIN_RTTR_REG(IUnselectableNode)
rttr::registration::class_<IUnselectableNode>("IUnselectableNode");
END_RTTR_REG(IUnselectableNode)
