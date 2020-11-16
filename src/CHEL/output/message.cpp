#include "message.h"

namespace JS::Output {
    Message::Message(std::string& message, LogType type, int line) {
        this->message = message;
        this->type = type;
        this->line = line;
    }
}