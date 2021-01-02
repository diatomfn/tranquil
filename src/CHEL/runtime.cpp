#include "runtime.h"

#include "FatalRuntimeException.h"

#include "types/types.h"
#include "native.h"
#include "common.h"

namespace JS {
    thread_local uint Runtime::contextNumber = 0;

    Runtime::Runtime(int memoryLimit) {
        if (JsCreateRuntime(JsRuntimeAttributeDisableFatalOnOOM, nullptr, &this->runtime) != JsNoError)
            throw FatalRuntimeException();

        JsSetRuntimeMemoryLimit(this->runtime, memoryLimit * 1048576);

        if (JsCreateContext(this->runtime, &this->context) != JsNoError)
            throw FatalRuntimeException();

        JsSetCurrentContext(this->context);

        Runtime::contextNumber = 0;

        // Setup native function binds from JS
        Native::Init();

        // Initialize object members
        this->modules = JS::Object();

        // Register bindings from C++ to JS
        this->RegisterBindings();

        this->eventLoop.InitPromiseCallback();
    }

    JS::Value Runtime::RunContext(JS::Value context, bool inheritModules, const std::string& name, const std::string& script) {
        JsContextRef tempContext;
        if (JsCreateContext(this->runtime, &tempContext) != JsNoError)
            throw FatalRuntimeException();

        // Attempt to switch to temporary context
        if (JsSetCurrentContext(tempContext) != JsNoError) {
            JsSetCurrentContext(this->context);
            throw FatalRuntimeException();
        }

        JS::Object global = JS::Object::GetGlobalObject();

        // Inherit module object from runtime
        if (inheritModules)
            JS::Native::ObjectAssign(global, this->modules);

        // Inherit provided values for this
        JS::Native::ObjectAssign(global, context);

        JS::Value result = this->RunBasic(name, script);
        
        // Write the context back to the context object
        JS::Native::ObjectAssign(context, global);

        // Switch back to main context
        if (JsSetCurrentContext(this->context) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JS::Value Runtime::RunContext(const std::string& name, const std::string& script) {
        JsContextRef tempContext;
        if (JsCreateContext(this->runtime, &tempContext) != JsNoError)
            throw FatalRuntimeException();

        // Attempt to switch to temporary context
        if (JsSetCurrentContext(tempContext) != JsNoError) {
            JsSetCurrentContext(this->context);
            throw FatalRuntimeException();
        }

        JS::Object global = JS::Object::GetGlobalObject();

        // Inherit module object from runtime
        JS::Native::ObjectAssign(global, this->modules);

        JS::Value result = this->RunBasic(name, script);

        // Switch back to main context
        if (JsSetCurrentContext(this->context) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JS::Value Runtime::Run(const std::string& name, const std::string& script) {
        // Inherit all requires of the require object
        JS::Object global = JS::Object::GetGlobalObject();
        JS::Native::ObjectAssign(global, this->modules);

        return this->RunBasic(name, script);
    }

    JS::Value Runtime::RunBasic(const std::string& name, const std::string& script) {
        JsValueRef result;

        if (JsRun(JS::String(script.c_str()), Runtime::contextNumber++, JS::String(name.c_str()), JsParseScriptAttributeNone, &result) != JsNoError) {
            // Get exception object
            JsValueRef jsException;
            if (JsGetAndClearException(&jsException) != JsNoError)
                throw FatalRuntimeException();

            if (this->errorCallback != nullptr) {
                this->errorCallback(JS::Object(jsException));
            }
        }

        if (this->errorCallback != nullptr) {
            this->eventLoop.SetErrorCallback(this->errorCallback);
        }

        // Resolve all promises and continuation callbacks
        this->eventLoop.Loop();

        // Return undefined if the result is empty
        if (result == JS_INVALID_REFERENCE)
            return JS::Value();

        return JS::Value(result);
    }

    Runtime::~Runtime() {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(this->runtime);
    }

    void Runtime::Register(const char *name, JS::Value value) {
        JS::Object modules(this->modules);

        modules.SetProperty(name, value);
    }

    void Runtime::RegisterBindings() {
        this->Register("setTimeout", JS::Common::CreateFunction(Bindings::NativeSetTimeout, this));
        this->Register("setInterval", JS::Common::CreateFunction(Bindings::NativeSetTimeout, this));

        // Clear interval/timeout, same function with two aliases
        this->Register("clearTimeout", JS::Common::CreateFunction(Bindings::NativeClearTimeout, nullptr));
        this->Register("clearInterval", JS::Common::CreateFunction(Bindings::NativeClearTimeout, nullptr));
    }

    void Runtime::ThrowException(const char *error) {
        JS::String value(error);
        JsValueRef errorObject;
        if (JsCreateError(value, &errorObject) != JsNoError) return;
        if (JsSetException(errorObject) != JsNoError) return;
    }

    void Runtime::SetErrorCallback(std::function<void(JS::Object)> callback) {
        this->errorCallback = callback;
    }
}