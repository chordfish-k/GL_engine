#include "engine/event/EventSystem.hpp"

std::vector<IObserver*> EventSystem::observers;

void EventSystem::AddObserver(IObserver *observer) {
    observers.push_back(observer);
}

void EventSystem::Notify(Node *node, Event event) {
    for (auto obs : observers) {
        obs->Notify(node, event);
    }
}