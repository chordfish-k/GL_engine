#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
    glm::mat4 projectionMatrix, viewMatrix, invProjection, invView;
    float projectWidth = 40 / 7.f;
    float projectHeight = 21 / 7.f;
    float zoom = 0.5f;
    glm::vec2 projectionSize = {32.0f * 40.0f, 32.0f * 21.0f};
    // 定义摄像机朝向、上轴
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    glm::vec2 position;

public:
    Camera();

    Camera(glm::vec2 position);

    void AdjustProjection();

    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjMatrix();

    glm::mat4 &GetInvProjection();

    glm::mat4 &GetInvView();

    glm::vec2 GetProjectionSize(){
        return projectionSize;
    }

    float GetZoom() {return zoom;}

    void SetZoom(float zoom) {this->zoom = zoom;}

    void AddZoom(float value) {this->zoom += value;}
};