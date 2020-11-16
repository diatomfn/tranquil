#pragma once

#include "CHEL/pch.h"

#include "CHEL/FatalRuntimeException.h"

namespace JS {
    class Task {
    public:
        Task(JsValueRef func, int delay, JsValueRef thisArg, JsValueRef extraArgs, JsValueRef repeat);
        JsValueRef Invoke();
        ~Task();

        JsValueRef func;
        JsValueRef args[2];
        int argCount = 1;
        int delay;
        JsValueRef repeat;
        int time;
    };
}