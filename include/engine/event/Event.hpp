#pragma once

enum EventType {

    SaveScene,
    CloseProject,
    LoadScene
};

struct Event {
    EventType type;
};
