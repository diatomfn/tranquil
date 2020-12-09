#pragma once

#include "message.h"
#include "logtype.h"

namespace JS::Output {
    class Log {
    public:
        void Push(std::string message, LogType logType, int line = 0, int col = 0);

        void SetOutputCallback(std::function<void(Output::Message)>);

        json ToJson();
    private:
        std::vector<Message> messages;
        
        std::function<void(Output::Message)> logCallback = nullptr;
    };
}