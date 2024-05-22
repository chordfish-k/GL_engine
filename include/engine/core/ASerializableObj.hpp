#pragma once

#include <string>
#include "engine/util/Json.hpp"
#include "engine/util/Print.hpp"

using json = nlohmann::json;

class ASerializableObj {
public:
    virtual json Serialize() = 0;
    virtual ASerializableObj *Deserialize(json j) = 0;
};

static json Str2Json(std::string str) {
    json j;
    try{
        j = json::parse(str);
    }catch (std::exception &e) {
        util::Print(e.what());
        assert(false);
    }
    return j;
}
