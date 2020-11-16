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

        JS::Timeout timeout(func, args[2], JS::Boolean::ToJS(false));

        runtime->eventLoop.taskQueue.push(new Task(timeout.object, args[0], JS_INVALID_REFERENCE));

        return timeout.object;
    }

    JsValueRef Bindings::NativeSetInterval(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        auto* runtime = static_cast<Runtime*>(callbackState);

        if (isConstructCall || argumentCount < 3)
            return JS_INVALID_REFERENCE;

        JsValueRef func = args[1];

        JS::Timeout timeout(func, args[2], JS::Boolean::ToJS(false));

        runtime->eventLoop.taskQueue.push(new Task(timeout.object, args[0], JS_INVALID_REFERENCE, true));

        return timeout.object;
    }

    JsValueRef Bindings::NativeClearTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        if (isConstructCall || argumentCount < 2)
            return JS_INVALID_REFERENCE;

        Timeout timeout(args[1]);
        timeout.Destroy();

        return JS_INVALID_REFERENCE;
    }
}