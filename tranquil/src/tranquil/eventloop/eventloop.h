#pragma once

#include "tranquil/pch.h"

#include "tranquil/value.h"
#include "tranquil/eventloop/task.h"

namespace tranquil {
    // Declare to prevent include loop
    class Runtime;

    class EventLoop {
    public:
        void Loop();

        /**
         * @brief Add a task to the eventloop
         * 
         * @param task to add
         */
        void AddTask(Task* task);
    private:
        void SetErrorCallback(std::function<void(tranquil::Value)> callback);

        void InitPromiseCallback();
        static void PromiseCallback(JsValueRef task, void* callbackState);

        void HandleException(JsValueRef except);

        std::queue<Task*> taskQueue;

        std::function<void(tranquil::Value)> errorCallback = nullptr;

        JsValueRef SetTimeout(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args);
        JsValueRef SetInterval(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args);
        JsValueRef ClearTimeout(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args);

        friend class Runtime;
        friend class Bindings;
    };
}