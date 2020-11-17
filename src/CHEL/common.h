#pragma once

#include "pch.h"
#include "FatalRuntimeException.h"

#include "runtime.h"

namespace JS::Common {
    JsValueRef GetUndefined();
    JsValueRef GetGlobalObject();
    bool IsPromise(JsValueRef value);
    JsValueRef GetNewObject();
    JsValueRef CreateFunction(JsNativeFunction function, void* callbackState);
    JsValueType GetType(JsValueRef value);
    const char* GetTypeString(JsValueType type);
    bool CheckArgument(JsValueRef value, JsValueType type, bool exception);
}