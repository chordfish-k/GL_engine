#pragma once

#include <rttr/registration>
#include "engine/core/Transform.hpp"
#include "engine/node/SpriteRenderer.hpp"

RTTR_REGISTRATION  {

//    rttr::registration::class_<AGuiObj>("AGuiObj");

    rttr::registration::class_<Node>("Node")
        .constructor<>()(
            rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )
        .method("Deserialize", &Node::Deserialize)
        .method("Imgui", &Node::Imgui);

    rttr::registration::class_<SpriteRenderer>("SpriteRenderer")
        .constructor<>()(
                rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
            )
        .property("centered", &SpriteRenderer::IsCentered, &SpriteRenderer::SetCentered)
        .property("color", &SpriteRenderer::GetColor, &SpriteRenderer::SetColor)
        .property("offset", &SpriteRenderer::GetOffset, &SpriteRenderer::SetOffset)
        .property("sprite", &SpriteRenderer::GetSprite, &SpriteRenderer::SetSprite);
}
