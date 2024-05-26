#pragma once
#include <string>

class Setting {
public:
    static int WINDOW_W;
    static int WINDOW_H;
    static int GAME_VIEW_BUFFER_W;
    static int GAME_VIEW_BUFFER_H;
    static float GAME_VIEW_ASPECT;
    static std::string PROJECT_ROOT;

    static float PHYSICS_SCALE;
    static float PHYSICS_SCALE_INV;

public:
    static void Save();
    static void Load();
};
