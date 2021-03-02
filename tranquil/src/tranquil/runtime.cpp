#include "runtime.h"

namespace tranquil {
    thread_local bool Runtime::ACTIVE_THREAD = false;

    Runtime::Runtime(int memoryLimit, Context& context) : context(context) {
        if (Runtime::ACTIVE_THREAD)
            throw FatalRuntimeException("You may not create two runtime instances on the same thread");

        Runtime::ACTIVE_THREAD = true;

        if (JsCreateRuntime(JsRuntimeAttributeDisableFatalOnOOM, nullptr, &this->runtime) != JsNoError)
            throw FatalRuntimeException("There was a problem initializing the runtime");

        JsSetRuntimeMemoryLimit(this->runtime, memoryLimit * 1048576);

        if (context.context != nullptr)
            throw FatalRuntimeException("A previously initialized context for a different runtime was provided. This is not allowed");

        if (JsCreateContext(this->runtime, &this->context.context) != JsNoError)
            throw FatalRuntimeException("There was a problem initializing the context");

        if (JsSetCurrentContext(this->context.context) != JsNoError)
            throw FatalRuntimeException("There was a problem setting the context");

        // Setup native function binds from JS
        Native::Init(context);

        // Initialize object members
        this->modules = tranquil::Value::Object();

        // Register bindings from C++ to JS
        this->RegisterBindings();

        this->eventLoop.InitPromiseCallback();
    }

    void Runtime::SetContext(Context& context) {
        // Flag to set if value is being initialized for the first time
        bool postInit = false;

        // Check if context has already been initialized
        // If not initialize the context for the current runtime
        if (context.context != nullptr) {
            if (JsCreateContext(this->runtime, &context.context) != JsNoError)
                throw FatalRuntimeException("There was a problem initializing the context");
            postInit = true;
        }
        if (JsSetCurrentContext(this->context.context) != JsNoError)
            throw FatalRuntimeException("There was a problem setting the context");
        if (postInit)
            this->RegisterBindings();
        this->context = context;
    }

    tranquil::Value Runtime::Run(const std::string& name, const std::string& script) {
        // Inherit all requires of the require object
        tranquil::Value global = tranquil::Value::GetGlobalObject();
        tranquil::Native::ObjectAssign(global, this->modules);

        return this->RunBasic(name, script);
    }

    tranquil::Value Runtime::RunBasic(const std::string& name, const std::string& script) {
        JsValueRef result;

        bool error = false;

        if (JsRun(tranquil::Value(script), context.contextNumber++, tranquil::Value(name), JsParseScriptAttributeNone, &result) != JsNoError) {
            // Get exception object
            JsValueRef jsException;
            if (JsGetAndClearException(&jsException) != JsNoError)
                throw FatalRuntimeException("There was a problem while getting exception");
            
            if (this->errorCallback != nullptr) {
                this->errorCallback(tranquil::Value(jsException));
            }

            error = true;
        }

        if (this->errorCallback != nullptr) {
            this->eventLoop.SetErrorCallback(this->errorCallback);
        }

        // Resolve all promises and continuation callbacks
        this->eventLoop.Loop();
        
        // Return undefined if the result is empty or invalid
        if (result == JS_INVALID_REFERENCE || error)
            return tranquil::Value();

        return tranquil::Value(result);
    }

    Runtime::~Runtime() {
        Runtime::ACTIVE_THREAD = false;
        if (JsSetCurrentContext(JS_INVALID_REFERENCE) != JsNoError)
            throw FatalRuntimeException("There was a unsetting the context");
        if (JsDisposeRuntime(this->runtime) != JsNoError)
            throw FatalRuntimeException("There was a problem disposing of the runtime");
    }

    void Runtime::ThrowException(const char *error) {
        tranquil::Value value(error);
        JsValueRef errorObject;
        if (JsCreateError(value, &errorObject) != JsNoError) return;
        if (JsSetException(errorObject) != JsNoError) return;
    }

    void Runtime::ThrowException(JsValueRef error) {
        if (JsSetException(error) != JsNoError) return;
    }

    void Runtime::SetErrorCallback(std::function<void(tranquil::Value)> callback) {
        this->errorCallback = callback;
    }

    void Runtime::RegisterBindings() {
        tranquil::Value global = tranquil::Value::GetGlobalObject();

        // setTimeout
        tranquil::Value setTimeoutFunction(ADAPT_METHOD(EventLoop, &EventLoop::SetTimeout), &this->eventLoop);
        global.SetProperty("setTimeout", setTimeoutFunction);

        // setInterval
        tranquil::Value setIntervalFunction(ADAPT_METHOD(EventLoop, &EventLoop::SetInterval), &this->eventLoop);
        global.SetProperty("setInterval", setIntervalFunction);

        // clearInterval/clearTimeout
        tranquil::Value clearTimeoutFunction(ADAPT_METHOD(EventLoop, &EventLoop::ClearTimeout), &this->eventLoop);
        global.SetProperty("clearInterval", clearTimeoutFunction);
        global.SetProperty("clearTimeout", clearTimeoutFunction);
    }
}