#pragma once

class Angular {
public:
    float velocity = 0;
    float damp = 0.8f;
public:
    Angular(){};
    Angular(float velocity, float damp);
    bool Imgui();
};
