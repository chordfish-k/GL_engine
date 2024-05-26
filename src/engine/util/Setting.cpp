#include <fstream>
#include "engine/util/Setting.hpp"
#include "engine/util/Json.hpp"
#include "engine/util/Print.hpp"

using json = nlohmann::json;

std::string Setting::PROJECT_ROOT;
int Setting::WINDOW_W = 1920;
int Setting::WINDOW_H = 1080;
int Setting::GAME_VIEW_BUFFER_W = 1920 * 2;
int Setting::GAME_VIEW_BUFFER_H = 1080 * 2;
float Setting::GAME_VIEW_ASPECT = GAME_VIEW_BUFFER_W * 1.f / GAME_VIEW_BUFFER_H;

float Setting::PHYSICS_SCALE = 50;
float Setting::PHYSICS_SCALE_INV = 1.f/50;

void Setting::Save() {
    json j;
    j["WINDOW_SIZE"] = {WINDOW_W, WINDOW_H};
    j["GAME_VIEW_BUFFER_SIZE"] = {GAME_VIEW_BUFFER_W, GAME_VIEW_BUFFER_H};
    j["PROJECT_ROOT"] = PROJECT_ROOT;


    j["PHYSICS_SCALE"] = PHYSICS_SCALE;

    auto str = j.dump(1, '\t', true,
                      nlohmann::json::error_handler_t::replace);
    std::ofstream os("setting.json", std::ios::trunc);
    if (os.is_open()){
        os << str;
        os.close();
    } else {
        util::Println("ERROR:Saving Setting.json failed.");
    }
}

void Setting::Load() {
    std::ifstream fin("setting.json");
    std::string jsonText;
    if (fin.is_open()) {
        std::stringstream ss;
        ss << fin.rdbuf();
        jsonText = ss.str();
        if (jsonText.empty())
            return;
    }
    json j;
    if (jsonText.empty()) return;
    try{
        j = json::parse(jsonText);
    }catch (std::exception &e) {
        assert(false && e.what());
    }
    if (j.empty())
        return;

    auto &ws = j["WINDOW_SIZE"];
    if (!ws.empty()) {
        WINDOW_W = ws[0];
        WINDOW_H = ws[1];
    }

    auto &bs = j["GAME_VIEW_BUFFER_SIZE"];
    if (!bs.empty()) {
        GAME_VIEW_BUFFER_W = bs[0];
        GAME_VIEW_BUFFER_H = bs[1];
    }

    auto &pr = j["PROJECT_ROOT"];
    if (!pr.empty()) {
        PROJECT_ROOT = pr;
    }

    auto &ps = j["PHYSICS_SCALE"];
    if (!ps.empty()) {
        PHYSICS_SCALE = ps;
        PHYSICS_SCALE_INV = 1.f / PHYSICS_SCALE;
    }

}
