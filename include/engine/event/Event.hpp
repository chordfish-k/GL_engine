#pragma once

enum EventType {

    SaveScene,
    CloseProject,
    LoadScene,
    CloseTopWindow,
    GameEngineStartPlay,
    GameEngineStopPlay,
    OnCollision
};

struct Event {
    EventType type;
    void *data;
};
