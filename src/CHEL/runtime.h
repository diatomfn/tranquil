#pragma once

#include "pch.h"

#include "module.h"
#include "types/types.h"

#include "CHEL/output/log.h"
#include "CHEL/eventloop/eventloop.h"

namespace JS {
    class Runtime {
    public:
        explicit Runtime(int memoryLimit);

        ~Runtime();
        JS::Output::Log Run(const std::string& script);
        void Register(const char* name, JS::Value value, bool isGlobal);
        Output::Log logOutput;

        static void ThrowException(const char* error);
    private:
        JsRuntimeHandle runtime = nullptr;
        JsContextRef context = nullptr;

        std::unordered_map<std::string, JsValueRef> requireMap;
        JS::EventLoop eventLoop;

        // Register native functions into the runtime
        void RegisterBindings();

        friend class Bindings;
    };
}