#pragma once

#include "pch.h"

#include "types/types.h"

#include "CHEL/eventloop/eventloop.h"

namespace JS {
    class Runtime {
    public:
        explicit Runtime(const std::string& name, int memoryLimit);
        ~Runtime();
        
        /**
         * @brief run JavaScript code in the main context
         * 
         * @param script the script you want to run
         * 
         * @return script evaluation value
         */
        JS::Value Run(const std::string& name, const std::string& script);

        /**
         * @brief run JavaScript code in an isolated temporary context
         * 
         * @param context the object that will be used as the top level context template.
         * After evaluation the top level context will be written to this object.
         * 
         * @param inheritModules should registered modules be available within this context
         * @param script the script you want to run
         * @param name the name of the script to run
         * 
         * @return script evaluation
         */
        JS::Value RunContext(JS::Value context, bool inheritModules, const std::string& name, const std::string& script);

        /**
         * @brief like the Run method but in an isolated temporary context
         * 
         * @param name the name of the script
         * @param script the script source code
         * 
         * @return script evaluation
         */
        JS::Value RunContext(const std::string& name, const std::string& script);

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

        /**
         * @brief Set the function to be called on exceptions
         * 
         * @param callback the function to execute
         */
        void SetErrorCallback(std::function<void(JS::Object)> callback);
    protected:
        JS::Value RunBasic(const std::string& name, const std::string& script);

        JsRuntimeHandle runtime = nullptr;
        JsContextRef context = nullptr;

        JS::Object modules = JS::Object(JS_INVALID_REFERENCE);
        JS::EventLoop eventLoop;

        std::function<void(JS::Object)> errorCallback = nullptr;

        // Register native functions into the runtime
        void RegisterBindings();

        friend class Bindings;
    };
}