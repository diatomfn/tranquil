#include "bindings.h"

#include "runtime.h"

#include "string.h"
#include "number.h"

#include "boolean.h"

#include "common.h"

namespace JS {
    JsValueRef Bindings::NativeRequire(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        auto* runtime = static_cast<Runtime*>(callbackState);

        if (argumentCount < 2)
            return JS_INVALID_REFERENCE;

        std::string requireName = JS::String::FromJS(args[1]);

        if (runtime->requireMap.find(requireName) != runtime->requireMap.end()) {
            return runtime->requireMap[requireName];
        }

        return JS_INVALID_REFERENCE;
    }

    JsValueRef Bindings::NativeSetTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        auto* runtime = static_cast<Runtime*>(callbackState);

        if (isConstructCall || argumentCount < 3)
            return JS_INVALID_REFERENCE;

        JsValueRef func = args[1];
        int delay = JS::Number::FromJS(args[2]);

        runtime->eventLoop.taskQueue.push(new Task(func, delay, args[0], JS_INVALID_REFERENCE, JS::Boolean::ToJS(false)));

        return JS_INVALID_REFERENCE;
    }

    JsValueRef Bindings::NativeSetInterval(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        auto* runtime = static_cast<Runtime*>(callbackState);

        if (isConstructCall || argumentCount < 3)
            return JS_INVALID_REFERENCE;

        JsValueRef func = args[1];
        int delay = JS::Number::FromJS(args[2]);

        JsValueRef jsObj = JS::Common::GetNewObject();
        JS::Object object(jsObj);

        // Set meta values of interval object
        JsValueRef repeat = JS::Boolean::ToJS(true);
        object.SetProperty("_repeat", repeat);

        runtime->eventLoop.taskQueue.push(new Task(func, delay, args[0], JS_INVALID_REFERENCE, repeat));

        return object.jsObject;
    }
}