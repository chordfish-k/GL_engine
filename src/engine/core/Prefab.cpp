#include "engine/core/Prefab.hpp"
#include "engine/util/PrefabsUtils.hpp"
#include "engine/util/Common.hpp"

Prefab::Prefab(std::string_view filePath)
    :filePath(filePath) {

}

Node *Prefab::Instance() const {
    return PrefabsUtils::GenerateNodeFromJson(util::LoadJsonFromFile(filePath));
}
