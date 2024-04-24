#include "engine/core/Camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

Camera::Camera() : Camera(glm::vec2()) {}

Camera::Camera(glm::vec2 position) {
    this->position = position;
    AdjustProjection();
}

void Camera::AdjustProjection() {
    // 创建正射投影矩阵，定义了一个类似立方体的平截头箱，它定义了一个裁剪空间，在这空间之外的顶点都会被裁剪掉
    // 会将处于这些x，y，z值范围内的坐标变换为标准化设备坐标(NDC)，其余的裁剪掉。
    projectionMatrix =
        glm::ortho(0.0f, 32.f * 40.f, 0.f, 32.f * 21.f, 0.f, 100.f);
}

glm::mat4 Camera::GetViewMatrix() {
    // lookAt三个参数：位置、目标、上轴
    viewMatrix = glm::lookAt(glm::vec3(position, 20.f),
                             glm::vec3(position, 0.f) + cameraFront, cameraUp);
    return viewMatrix;
}

glm::mat4 Camera::GetProjMatrix() {
    return projectionMatrix;
}