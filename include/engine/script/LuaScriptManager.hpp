#pragma once

#include <sol/state.hpp>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include "Script.hpp"
#include "engine/util/Print.hpp"
#include "engine/node/Node.hpp"

class LuaScriptManager : public b2ContactListener{
public:
    LuaScriptManager();
    void AddScriptNode(Node *node);

    virtual void OnAttached(Node *node = nullptr);
    virtual void OnGameStart(Node *node = nullptr);
    virtual void OnGameUpdate(float dt);
    virtual void OnCollision(Node *a, Node *b);

    template<typename ...Args>
    void Call(Script &script, const std::string& fnName, Args&& ...args) {
        auto fn = script.self[fnName];
        if (!fn.valid())
            return;
        sol::function_result result = script.self[fnName].call(std::forward<Args>(args)...);
        if(!result.valid())
        {
            sol::error err = result;
            util::Println(err.what());
        }
    }
    void DestroyNode(Node *pNode);

    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    void PostSolve(b2Contact *contact,
                   const b2ContactImpulse *impulse) override;

private:
    sol::state state;
    std::vector<Node*> scripts;
};
