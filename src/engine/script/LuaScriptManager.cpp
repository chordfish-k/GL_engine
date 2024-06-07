#include <string>
#include <sol/sol.hpp>
#include "engine/script/LuaScriptManager.hpp"
#include "engine/script/LuaBinder.hpp"
#include "engine/util/Common.hpp"


LuaScriptManager::LuaScriptManager() {
    LuaBinder::BindAll(state);
}


void LuaScriptManager::OnAttached(Node *node) {
    if (node == nullptr) {
        for (auto node_ : scripts){
            Script &item = node_->script;
            if (item.filePath.empty() || item.attached) continue;
            auto result = state.safe_script_file(
                util::GetAbsolutePath(item.filePath),
                &sol::script_pass_on_error);
            if(result.valid())
            {
                if(result.return_count() == 1 && result[0].is<sol::table>())
                {
                    item.self = result[0];
                    item.self["this"] = node_;
                }
            }
            else {
                sol::error err = result;
                util::Println(err.what());
            }
            item.attached = true;
        }
    }
    else {
        Script &item = node->script;
        if (item.filePath.empty() || item.attached)
            return;
        auto result = state.safe_script_file(
            util::GetAbsolutePath(item.filePath),
            &sol::script_pass_on_error);
        if(result.valid())
        {
            if(result.return_count() == 1 && result[0].is<sol::table>())
            {
                item.self = result[0];
                item.self["this"] = node;
            }
        }
        else {
            sol::error err = result;
            util::Println(err.what());
        }
        item.attached = true;
    }
}

void LuaScriptManager::OnGameStart(Node *node) {
    if (node == nullptr) {
        for (auto node_ : scripts){
            Script &item = node_->script;
            if (item.filePath.empty()) continue;
            if (!item.attached) {
                OnAttached(node_);
            }
            Call(item, "Start");
        }
    } else {
        Script &item = node->script;
        if (item.filePath.empty())
            return;
        if (!item.attached) {
            OnAttached(node);
        }
        Call(item, "Start");
    }
}


void LuaScriptManager::OnGameUpdate(float dt) {
    for (int i = 0; i < scripts.size(); ++i){
        Script &item = scripts[i]->script;
        if (item.filePath.empty()) continue;
        if (!item.attached) {
            OnAttached(scripts[i]);
            OnGameStart(scripts[i]);
        } else {
            Call(item, "Update",
                 "dt", dt);
        }
    }
}

void LuaScriptManager::OnCollision(Node *a, Node *b) {
    if (!a->script.filePath.empty()) {
        Call(a->script, "OnCollision", "other", b);
    }
    if (!b->script.filePath.empty()) {
        Call(b->script, "OnCollision", "other", a);
    }
}

void LuaScriptManager::AddScriptNode(Node *node) {
    if (scripts.end() == std::find(scripts.begin(), scripts.end(), node)
        && !node->script.filePath.empty()) {
        scripts.push_back(node);
    }
}

void LuaScriptManager::DestroyNode(Node *node) {
    auto it = std::find(scripts.begin(), scripts.end(), node);
    if (it != scripts.end()) {
        scripts.erase(it);
    }
}

void LuaScriptManager::BeginContact(b2Contact *contact) {
    b2ContactListener::BeginContact(contact);
    auto A = (Node*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    auto B = (Node*) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    if (A != nullptr && B != nullptr) {
        OnCollision(A, B);
    }
}

void LuaScriptManager::EndContact(b2Contact *contact) {
    b2ContactListener::EndContact(contact);
}

void LuaScriptManager::PreSolve(b2Contact *contact,
                                const b2Manifold *oldManifold) {
    b2ContactListener::PreSolve(contact, oldManifold);
}

void LuaScriptManager::PostSolve(b2Contact *contact,
                                 const b2ContactImpulse *impulse) {
    b2ContactListener::PostSolve(contact, impulse);
}

/*
void LuaScriptManager::Notify(Node *node, Event event) {
    switch (event.type) {
    case EventType::GameEngineStartPlay:
        OnAttached(node);
        OnGameStart(node);
        break;
//    case EventType::GameEngineStopPlay:
//        OnGameUpdate();
//        break;
    default:
        break;
    }
}
*/