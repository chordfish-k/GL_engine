#include "engine/core/MainWindow.hpp"
#include "engine/script/LuaScriptLayer.hpp"
#include <sol/sol.hpp>

//class GameObject {
//private:
//    std::string name;
//public:
//    GameObject(const std::string&name_) {
//        name = name_;
//    }
//    void move(){};
//    std::string getName(){return name;}
//    int getX(){return 1;}
//    int getY(){return 2;}
//};

int main() {
    MainWindow *window = MainWindow::Get();
    window->SetDefaultScenePath("test/scene/a.scene");
    window->Run();
//    LuaScriptLayer::Get()->GetLua()->new_usertype<GameObject>("GameObject",
//                                 sol::constructors<GameObject(const std::string&)>(),
//                                 "move", &GameObject::move,
//                                 "getName", &GameObject::getName,
//                                 "getX", &GameObject::getX,
//                                 "getY", &GameObject::getY
//    );
//
//
//    Script script("assets/script/test.lua");
//    script.Execute();
    return 0;
}