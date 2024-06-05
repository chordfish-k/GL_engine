#include "engine/script/LuaBinder.hpp"
#include "engine/script/LuaGlobalBinder.hpp"
#include "engine/script/LuaMathBinder.hpp"
#include "engine/script/LuaNodeBinder.hpp"

void LuaBinder::BindAll(sol::state &state) {
    // 删除sol2的function_type_stateless.hpp中361/363行的noexcept(std::is_nothrow_copy_assignable_v<T>)
    // 否则new_usertype绑定成员变量会报错

    state.open_libraries(sol::lib::base,sol::lib::math,sol::lib::string);

    LuaGlobalBinder::Bind(state);
    LuaMathBinder::Bind(state);
    LuaNodeBinder::Bind(state);
}
