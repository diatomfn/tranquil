#pragma once

#include "message.h"
#include "logtype.h"

namespace JS::Output {
    class Log {
        std::vector<Message> messages;

    public:
        void Push (std::string message, LogType logType, int line = 0);
        json ToJson();
    };
}