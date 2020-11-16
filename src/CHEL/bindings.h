#pragma once

#include "pch.h"

namespace JS {
    class Bindings {
    public:
        static JsValueRef NativeRequire(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState);
        static JsValueRef NativeSetTimeout(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState);
        static JsValueRef NativeSetInterval(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState);
    };
}