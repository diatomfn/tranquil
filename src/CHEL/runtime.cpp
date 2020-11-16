#include "runtime.h"

#include "FatalRuntimeException.h"

// Include JS type wrappers
#include "number.h"
#include "string.h"
#include "object.h"
#include "native.h"
#include "common.h"

JsValueRef Log(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) {
    std::cout << JS::String::FromJS(arguments[1]) << std::endl;
    return JS_INVALID_REFERENCE;
}

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

        this->eventLoop.SetCallback();

        JsValueRef globalRef = JS::Common::GetGlobalObject();
        JS::Object global(globalRef);

        global.SetFunctionProperty("require", METHOD_BIND(Runtime, &Runtime::RequireNative), this);
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

    JsValueRef Runtime::RequireNative(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount) {
        if (argumentCount < 1) {
            return JS_INVALID_REFERENCE;
        }

        std::string requireName = JS::String::FromJS(args[1]);

        if (this->requireMap.find(requireName) != this->requireMap.end()) {
            return this->requireMap[requireName];
        }

        return JS_INVALID_REFERENCE;
    }
}