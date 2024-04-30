#pragma once

#include <rttr/registration>
#include "engine/component/Transform.hpp"
#include "engine/component/SpriteRenderer.hpp"

RTTR_REGISTRATION  {
    rttr::registration::class_<Transform>("Transform")
        .property("position", &Transform::GetPosition, &Transform::SetPosition)
        .property("scale", &Transform::GetScale, &Transform::SetScale)
        .property("rotaion", &Transform::GetRotation, &Transform::SetRotation);

    rttr::registration::class_<SpriteRenderer>("SpriteRenderer")
        .property("color", &SpriteRenderer::GetColor, &SpriteRenderer::SetColor)
        .property("offset", &SpriteRenderer::GetOffset, &SpriteRenderer::SetOffset);
}
