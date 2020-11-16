#include "logtype.h"

namespace JS::Output {
    const char* GetStringLogType(LogType log_type) {
        static const char * eString[] = { "log", "info", "error", "warn", "debug" };
        return eString[log_type];
    }
}