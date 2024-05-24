#pragma once

#include <vector>
#include "engine/event/IObserver.hpp"
#include "engine/event/Event.hpp"

class EventSystem {
private:
    static std::vector<IObserver*> observers;

public:
    static void AddObserver(IObserver *observer);

    static void Notify(Node *node, Event event);
};
