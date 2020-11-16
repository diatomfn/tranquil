#include "native.h"

#include "string.h"
#include "common.h"

namespace JS {
    thread_local JsValueRef Native::jsonStringify = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::jsonParse = JS_INVALID_REFERENCE;

    void Native::Init() {
        Native::jsonStringify = GetFromJS("JSON.stringify");
        Native::jsonParse = GetFromJS("JSON.parse");
    }

    JsValueRef Native::GetFromJS(const char *identifier) {
        JsValueRef toReturn = JS::String::ToJS(identifier);

        JsValueRef name = JS::String::ToJS(identifier);

        JsValueRef result;
        JsRun(toReturn, 0, name, JsParseScriptAttributeNone, &result);

        return result;
    }

    JsValueRef Native::JSONStringify(JsValueRef object) {
        JsValueRef function;

        auto parseFormat = [](JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) -> JsValueRef {
            if (argumentCount < 3) {
                return JS_INVALID_REFERENCE;
            }

            JsValueType type;
            if (JsGetValueType(args[2], &type) != JsNoError)
                return args[2];

            if (type == JsFunction)
                return JS::String::ToJS("[function]");

            return args[2];
        };

        if (JsCreateFunction(parseFormat, nullptr, &function) != JsNoError)
            throw FatalRuntimeException();

        JsValueRef arguments[] = {JS::Common::GetUndefined(), object, function};

        JsValueRef result;

        if (JsCallFunction(Native::jsonStringify, arguments, 3, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::JSONParse(JsValueRef string) {
        JsValueRef arguments[] = {JS::Common::GetUndefined(), string};

        JsValueRef result;

        if (JsCallFunction(Native::jsonParse, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }
}