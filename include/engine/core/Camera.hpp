#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
    glm::mat4 projectionMatrix, viewMatrix;
    // 定义摄像机朝向、上轴
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    glm::vec2 position;

public:
    Camera(glm::vec2 position);

    void AdjustProjection();

    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjMatrix();
};