#pragma once

enum EventType {

    SaveScene,
    CloseProject,
    LoadScene,
    CloseTopWindow
};

struct Event {
    EventType type;
};
