#pragma once

#include <string>
#include "engine/util/Json.hpp"

using json = nlohmann::json;

class ASerializableObj {
public:
    virtual std::string Serialize() {return "";};
    virtual ASerializableObj *Deserialize(json j) {return nullptr;} ;
};

static json Str2Json(std::string str) {
    json j;
    try{
        j = json::parse(str);
    }catch (std::exception &e) {
        assert(false && e.what());
    }
    return j;
}
