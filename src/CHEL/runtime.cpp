#include "runtime.h"

#include "FatalRuntimeException.h"

// Include JS type wrappers
#include "number.h"
#include "string.h"
#include "object.h"
#include "native.h"
#include "common.h"

#define BIND_NATIVE(object, name, function) object.SetFunctionProperty(name, function, this)

namespace JS {
    Runtime::Runtime() {
        if (JsCreateRuntime(JsRuntimeAttributeDisableFatalOnOOM, nullptr, &this->runtime) != JsNoError)
            throw FatalRuntimeException();

        JsSetRuntimeMemoryLimit(this->runtime, 10000000);

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

        JsValueRef jScript = JS::String::ToJS(script.c_str());
        JsValueRef name = JS::String::ToJS("script");

        JsValueRef result;

        if (JsRun(jScript, 0, name, JsParseScriptAttributeNone, &result)) {
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

            std::string message = JS::String::FromJS(messageValue);
            int line = JS::Number::FromJS(lineValue);

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

    void Runtime::Register(const char *name, JsValueRef value, bool isGlobal) {
        if (isGlobal) {
            JsValueRef globalRef = JS::Common::GetGlobalObject();

            JS::Object global(globalRef);

            // Set return to result
            global.SetProperty(name, value);
        } else {
            this->requireMap.insert(std::pair<std::string, JsValueRef>(std::string(name), value));
        }
    }

    void Runtime::RegisterBindings() {
        JsValueRef globalRef = JS::Common::GetGlobalObject();
        JS::Object global(globalRef);

        global.SetFunctionProperty("require", Bindings::NativeRequire, this);
        global.SetFunctionProperty("setTimeout", Bindings::NativeSetTimeout, this);
        global.SetFunctionProperty("setInterval", Bindings::NativeSetInterval, this);

        // Clear interval/timeout, same function with two aliases
        global.SetFunctionProperty("clearTimeout", Bindings::NativeClearTimeout, nullptr);
        global.SetFunctionProperty("clearInterval", Bindings::NativeClearTimeout, nullptr);
    }
}