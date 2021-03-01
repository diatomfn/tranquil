#include "native.h"
#include "runtime.h"

namespace tranquil {
    thread_local JsValueRef Native::jsonStringify = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::jsonParse = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::objectAssign = JS_INVALID_REFERENCE;
    thread_local JsValueRef Native::objectValues = JS_INVALID_REFERENCE;

    void Native::Init(Context& context) {
        // Copy native functions from runtime before user can change references
        Native::jsonStringify = GetFromJS("JSON.stringify", context);
        Native::jsonParse = GetFromJS("JSON.parse", context);
        Native::objectAssign = GetFromJS("Object.assign", context);
        Native::objectValues = GetFromJS("Object.values", context);
    }

    JsValueRef Native::GetFromJS(const std::string& identifier, Context& context) {
        JsValueRef toReturn = tranquil::Value(identifier);
        JsValueRef name = tranquil::Value(identifier);

        JsValueRef result;

        if (JsRun(toReturn, context.contextNumber++, name, JsParseScriptAttributeNone, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::JSONStringify(JsValueRef object) {
        JsValueRef arguments[] = {tranquil::Value(), object};

        JsValueRef result;

        if (JsCallFunction(Native::jsonStringify, arguments, 3, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::JSONParse(JsValueRef string) {
        JsValueRef arguments[] = {tranquil::Value(), string};

        JsValueRef result;

        if (JsCallFunction(Native::jsonParse, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::ObjectAssign(JsValueRef destination, JsValueRef from) {
        JsValueRef arguments[] = {tranquil::Value(), destination, from};

        JsValueRef result;

        if (JsCallFunction(Native::objectAssign, arguments, 3, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }

    JsValueRef Native::ObjectValues(JsValueRef object) {
        JsValueRef arguments[] = {tranquil::Value(), object};

        JsValueRef result;

        if (JsCallFunction(Native::objectValues , arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return result;
    }
}
