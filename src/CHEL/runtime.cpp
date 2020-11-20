#include "runtime.h"

#include "FatalRuntimeException.h"

//// Include JS type wrappers
//#include "number.h"
//#include "string.h"
//#include "object.h"
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

        // Register bindings from C++ to JS
        this->RegisterBindings();

        this->eventLoop.SetCallback();
    }

    JS::Output::Log Runtime::Run(const std::string &script) {
        if (JsSetCurrentContext(this->context) != JsNoError)
            throw FatalRuntimeException();

        JsValueRef jScript = JS::String(script.c_str());
        JsValueRef name = JS::String("script");

        JsValueRef result;

        if (JsRun(jScript, 0, name, JsParseScriptAttributeNone, &result) != JsNoError) {
            // Get exception object
            JsValueRef jsException;
            if (JsGetAndClearExceptionWithMetadata(&jsException) != JsNoError)
                throw FatalRuntimeException();

            // Get main exception object
            JS::Object exceptionMeta(jsException);

            JsValueRef exceptionValue = (JsValueRef)exceptionMeta.GetProperty("exception");
            JS::Object exception(exceptionValue);

            JsValueRef messageValue = (JsValueRef)exception.GetProperty("message");
            JsValueRef lineValue = (JsValueRef)exceptionMeta.GetProperty("line");

            std::string message = JS::String(messageValue);
            int line = (int)(double)JS::Number(lineValue);

            // Push the error to the log
            logOutput.Push(message, Output::LogType::ERROR, line);
        }

        this->eventLoop.Loop();

        return logOutput;
    }

    Runtime::~Runtime() {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(this->runtime);
    }

    void Runtime::Register(const char *name, JS::Value value, bool isGlobal) {
        if (isGlobal) {
            JS::Object global(JS::Common::GetGlobalObject());

            // Set return to result
            global.SetProperty(name, value);
        } else {
            this->requireMap.insert(std::pair<std::string, JsValueRef>(std::string(name), value));
        }
    }

    void Runtime::RegisterBindings() {
        JsValueRef globalRef = JS::Common::GetGlobalObject();
        JS::Object global(globalRef);

        global.SetProperty("require", Bindings::NativeRequire, this);
        global.SetProperty("setTimeout", Bindings::NativeSetTimeout, this);
        global.SetProperty("setInterval", Bindings::NativeSetInterval, this);

        // Clear interval/timeout, same function with two aliases
        global.SetProperty("clearTimeout", Bindings::NativeClearTimeout, nullptr);
        global.SetProperty("clearInterval", Bindings::NativeClearTimeout, nullptr);
    }

    void Runtime::ThrowException(const char *error) {
        JS::String value(error);
        JsValueRef errorObject;
        if (JsCreateError(value, &errorObject) != JsNoError) return;
        if (JsSetException(errorObject) != JsNoError) return;
    }
}