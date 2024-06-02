#pragma once
#include <box2d/b2_draw.h>
#include "engine/renderer/Box2dDebugDraw.hpp"
#include "engine/renderer/DebugDraw.hpp"
#include "engine/util/Setting.hpp"

class Box2DDebugDraw : public b2Draw {
public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        for (int i = 0; i < vertexCount; ++i) {
            int j = (i + 1) % vertexCount;
            DebugDraw::AddLine2D(glm::vec2(vertices[i].x, vertices[i].y) * Setting::PHYSICS_SCALE,
                                 glm::vec2(vertices[j].x, vertices[j].y) * Setting::PHYSICS_SCALE,
                                 glm::vec4(color.r, color.g, color.b, 1.0f));
        }
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        // Draw the outline
        DrawPolygon(vertices, vertexCount, color);

        // Fill the polygon (optional, could add more complex fill here)
        glm::vec4 fillColor = glm::vec4(color.r, color.g, color.b, 0.5f);
        for (int i = 0; i < vertexCount - 2; ++i) {
            DebugDraw::AddLine2D(glm::vec2(vertices[0].x, vertices[0].y) * Setting::PHYSICS_SCALE,
                                 glm::vec2(vertices[i + 1].x, vertices[i + 1].y) * Setting::PHYSICS_SCALE,
                                 fillColor);
            DebugDraw::AddLine2D(glm::vec2(vertices[i + 1].x, vertices[i + 1].y) * Setting::PHYSICS_SCALE,
                                 glm::vec2(vertices[i + 2].x, vertices[i + 2].y) * Setting::PHYSICS_SCALE,
                                 fillColor);
        }
    }

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override {
        DebugDraw::AddCircle(glm::vec2(center.x, center.y) * Setting::PHYSICS_SCALE, radius, glm::vec4(color.r, color.g, color.b, 1.0f));
    }

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {
        DrawCircle(center, radius, color);

        // Draw the axis
        b2Vec2 endPoint = center + radius * axis;
        DebugDraw::AddLine2D(glm::vec2(center.x, center.y) * Setting::PHYSICS_SCALE, glm::vec2(endPoint.x, endPoint.y) * Setting::PHYSICS_SCALE, glm::vec4(color.r, color.g, color.b, 1.0f));
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {
        DebugDraw::AddLine2D(glm::vec2(p1.x, p1.y) * Setting::PHYSICS_SCALE, glm::vec2(p2.x, p2.y) * Setting::PHYSICS_SCALE, glm::vec4(color.r, color.g, color.b, 1.0f));
    }

    void DrawTransform(const b2Transform& xf) override {
        float axisScale = 0.4f;
        b2Vec2 p1 = xf.p, p2;

        // X axis
        p2 = p1 + axisScale * xf.q.GetXAxis();
        DebugDraw::AddLine2D(glm::vec2(p1.x, p1.y) * Setting::PHYSICS_SCALE, glm::vec2(p2.x, p2.y) * Setting::PHYSICS_SCALE, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        // Y axis
        p2 = p1 + axisScale * xf.q.GetYAxis();
        DebugDraw::AddLine2D(glm::vec2(p1.x, p1.y) * Setting::PHYSICS_SCALE, glm::vec2(p2.x, p2.y) * Setting::PHYSICS_SCALE, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    }

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override {
        // Implement if needed
    }
};
