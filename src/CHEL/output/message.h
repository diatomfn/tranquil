#pragma once

#include "CHEL/pch.h"

#include "logtype.h"

namespace JS::Output {
    class Message {
    public:
        Message(const std::string& message, LogType type, int line, int col);

        std::string message;
        LogType type;
        int line;
        int col;
    };
}