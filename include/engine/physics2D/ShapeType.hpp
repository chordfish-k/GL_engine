#pragma once

#include <string>
#include "engine/util/ReflectUtil.hpp"

enum class ShapeType {
    Box2DCollider = 0,
    CircleCollider
};

inline std::string GetNameByShapeType(ShapeType type) {
    switch (type) {

    case ShapeType::Box2DCollider:
        return "Box2DCollider";
    case ShapeType::CircleCollider:
        return "CircleCollider";
    }
    return "Rectangle";
}

inline ShapeType GetShapeTypeByName(const std::string &type) {
    if (type == "Box2DCollider") return ShapeType::Box2DCollider;
    if (type == "CircleCollider") return ShapeType::CircleCollider;
    return ShapeType::Box2DCollider;
}

BEGIN_RTTR_REG(ShapeType)
rttr::registration::enumeration<ShapeType>("ShapeType")(
    rttr::value("Box2DCollider", ShapeType::Box2DCollider),
    rttr::value("CircleCollider", ShapeType::CircleCollider)
);
END_RTTR_REG(ShapeType)
