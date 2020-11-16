#pragma once

#include "CHEL/pch.h"

#include "CHEL/FatalRuntimeException.h"
#include "timeout.h"

namespace JS {
    class Task {
    public:
        Task(JsValueRef timeout, JsValueRef thisArg, JsValueRef extraArgs, bool repeat = false);
        JsValueRef Invoke();
        ~Task();

        JsValueRef timeout;
        JsValueRef args[2];
        int argCount = 1;
        int delay;
        bool repeat;
        int time;
    };
}