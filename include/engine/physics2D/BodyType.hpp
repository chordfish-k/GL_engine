#pragma once

#include "engine/util/ReflectUtil.hpp"

enum class BodyType {
    Static = 0,
    Dynamic,
    Kinematic
};

inline std::string GetNameByBodyType(BodyType type) {
    switch (type) {

    case BodyType::Static:
        return "Static";
    case BodyType::Dynamic:
        return "Dynamic";
    case BodyType::Kinematic:
        return "Kinematic";
    }
    return "Dynamic";
}

inline BodyType GetBodyTypeByName(const std::string &type) {
    if (type == "Static") return BodyType::Static;
    if (type == "Dynamic") return BodyType::Dynamic;
    if (type == "Kinematic") return BodyType::Kinematic;
    return BodyType::Dynamic;
}

BEGIN_RTTR_REG(BodyType)
rttr::registration::enumeration<BodyType>("BodyType")(
    rttr::value("Kinematic", BodyType::Kinematic),
    rttr::value("Dynamic", BodyType::Dynamic),
    rttr::value("Static", BodyType::Static)
);
END_RTTR_REG(BodyType)