#pragma once

#include "CHEL/pch.h"

#include "CHEL/bindings.h"
#include "task.h"

#include "CHEL/types/types.h"
#include "CHEL/output/log.h"

namespace JS {
    // Declare to prevent include loop
    class Runtime;

    class EventLoop {
    public:
        void SetCallback();
        void Loop();
        void SetLog(Output::Log* log);
    private:
        static void PromiseCallback(JsValueRef task, void* callbackState);

        void HandleException(JsValueRef except);

        std::queue<Task*> taskQueue;

        // Needed for exception logging within tasks
        Output::Log* outputLog;

        friend class Runtime;
        friend class Bindings;
    };
}