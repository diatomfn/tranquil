#include "task.h"

namespace JS {
    Task::Task(JsValueRef timeout, JsValueRef thisArg, JsValueRef extraArgs, bool repeat) : delay(delay), repeat(repeat), timeout(timeout) {
        this->args[0] = thisArg;
        this->args[1] = extraArgs;
        this->time = (int)(clock() / (double)(CLOCKS_PER_SEC / 1000));

        JsAddRef(this->timeout, nullptr);
        JsAddRef(this->args[0], nullptr);
        if (extraArgs != JS_INVALID_REFERENCE) {
            JsAddRef(this->args[1], nullptr);
            argCount = 2;
        }
    }

    JsValueRef Task::Invoke() {
        JsValueRef ret;
        JsCallFunction(JS::Timeout(this->timeout)._onTimeout, this->args, this->argCount, &ret);
        return ret;
    }

    Task::~Task() {
        JsRelease(this->timeout, nullptr);
        JsRelease(this->args[0], nullptr);
        if (this->args[1] != JS_INVALID_REFERENCE) {
            JsRelease(this->args[1], nullptr);
        }
    }
}