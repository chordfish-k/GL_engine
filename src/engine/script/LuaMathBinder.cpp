#include "engine/script/LuaMathBinder.hpp"
#include <glm/ext.hpp>

void LuaMathBinder::Bind(sol::state &state) {
    state.new_usertype<glm::vec2>("Vec2", sol::call_constructor,
                                  sol::constructors<glm::vec2>(),
                                  "x", &glm::vec2::x,
                                  "y", &glm::vec2::y
    );

    state.new_usertype<glm::vec3>("Vec3", sol::call_constructor,
                                  sol::constructors<glm::vec3>(),
                                  "x", &glm::vec3::x,
                                  "y", &glm::vec3::y,
                                  "z", &glm::vec3::z
    );

    state.new_usertype<glm::vec4>("Vec4", sol::call_constructor,
                                  sol::constructors<glm::vec4>(),
                                  "x", &glm::vec4::x,
                                  "y", &glm::vec4::y,
                                  "z", &glm::vec4::z,
                                  "w", &glm::vec4::w
    );
}