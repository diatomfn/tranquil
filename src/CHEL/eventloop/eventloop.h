#pragma once

#include "CHEL/pch.h"

#include "task.h"

namespace JS {
    class EventLoop {
    public:
        void SetCallback();
        void Loop();
    private:
        std::queue<Task*> taskQueue;
        static void PromiseCallback(JsValueRef task, void* callbackState);
    };
}