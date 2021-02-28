#pragma once

#include "pch.h"

namespace JS {
    class Bindings {
    public:
        static Value SomeFunction(bool isConstructor, std::vector<Value> args, void* callbackState) {
            
        }
    //         JsValueRef Bindings::NativeSetTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState) {
    //     try {
    //         auto* runtime = static_cast<Runtime*>(callbackState);

    //         if (isConstructCall || argumentCount < 3)
    //             return JS_INVALID_REFERENCE;

    //         JsValueRef func = args[1];
    //         JsValueRef time = args[2];

    //         if (!JS::Common::AssertArgument(func, JsFunction, true)) return JS_INVALID_REFERENCE;
    //         if (!JS::Common::AssertArgument(time, JsNumber, true)) return JS_INVALID_REFERENCE;

    //         JS::Timeout timeout(func, time, JS::Value(false));
    //         runtime->eventLoop.taskQueue.push(new Task(timeout.object, args[0], JS_INVALID_REFERENCE));

    //         return timeout.object;
    //     } catch (...) {
    //         return JS_INVALID_REFERENCE;
    //     }
    // }
        static JsValueRef NativeSetTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState);
        static JsValueRef NativeSetInterval(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState);
        static JsValueRef NativeClearTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void *callbackState);
    };
}