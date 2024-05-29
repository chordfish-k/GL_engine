#pragma once

#include <string>
#include "engine/node/Node.hpp"

class Prefab {
public:
    std::string filePath;

public:
    Prefab(std::string_view filePath);
    Node *Instance() const;
};
