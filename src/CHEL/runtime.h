#pragma once

#include "pch.h"

#include "types/types.h"

#include "CHEL/output/log.h"
#include "CHEL/eventloop/eventloop.h"

namespace JS {
    class Runtime {
    public:
        explicit Runtime(int memoryLimit);
        ~Runtime();
        
        /**
         * @brief run JavaScript code in the main context
         * 
         * @param script the script you want to run
         * 
         * @return script evaluation value
         */
        JS::Value Run(const std::string& script);

        /**
         * @brief run JavaScript code in an isolated temporary context
         * 
         * @param context the object that will be used as the top level context template.
         * After evaluation the top level context will be written to this object.
         * 
         * @param inheritModules should registered modules be available within this context
         * @param script the script you want to run
         * 
         * @return script evaluation value
         */
        JS::Value RunContext(JS::Value context, bool inheritModules, const std::string& script);

        /**
         * @brief register a value into the top level module object
         * 
         * @param name the name of the property internally
         * @param value the value of the property
         */
        void Register(const char* name, JS::Value value);

        /**
         * @brief throw an exception
         * 
         * @param error the exception string to throw
         */
        static void ThrowException(const char* error);

        Output::Log logOutput;
    private:
        JS::Value RunBasic(const std::string& scriptName, const std::string& script);

        JsRuntimeHandle runtime = nullptr;
        JsContextRef context = nullptr;

        JS::Object modules = JS::Object(JS_INVALID_REFERENCE);
        JS::EventLoop eventLoop;

        // Register native functions into the runtime
        void RegisterBindings();

        friend class Bindings;
    };
}