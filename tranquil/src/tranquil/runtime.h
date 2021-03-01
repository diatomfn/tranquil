#pragma once

#include "pch.h"

#include "value.h"
#include "context.h"
#include "native.h"

#include "FatalRuntimeException.h"
#include "tranquil/eventloop/eventloop.h"

namespace tranquil {
    /**
     * @brief Fatal runtime error
     */
    class FatalRuntimeException : public std::exception {
    private:
        std::string errorMessage;
    public:
        FatalRuntimeException() {
            this->errorMessage = "A fatal runtime error occured in the Javascript runtime";
        }
        FatalRuntimeException(const std::string& errorMessage) {
            this->errorMessage = errorMessage;
        }
        const char* what() const noexcept override {
            return this->errorMessage.c_str();
        }
    };

    /* 
     * Only one runtime must exist at one time per thread.
     * You may create more than one but undesired effects will occur
     */
    class Runtime {
    public:
        explicit Runtime(int memoryLimit, Context& context);
        ~Runtime();

        /**
         * @brief run JavaScript code in the main context
         * 
         * @param script the script you want to run
         * 
         * @return script evaluation value
         */
        tranquil::Value Run(const std::string &name, const std::string &script);

        /**
         * @brief Set the current active context.
         * Context provided must be initialized by this runtime or have not been initialized by any runtime yet.
         * 
         * @param context to set
         */
        void SetContext(Context& context);

        /**
         * @brief throw an exception
         * 
         * @param error the exception string to throw
         */
        static void ThrowException(const char *error);

        
        /**
         * @brief throw an exception
         * 
         * @param error exception value to throw
         */
        static void ThrowException(JsValueRef error);

        /**
         * @brief Set the function to be called on exceptions
         * 
         * @param callback the function to execute
         */
        void SetErrorCallback(std::function<void(tranquil::Value)> callback);
    protected:
        tranquil::Value RunBasic(const std::string &name, const std::string &script);
        
        JsRuntimeHandle runtime = nullptr;
        Context& context;

        tranquil::Value modules = tranquil::Value(JS_INVALID_REFERENCE);
        tranquil::EventLoop eventLoop;

        std::function<void(tranquil::Value)> errorCallback = nullptr;

        // Register native functions into the global object
        void RegisterBindings();

        // Flag to make sure that two runtimes do not exist on the same thread
        // The runtime is thread based
        static thread_local bool ACTIVE_THREAD;

        friend class Bindings;
        friend class Native;
    };
}
