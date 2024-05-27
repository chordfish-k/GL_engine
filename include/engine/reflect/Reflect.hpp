#pragma once

#include <rttr/registration>
#include "engine/node/IUnselectableNode.hpp"

RTTR_REGISTRATION  {

    rttr::registration::class_<Transform>("Transform")
        .constructor<>()
        .property("position", &Transform::position)
        .property("scale", &Transform::scale)
        .property("rotation", &Transform::rotation);

    rttr::registration::class_<Linear>("Linear")
        .constructor<>()
        .property("velocity", &Linear::velocity)
        .property("damp", &Linear::damp);

    rttr::registration::class_<Animation>("Animation")
        .constructor<>()
        .property("hFrames", &Animation::GetHFrames, &Animation::SetHFrames)
        .property("vFrames", &Animation::GetVFrames, &Animation::SetVFrames)
        .property("frame", &Animation::GetFrame, &Animation::SetFrame);

    rttr::registration::class_<IUnselectableNode>("IUnselectableNode");
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

    rttr::registration::class_<TileMap>("TileMap")
        .constructor<>()(
            rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
            )
        .property("cellWidth", &TileMap::GetCellWidth, &TileMap::SetCellWidth)
        .property("cellHeight", &TileMap::GetCellHeight, &TileMap::SetCellHeight);

    rttr::registration::enumeration<BodyType>("BodyType")(
        rttr::value("Kinematic", BodyType::Kinematic),
        rttr::value("Dynamic", BodyType::Dynamic),
        rttr::value("Static", BodyType::Static)
    );

    rttr::registration::class_<RigidBody2D>("RigidBody2D")
        .constructor<>()(
            rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
            )
        .property("mass", &RigidBody2D::GetMass, &RigidBody2D::SetMass)
        .property("body type", &RigidBody2D::GetBodyType, &RigidBody2D::SetBodyType)
        .property("fixed rotation", &RigidBody2D::IsFixedRotation, &RigidBody2D::SetFixedRotation)
        .property("continuous", &RigidBody2D::IsContinuousCollision, &RigidBody2D::SetContinuousCollision);

    rttr::registration::class_<ColliderShape2D>("ColliderShape2D")
        .constructor<>()(
            rttr::policy::ctor::as_raw_ptr // 使用 new 创建对象
            )
        ;
}
