#include "native.h"

#include "string.h"
#include "common.h"

#include "runtime.h"

namespace JS {
    thread_local JsValueRef Native::jsonStringify = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::jsonParse = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::objectAssign = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::objectValues = JS_INVALID_REFERENCE;

    void Native::Init() {
        // Copy native functions from runtime before user can change references
        Native::jsonStringify = GetFromJS("JSON.stringify");
        Native::jsonParse = GetFromJS("JSON.parse");
        Native::objectAssign = GetFromJS("Object.assign");
        Native::objectValues = GetFromJS("Object.values");
    }

    JsValueRef Native::GetFromJS(const std::string& identifier) {
        JsValueRef toReturn = JS::Value(identifier);
        JsValueRef name = JS::Value(identifier);

        JsValueRef result;

        if (JsRun(toReturn, Runtime::contextNumber++, name, JsParseScriptAttributeNone, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::JSONStringify(JsValueRef object) {
        JsValueRef arguments[] = {JS::Value(), object};

        JsValueRef result;

        if (JsCallFunction(Native::jsonStringify, arguments, 3, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::JSONParse(JsValueRef string) {
        JsValueRef arguments[] = {JS::Value(), string};

        JsValueRef result;

        if (JsCallFunction(Native::jsonParse, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::ObjectAssign(JsValueRef destination, JsValueRef from) {
        JsValueRef arguments[] = {JS::Value(), destination, from};

        JsValueRef result;

        if (JsCallFunction(Native::objectAssign, arguments, 3, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }
}
