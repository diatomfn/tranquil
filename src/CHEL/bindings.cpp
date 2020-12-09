#include "bindings.h"

#include "runtime.h"
#include "common.h"

namespace JS {
    JsValueRef Bindings::NativeSetTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        try {
            auto* runtime = static_cast<Runtime*>(callbackState);

            if (isConstructCall || argumentCount < 3)
                return JS_INVALID_REFERENCE;

            JsValueRef func = args[1];
            JsValueRef time = args[2];

            if (!JS::Common::AssertArgument(func, JsFunction, true)) return JS_INVALID_REFERENCE;
            if (!JS::Common::AssertArgument(time, JsNumber, true)) return JS_INVALID_REFERENCE;

            JS::Timeout timeout(func, time, JS::Boolean(false));
            runtime->eventLoop.taskQueue.push(new Task(timeout.object, args[0], JS_INVALID_REFERENCE));

            return timeout.object;
        } catch (...) {
            return JS_INVALID_REFERENCE;
        }
    }

    JsValueRef Bindings::NativeSetInterval(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        try {
            auto* runtime = static_cast<Runtime*>(callbackState);

            if (isConstructCall || argumentCount < 3)
                return JS_INVALID_REFERENCE;

            JsValueRef func = args[1];
            JsValueRef time = args[2];

            if (!JS::Common::AssertArgument(func, JsFunction, true)) return JS_INVALID_REFERENCE;
            if (!JS::Common::AssertArgument(time, JsNumber, true)) return JS_INVALID_REFERENCE;

            JS::Timeout timeout(func, time, JS::Boolean(false));
            runtime->eventLoop.taskQueue.push(new Task(timeout.object, args[0], JS_INVALID_REFERENCE, true));

            return timeout.object;
        } catch (...) {
            return JS_INVALID_REFERENCE;
        }
    }

    JsValueRef Bindings::NativeClearTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
        try {
            if (isConstructCall || argumentCount < 2)
                return JS_INVALID_REFERENCE;

            if (!JS::Common::AssertArgument(args[1], JsObject, false)) return JS_INVALID_REFERENCE;

            Timeout timeout(args[1]);
            timeout.Destroy();

            return JS_INVALID_REFERENCE;
        } catch (...) {
            return JS_INVALID_REFERENCE;
        }
    }
}