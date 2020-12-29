#pragma once

#include "CHEL/pch.h"

#include "CHEL/bindings.h"
#include "task.h"

#include "CHEL/types/types.h"

namespace JS {
    // Declare to prevent include loop
    class Runtime;

    class EventLoop {
    public:
        void Loop();

        /**
         * @brief Set the function to be called on exceptions
         * 
         * @param callback the function to execute
         */
        void SetErrorCallback(std::function<void(JS::Value)> callback);
    private:
        void InitPromiseCallback();
        static void PromiseCallback(JsValueRef task, void* callbackState);

        void HandleException(JsValueRef except);

        std::queue<Task*> taskQueue;

        std::function<void(JS::Value)> errorCallback = nullptr;

        friend class Runtime;
        friend class Bindings;
    };
}