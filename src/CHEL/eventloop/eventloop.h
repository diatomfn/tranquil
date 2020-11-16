#pragma once

#include "CHEL/pch.h"

#include "CHEL/bindings.h"
#include "task.h"

#include "CHEL/boolean.h"

namespace JS {
    // Declare to prevent include loop
    class Runtime;

    class EventLoop {
    public:
        void SetCallback();
        void Loop();
    private:
        static void PromiseCallback(JsValueRef task, void* callbackState);

        std::queue<Task*> taskQueue;

        friend class Runtime;
        friend class Bindings;
    };
}