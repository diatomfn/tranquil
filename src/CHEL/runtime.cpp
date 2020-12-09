#include "runtime.h"

#include "FatalRuntimeException.h"

#include "types/types.h"
#include "native.h"
#include "common.h"

namespace JS {
    Runtime::Runtime(int memoryLimit) {
        if (JsCreateRuntime(JsRuntimeAttributeDisableFatalOnOOM, nullptr, &this->runtime) != JsNoError)
            throw FatalRuntimeException();

        JsSetRuntimeMemoryLimit(this->runtime, memoryLimit * 1048576);

        if (JsCreateContext(this->runtime, &this->context) != JsNoError)
            throw FatalRuntimeException();

        JsSetCurrentContext(this->context);

        // Setup native function binds from JS
        Native::Init();

        // Create module object
        this->modules = JS::Object();

        // Register bindings from C++ to JS
        this->RegisterBindings();

        this->eventLoop.SetCallback();
    }

    JS::Value Runtime::RunContext(JS::Value context, bool inheritModules, const std::string& script) {
        JsContextRef tempContext;
        if (JsCreateContext(this->runtime, &tempContext) != JsNoError)
            throw FatalRuntimeException();

        // Attempt to switch to temporary context
        if (JsSetCurrentContext(tempContext) != JsNoError) {
            JsSetCurrentContext(this->context);
            throw FatalRuntimeException();
        }

        // Write temporary context values
        JsValueRef global = JS::Common::GetGlobalObject();

        // Inherit module object from runtime
        if (inheritModules)
            JS::Native::ObjectAssign(global, this->modules);

        // Inherit provided values for this
        JS::Native::ObjectAssign(global, context);

        JS::Value result = this->RunBasic("script", script);
        
        // Write the context back to the context object
        JS::Native::ObjectAssign(context, global);

        // Switch back to main context
        if (JsSetCurrentContext(this->context) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JS::Value Runtime::Run(const std::string& script) {
        // Inherit all requires of the require object
        JsValueRef global = JS::Common::GetGlobalObject();
        JS::Native::ObjectAssign(global, this->modules);

        return this->RunBasic("script", script);
    }

    JS::Value Runtime::RunBasic(const std::string& scriptName, const std::string& script) {
        JsValueRef result;

        if (JsRun(JS::String(script.c_str()), 0, JS::String(scriptName.c_str()), JsParseScriptAttributeNone, &result) != JsNoError) {
            // Get exception object
            JsValueRef jsException;
            if (JsGetAndClearExceptionWithMetadata(&jsException) != JsNoError)
                throw FatalRuntimeException();

            // Get main exception object
            JS::Object exceptionMeta(jsException);

            JsValueRef exceptionValue = exceptionMeta.GetProperty("exception");
            JS::Object exception(exceptionValue);

            JsValueRef messageValue = exception.GetProperty("message");
            JsValueRef lineValue = exceptionMeta.GetProperty("line");

            std::string message = JS::String(messageValue);
            int line = (int)(double)JS::Number(lineValue);

            // Push the error to the log
            logOutput.Push(message, Output::LogType::ERROR, line);
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
}