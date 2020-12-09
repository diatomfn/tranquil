#include "message.h"

namespace JS::Output {
    Message::Message(const std::string& message, LogType type, int line, int col) {
        this->message = message;
        this->type = type;
        this->line = line;
        this->col = col;
    }
}