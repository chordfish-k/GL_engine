#pragma once

#include "engine/event/Event.hpp"
#include "engine/node/Node.hpp"

class IObserver {
public:
    virtual void Notify(Node *node, Event event) = 0;
};
