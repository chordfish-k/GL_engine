#pragma once

#include <rttr/registration>
#include "engine/core/Transform.hpp"
#include "engine/node/SpriteRenderer.hpp"

RTTR_REGISTRATION  {

    rttr::registration::class_<Node>("Node")
        .constructor<>()(
            rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
        )
        .property("zIndex", &SpriteRenderer::GetZIndex, &SpriteRenderer::SetZIndex)
        .method("Deserialize", &SpriteRenderer::Deserialize);

    rttr::registration::class_<SpriteRenderer>("SpriteRenderer")
        .constructor<>()(
                rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
            )
        .property("color", &SpriteRenderer::GetColor, &SpriteRenderer::SetColor)
        .property("offset", &SpriteRenderer::GetOffset, &SpriteRenderer::SetOffset);
//        .method("Deserialize", &SpriteRenderer::Deserialize);
}
