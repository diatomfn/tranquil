#include "task.h"

namespace JS {
    Task::Task(JsValueRef func, int delay, JsValueRef thisArg, JsValueRef extraArgs, bool repeat) : func(func), delay(delay), repeat(repeat) {
        this->args[0] = thisArg;
        this->args[1] = extraArgs;
        this->time = (int)(clock() / (double)(CLOCKS_PER_SEC / 1000));

        JsAddRef(this->func, nullptr);
        JsAddRef(this->args[0], nullptr);
        if (extraArgs != JS_INVALID_REFERENCE) {
            JsAddRef(this->args[1], nullptr);
            argCount = 2;
        }
    }

    JsValueRef Task::Invoke() {
        JsValueRef ret;
        if (JsCallFunction(this->func, this->args, this->argCount, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }

    Task::~Task() {
        JsRelease(this->func, nullptr);
        JsRelease(this->args[0], nullptr);
        if (this->args[1] != JS_INVALID_REFERENCE) {
            JsRelease(this->args[1], nullptr);
        }
    }
}