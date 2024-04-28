#pragma once

#include <rttr/registration>
#include "engine/component/Transform.hpp"
#include "engine/component/SpriteRenderer.hpp"

RTTR_REGISTRATION  {
    rttr::registration::class_<Transform>("Transform")
        .property("position", &Transform::position)
        .property("scale", &Transform::scale);

    rttr::registration::class_<SpriteRenderer>("SpriteRenderer")
        .property("color", &SpriteRenderer::GetColor, &SpriteRenderer::SetColor);
}
