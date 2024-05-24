#pragma once

enum EventType {

    SaveScene,
    CloseProject,
    LoadScene,
    CloseTopWindow,
    GameEngineStartPlay,
    GameEngineStopPlay
};

struct Event {
    EventType type;
};
