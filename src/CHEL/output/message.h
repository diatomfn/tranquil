#pragma once

#include "CHEL/pch.h"

#include "logtype.h"

namespace JS::Output {
    class Message {
    public:
        Message(std::string&, LogType, int);

        std::string message;
        LogType type;
        int line;
    };
}